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
OUTDIR=$CURDIR/log_exe
EXEDIR=$CURDIR/../examples_build_

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

  echo "... Example $EXAMPLE"
  
  if [ "$EXAMPLE" != "E03" -a "$EXAMPLE" != "A01" ]; then 
    if [ "$TESTG3" = "1" ]; then
      EXE=$EXEDIR"g3/"$EXAMPLE"/test"$EXAMPLE
      echo "... Running test with G3, geometry via TGeo, TGeo navigation"
      $EXE -g3g TGeant3TGeo -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out

      echo "... Running test with G3, geometry via VMC,  Native navigation"
      $EXE -g3g TGeant3 -rm "test_$EXAMPLE.C(\"\", kTRUE)" >& $OUT/test_g3_vmc_nat.out

      echo "... Running test with G3, geometry via VMC,  TGeo navigation"
      $EXE -g3g TGeant3TGeo -rm "test_$EXAMPLE.C(\"\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out
    fi  
    if [ "$TESTG4" = "1" ]; then
      EXE=$EXEDIR"g4/"$EXAMPLE"/test"$EXAMPLE
      echo "... Running test with G4, geometry via TGeo, Native navigation"
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out

      echo "... Running test with G4, geometry via TGeo, TGeo navigation"
      $EXE -g4g geomRoot -g4vm "" -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out

      echo "... Running test with G4, geometry via VMC,  Native navigation"
      $EXE -g4g geomVMCtoGeant4 -g4vm "" -rm "test_$EXAMPLE.C(\"\", kTRUE)" >& $OUT/test_g4_vmc_nat.out

      echo "... Running test with G4, geometry via VMC,  TGeo navigation"
      $EXE -g4g geomVMCtoRoot -g4vm "" -rm "test_$EXAMPLE.C(\"\", kTRUE)" >& $OUT/test_g4_vmc_tgeo.out
    fi  
  fi  

  if [ "$EXAMPLE" = "E03" ]; then 
    # Run all macros + special configuration available only in E03 test
    if [ "$TESTG3" = "1" ]; then
      EXE=$EXEDIR"g3/"$EXAMPLE"/test"$EXAMPLE
      echo "... Running test with G3, geometry via TGeo, TGeo navigation"
      $EXE -g3g TGeant3TGeo -rm "test_E03_1.C(\"\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out   
      $EXE -g3g TGeant3TGeo -rm "test_E03_2.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_3.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_4.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_5.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out

      echo "... Running test with G3, geometry via VMC,  Native navigation"
      $EXE -g3g TGeant3 -rm "test_E03_1.C(\"\", kTRUE)" >& $OUT/test_g3_vmc_nat.out
      $EXE -g3g TGeant3 -rm "test_E03_2.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      $EXE -g3g TGeant3 -rm "test_E03_3.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      $EXE -g3g TGeant3 -rm "test_E03_4.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      $EXE -g3g TGeant3 -rm "test_E03_5.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_vmc_nat.out

      echo "... Running test with G3, geometry via VMC,  TGeo navigation"
      $EXE -g3g TGeant3TGeo -rm "test_E03_1.C(\"\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_2.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_3.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_4.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_5.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out

    fi  
    # Run all macros + special configuration available only in E03 test
    if [ "$TESTG4" = "1" ]; then
      EXE=$EXEDIR"g4/"$EXAMPLE"/test"$EXAMPLE
      echo "... Running test with G4, geometry via TGeo, Native navigation"
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_E03_1.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_E03_2.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_E03_3.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_E03_4.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      $EXE -g4g geomRootToGeant4 -g4sp specialCuts+specialControls -g4vm "" -rm "test_E03_5.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out

      echo "... Running test with G4, geometry via TGeo, TGeo navigation"
      $EXE -g4g geomRoot -g4vm "" -rm "test_E03_1.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   
      $EXE -g4g geomRoot -g4vm "" -rm "test_E03_2.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      $EXE -g4g geomRoot -g4vm "" -rm "test_E03_3.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      $EXE -g4g geomRoot -g4vm "" -rm "test_E03_4.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      $EXE -g4g geomRoot  -g4sp specialCuts+specialControls -g4vm "" -rm "test_E03_5.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out

      echo "... Running test with G4, geometry via VMC,  Native navigation"
      $EXE -g4g geomVMCtoGeant4 -g4vm "" -rm "test_E03_1.C(\"\", kTRUE)" >& $OUT/test_g4_vmc_nat.out   
      $EXE -g4g geomVMCtoGeant4 -g4vm "" -rm "test_E03_2.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_vmc_nat.out
      $EXE -g4g geomVMCtoGeant4 -g4vm "" -rm "test_E03_3.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_vmc_nat.out
      $EXE -g4g geomVMCtoGeant4 -g4vm "" -rm "test_E03_4.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_vmc_nat.out

      echo "... Running test with G4, geometry via VMC,  TGeo navigation"
      $EXE -g4g geomVMCtoRoot -g4vm "" -rm "test_E03_1.C(\"\", kTRUE)" >& $OUT/test_g4_vmc_tgeo.out   
      $EXE -g4g geomVMCtoRoot -g4vm "" -rm "test_E03_2.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
      $EXE -g4g geomVMCtoRoot -g4vm "" -rm "test_E03_3.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
      $EXE -g4g geomVMCtoRoot -g4vm "" -rm "test_E03_4.C(\"\", kTRUE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out

      echo "... Running test with G4, geometry via G4,   Native navigation"
      $EXE -g4uc geometry -g4vm "" -rm "test_E03_1.C(\"\", kFALSE)" >& $OUT/test_g4_g4_nat.out   
      $EXE -g4uc geometry -g4vm "" -rm "test_E03_2.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_g4_nat.out
      $EXE -g4uc geometry -g4vm "" -rm "test_E03_3.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_g4_nat.out
      $EXE -g4uc geometry -g4vm "" -rm "test_E03_4.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_g4_nat.out

      echo "... Running test with G4,  geometry via TGeo, Native navigation, User physics list"
      $EXE -g4uc physics-list -g4vm "" -rm "test_E03_1.C(\"\", kFALSE)" >& $OUT/test_g4_g4_nat_pl.out   
      $EXE -g4uc physics-list -g4vm "" -rm "test_E03_2.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_g4_nat_pl.out
      $EXE -g4uc physics-list -g4vm "" -rm "test_E03_3.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_g4_nat_pl.out
      $EXE -g4uc physics-list -g4vm "" -rm "test_E03_4.C(\"\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g4_g4_nat_pl.out
    fi
  fi    

  if [ "$EXAMPLE" = "A01" ]; then 
    if [ "$TESTG3" = "1" ]; then
      EXE=$EXEDIR"g3/"$EXAMPLE"/test"$EXAMPLE
      echo "... Running test with G3, geometry via TGeo, TGeo navigation"
      $EXE -g3g TGeant3TGeo -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out   
    fi  
    if [ "$TESTG4" = "1" ]; then
      EXE=$EXEDIR"g4/"$EXAMPLE"/test"$EXAMPLE
      echo "... Running test with G4, geometry via TGeo, Native navigation"
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   

      echo "... Running test with G4, geometry via TGeo, TGeo navigation"
      $EXE -g4g geomRoot -g4vm "" -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   

      echo "... Running test with G4, geometry via G4, Native navigation"
      $EXE -g4uc geometry -g4vm "" -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g4_g4_nat.out   
    fi  
  fi  

  echo " "
done  
  
cd $CURDIR
