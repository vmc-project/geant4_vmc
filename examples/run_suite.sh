#!/bin/sh
#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2007 - 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

#
# Run tests for all VMC examples from run_g[3,4].C macros.
# Only g4tgeoConfig.C and g3tgeoConfig.C configurations are tested.
#
# Usage:
# run_suite.sh [--g3=on|off] [--g4=on|off] [--garfield=on|off] [--examples="E01 E03 ..."]
#
# by I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
OUTDIR=$CURDIR/logs/run

# Options
TESTG3="1"
TESTG4="1"
TESTGARFIELD="1"

# The default list of examples (all)
ALL_EXAMPLES="E01 E02 E03 E06 A01 ExGarfield Gflash Monopole TR"
EXAMPLES="$ALL_EXAMPLES"

# Function arguments:
# {1}  Load macro: g3[a,b], g4[a,b]
# {2}  Run macro:  g3, g4
# {3}  Config macro: g3, g3tgeo, g4, g4tgeo
function run_mc()
{
  echo "    - with ${1}, config ${3}"
  root.exe -q -b load_${1}.C run_${2}.C\(\"${3}Config.C\"\) >& $OUT/run_${3}.out
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
                       echo "run_suite.sh [--g3=on|off] [--g4=on|off] [--garfield=on|off] [--examples=\"E01 E03 ...\"] "
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
  cd $CURDIR/$EXAMPLE
  OUT=$OUTDIR/$EXAMPLE
  if [ ! -d $OUT ]; then
    mkdir -p $OUT
  fi

  # skip Garfield if switch off
  if [ "$EXAMPLE" = "ExGarfield" -a "$TESTGARFIELD" = "0" ]; then
    continue 1
  fi

  echo "... Running example $EXAMPLE"

  if [ "$TESTG3" = "1" -a "$EXAMPLE" != "Monopole" ]; then
    if [ "$EXAMPLE" = "E03" ]; then
      run_mc g3a g3 g3tgeo
      run_mc g3b g3 g3tgeo
    else
      run_mc g3 g4 g3tgeo
    fi
  fi

  if [ "$TESTG4" = "1" ]; then
    if [ "$EXAMPLE" = "E03" ]; then
      run_mc g4a g4 g4tgeo
      run_mc g4b g4 g4tgeo
    else
      run_mc g4 g4 g4tgeo
    fi
  fi

  # clean-up generated files
  rm -f Example*.root
  rm -f gphysi.dat
done

cd $CURDIR
