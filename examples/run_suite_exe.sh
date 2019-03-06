#!/bin/sh 
#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

#
# Run tests for all VMC examples from built executables.
# Only g4tgeoConfig.C and g3tgeoConfig.C configurations are tested.
# The executables are supposed to be on the executable path.
#
# Usage:
# run_suite_exe.sh [--g3=on|off] [--g4=on|off] [--garfield=on|off] [--examples="E01 E03 ..."]
#
# By I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
OUTDIR=$CURDIR/logs/run_exe

# Set 1 to 0 if you want to skip given MC or Garfield test
TESTG3="1"
TESTG4="1"

# Run Garfield optionally
TESTGARFIELD="1"

# The default list of examples (all)
ALL_EXAMPLES="E01 E02 E03 E06 A01 ExGarfield Gflash Monopole TR"
EXAMPLES="$ALL_EXAMPLES"

# Function arguments:
# {1}  Run macro:  g3[q,b], g4[a,b]
function run_mc_exe()
{
  echo "... Running ${1}vmc_example$EXAMPLE"
  ${1}vmc_example$EXAMPLE >& $OUT/${1}vmc_example$EXAMPLE.out
}


# Process script arguments
for arg in "${@}"
do
  #echo "got: $arg"
  case $arg in
    "--g3=on"        ) TESTG3="1" ;;
    "--g3=off"       ) TESTG3="0" ;;
    "--g4=on"        ) TESTG4="1" ;;
    "--g4=off"       ) TESTG4="0" ;;
    "--garfield=on"  ) TESTGARFIELD="1" ;;
    "--garfield=off" ) TESTGARFIELD="0" ;;
     --examples=*    ) EXAMPLES=${arg#--examples=} ;;
    *                ) echo "Unsupported option $arg chosen."
                       echo "run_suite_exe.sh [--g3=on|off] [--g4=on|off] [--garfield=on|off] [--examples=\"E01 E03 ...\"]"
                       exit 1
                       ;;
  esac
done

# Recreate log directory only if running test for both G3 and G4
if [ "$TESTG3" = "1" -a  "$TESTG4" = "1"  -a "$EXAMPLES" = "$ALL_EXAMPLES" ]; then
  rm -fr $OUTDIR
fi  

for EXAMPLE in $EXAMPLES
do
  OUT=$OUTDIR/$EXAMPLE
  if [ ! -d $OUT ]; then
    mkdir -p $OUT
  fi

  # skip Garfield if switch off
  if [ "$EXAMPLE" = "ExGarfield" -a "$TESTGARFIELD" = "0" ]; then
    continue 1
  fi

  cd $CURDIR/$EXAMPLE
  
  if [ "$TESTG3" = "1" -a "$EXAMPLE" != "Monopole" ]; then
    if [ "$EXAMPLE" = "E03" ]; then
      run_mc_exe g3a
      run_mc_exe g3b
    else
      run_mc_exe g3
    fi
  fi  

  if [ "$TESTG4" = "1" ]; then
    if [ "$EXAMPLE" = "E03" ]; then
      run_mc_exe g4a
      run_mc_exe g4b
    else
      run_mc_exe g4
    fi
  fi  
  echo " "

  # clean-up generated files
  rm -f Example*.root
  rm -f gphysi.dat
done  
  
cd $CURDIR
