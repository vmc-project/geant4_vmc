#!/bin/sh 
# $Id$

#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
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

# Set 1 to 0 if you want to skip given MC
TESTG3=1
TESTG4=1

# Recreate log directory only if running test for both G3 and G4
if [ "$TESTG3" = "1" -a  "$TESTG4" = "1" ]; then
  rm -fr $OUTDIR
fi  

for EXAMPLE in E01 E02 E03 E06 A01
do
  OUT=$OUTDIR/$EXAMPLE
  if [ ! -d $OUT ]; then
    mkdir -p $OUT
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
