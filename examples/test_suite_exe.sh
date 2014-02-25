#!/bin/sh 
# $Id$

#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2007 - 2010 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

#
# Run tests for all VMC examples from built executables and regenerate output files.
# Only g4Config.C and g3tgeoConfig.C configurations are tested.
#
# By I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
OUTDIR=$CURDIR/log_exe
EXEDIR=$CURDIR/../examples_build_

# Set 1 to 0 if you want to skip given MC
TESTG3=1
TESTG4=0

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

  echo "... Example $EXAMPLE"
  
  if [ "$EXAMPLE" != "E03" -a "$EXAMPLE" != "A01" ]; then 
    if [ "$TESTG3" = "1" ]; then
      EXE=$EXEDIR"g3/"$EXAMPLE"/example"$EXAMPLE
      echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
      $EXE "test_$EXAMPLE.C" "\"\"" "kFALSE" >& $OUT/test_g3_tgeo_tgeo.out   
    fi  
    if [ "$TESTG4" = "1" ]; then
      EXE=$EXEDIR"g4/"$EXAMPLE"/example"$EXAMPLE
      echo "... Running test with G4, geometry via TGeo, Native navigation" 
      $EXE "test_$EXAMPLE.C" "\"\"" "kFALSE" >& $OUT/test_g4_tgeo_nat.out   
    fi  
  fi  

  if [ "$EXAMPLE" = "E03" ]; then 
    # Run all macros + special configuration available only in E03 example
    if [ "$TESTG3" = "1" ]; then
      EXE=$EXEDIR"g3/"$EXAMPLE"/example"$EXAMPLE
      echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
      $EXE "test_E03_1.C" "\"\"" "kFALSE" >& $OUT/test_g3_tgeo_tgeo.out   
      $EXE "test_E03_2.C" "\"\"" "kFALSE" >& tmpfile    
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      $EXE "test_E03_3.C" "\"\"" "kFALSE" >& tmpfile
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      $EXE "test_E03_4.C" "\"\"" "kFALSE" >& tmpfile
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      $EXE "test_E03_5.C" "\"\"" "kFALSE" >& tmpfile
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
    fi  
    # Run all macros + special configuration available only in E03 example
    if [ "$TESTG4" = "1" ]; then
      EXE=$EXEDIR"g4/"$EXAMPLE"/example"$EXAMPLE
      echo "... Running test with G4, geometry via TGeo, Native navigation" 
      $EXE "test_E03_1.C" "\"\"" "kFALSE" >& $OUT/test_g4_tgeo_nat.out   
      $EXE "test_E03_2.C" "\"\"" "kFALSE" >& tmpfile    
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      $EXE "test_E03_3.C" "\"\"" "kFALSE" >& tmpfile
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      $EXE "test_E03_4.C" "\"\"" "kFALSE" >& tmpfile
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      $EXE "test_E03_5.C" "\"\"" "kFALSE" >& tmpfile
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
    fi
  fi    

  if [ "$EXAMPLE" = "A01" ]; then 
    if [ "$TESTG3" = "1" ]; then
      EXE=$EXEDIR"g3/"$EXAMPLE"/example"$EXAMPLE
      echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
      $EXE "test_$EXAMPLE.C" "\"\"" "kFALSE" >& $OUT/test_g3_tgeo_tgeo.out   
    fi  
    if [ "$TESTG4" = "1" ]; then
      EXE=$EXEDIR"g4/"$EXAMPLE"/example"$EXAMPLE
      echo "... Running test with G4, geometry via TGeo, Native navigation" 
      $EXE "test_$EXAMPLE.C" "\"\"" "kFALSE" >& $OUT/test_g4_tgeo_nat.out   
    fi  
  fi  

  echo " "
done  
  
cd $CURDIR
