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
# Run tests for all VMC examples and regenerate output files
#
# By I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
OUTDIR=$CURDIR/log

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
      echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g3tgeoConfig.C\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out   

      echo "... Running test with G3, geometry via VMC,  Native navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g3Config.C\", kTRUE)" >& $OUT/test_g3_vmc_nat.out   

      echo "... Running test with G3, geometry via VMC,  TGeo navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g3tgeoConfig.C\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out   
    fi  

    if [ "$TESTG4" = "1" ]; then
      echo "... Running test with G4, geometry via TGeo, Native navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4Config.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   
 
      echo "... Running test with G4, geometry via TGeo, TGeo navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4tgeoConfig.C\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   

      echo "... Running test with G4, geometry via VMC,  Native navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4ConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_nat.out   

      echo "... Running test with G4, geometry via VMC,  TGeo navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4tgeoConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_tgeo.out   
    fi  
  fi  

  if [ "$EXAMPLE" = "E03" ]; then 
    # Run three macro + special configuration available only in E03 example
    if [ "$TESTG3" = "1" ]; then
      echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
      root.exe -b -q "test_E03_1.C(\"g3tgeoConfig.C\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out   
      root.exe -b -q "test_E03_2.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      root.exe -b -q "test_E03_3.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      root.exe -b -q "test_E03_4.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      root.exe -b -q "test_E03_5.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out

      echo "... Running test with G3, geometry via VMC,  Native navigation" 
      root.exe -b -q "test_E03_1.C(\"g3Config.C\", kTRUE)" >& $OUT/test_g3_vmc_nat.out   
      root.exe -b -q "test_E03_2.C(\"g3Config.C\", kTRUE)" >& tmpfile   
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      root.exe -b -q "test_E03_3.C(\"g3Config.C\", kTRUE)" >& tmpfile   
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      root.exe -b -q "test_E03_4.C(\"g3Config.C\", kTRUE)" >& tmpfile   
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      root.exe -b -q "test_E03_5.C(\"g3Config.C\", kTRUE)" >& tmpfile   
      cat tmpfile >> $OUT/test_g3_vmc_nat.out

      echo "... Running test with G3, geometry via VMC,  TGeo navigation" 
      root.exe -b -q "test_E03_1.C(\"g3tgeoConfig.C\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out   
      root.exe -b -q "test_E03_2.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      root.exe -b -q "test_E03_3.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      root.exe -b -q "test_E03_4.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      root.exe -b -q "test_E03_5.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
    fi  

    if [ "$TESTG4" = "1" ]; then
      echo "... Running test with G4, geometry via TGeo, Native navigation" 
      root.exe -b -q "test_E03_1.C(\"g4Config.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   
      root.exe -b -q "test_E03_2.C(\"g4Config.C\", kFALSE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      root.exe -b -q "test_E03_3.C(\"g4Config.C\", kFALSE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      root.exe -b -q "test_E03_4.C(\"g4Config.C\", kFALSE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      root.exe -b -q "test_E03_5.C(\"g4Config4.C\", kFALSE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out

      echo "... Running test with G4, geometry via TGeo, TGeo navigation" 
      root.exe -b -q "test_E03_1.C(\"g4tgeoConfig.C\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   
      root.exe -b -q "test_E03_2.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile   
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      root.exe -b -q "test_E03_3.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile   
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      root.exe -b -q "test_E03_4.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile   
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      root.exe -b -q "test_E03_5.C(\"g4tgeoConfig4.C\", kFALSE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out

      echo "... Running test with G4, geometry via VMC,  Native navigation" 
      root.exe -b -q "test_E03_1.C(\"g4ConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_nat.out   
      root.exe -b -q "test_E03_2.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g4_vmc_nat.out
      root.exe -b -q "test_E03_3.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g4_vmc_nat.out
      root.exe -b -q "test_E03_4.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile  
      cat tmpfile >> $OUT/test_g4_vmc_nat.out

      echo "... Running test with G4, geometry via VMC,  TGeo navigation" 
      root.exe -b -q "test_E03_1.C(\"g4tgeoConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_tgeo.out   
      root.exe -b -q "test_E03_2.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile 
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
      root.exe -b -q "test_E03_3.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile 
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
      root.exe -b -q "test_E03_4.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile 
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out

      echo "... Running test with G4, geometry via G4,   Native navigation" 
      root.exe -b -q "test_E03_1.C(\"g4Config1.C\", kFALSE)" >& $OUT/test_g4_g4_nat.out   
      root.exe -b -q "test_E03_2.C(\"g4Config1.C\", kFALSE)" >& tmpfile   
      cat tmpfile >> $OUT/test_g4_g4_nat.out
      root.exe -b -q "test_E03_3.C(\"g4Config1.C\", kFALSE)" >& tmpfile   
      cat tmpfile >> $OUT/test_g4_g4_nat.out
      root.exe -b -q "test_E03_4.C(\"g4Config1.C\", kFALSE)" >& tmpfile   
      cat tmpfile >> $OUT/test_g4_g4_nat.out

      echo "... Running test with G4, geometry via TGeo, Native navigation, User physics list" 
      root.exe -b -q "test_E03_1.C(\"g4Config2.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat_pl.out   
      root.exe -b -q "test_E03_2.C(\"g4Config2.C\", kFALSE)" >& tmpfile      
      cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
      root.exe -b -q "test_E03_3.C(\"g4Config2.C\", kFALSE)" >& tmpfile    
      cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
      root.exe -b -q "test_E03_4.C(\"g4Config2.C\", kFALSE)" >& tmpfile    
      cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
      rm tmpfile
    fi 
  fi   

  if [ "$EXAMPLE" = "A01" ]; then 
    if [ "$TESTG3" = "1" ]; then
      echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g3tgeoConfig.C\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out   
    fi  

    if [ "$TESTG4" = "1" ]; then
      echo "... Running test with G4, geometry via TGeo, Native navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4Config.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   
 
      echo "... Running test with G4, geometry via TGeo, TGeo navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4tgeoConfig.C\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   

      echo "... Running test with G4, geometry via G4, Native navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4Config1.C\", kFALSE)" >& $OUT/test_g4_g4_nat.out   
    fi  
  fi  

  echo " "
done  
  
cd $CURDIR
