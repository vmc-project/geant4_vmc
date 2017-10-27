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
# Run tests for all VMC examples from built executables and regenerate output
# files. Only g4Config.C and g3tgeoConfig.C configurations are tested.
# The executables are supposed to be found in
# examples_build_g3/exampleName and examples_build_g4/exampleName
# directories.
#
# By I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
OUTDIR=$CURDIR/log_run_exe
G3EXEDIR=$CURDIR/../../geant3_install/bin
G4EXEDIR=$CURDIR/../../geant4_vmc_install/bin

# Set 1 to 0 if you want to skip given MC or Garfield test
TESTG3="1"
TESTG4="1"

# Run Garfield only with Root 5
TESTGARFIELD="1"
ROOT_VERSION=`root-config --version`
if [ ${ROOT_VERSION:0:1} = "6" ]; then
  TESTGARFIELD="0"
fi

# Recreate log directory only if running test for both G3 and G4
if [ "$TESTG3" = "1" -a  "$TESTG4" = "1" ]; then
  rm -fr $OUTDIR
fi  

for EXAMPLE in E01 E02 E03 E06 A01 ExGarfield Gflash TR
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
  
  if [ "$TESTG3" = "1" ]; then
    EXE=$G3EXEDIR"/g3vmc_example"$EXAMPLE
    echo "... Running g3vmc_example$EXAMPLE"
    $EXE >& $OUT/g3vmc_example$EXAMPLE.out
  fi  

  if [ "$TESTG4" = "1" ]; then
    EXE=$G4EXEDIR"/g4vmc_example"$EXAMPLE
    echo "... Running g4vmc_example$EXAMPLE"
    $EXE >& $OUT/g4vmc_example$EXAMPLE.out
  fi  
  echo " "
done  
  
cd $CURDIR
