#!/bin/sh 
# $Id$

#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2007, 2008 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

#
# Run tests for all VMC examples and regenerate output files
#
# by I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
OUTDIR=$CURDIR/log 

rm -fr $OUTDIR
mkdir $OUTDIR

for EXAMPLE in E01 E02 E03 E06
do
  mkdir $OUTDIR/$EXAMPLE
  OUT=$OUTDIR/$EXAMPLE
  
  cd $CURDIR/$EXAMPLE

  echo "... Example $EXAMPLE"
  
  if [ "$EXAMPLE" != "E03" ]; then 
    echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
    root.exe -q "test_$EXAMPLE.C(\"g3tgeoConfig.C\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out   

    echo "... Running test with G3, geometry via VMC,  Native navigation" 
    root.exe -q "test_$EXAMPLE.C(\"g3Config.C\", kTRUE)" >& $OUT/test_g3_vmc_nat.out   

    echo "... Running test with G3, geometry via VMC,  TGeo navigation" 
    root.exe -q "test_$EXAMPLE.C(\"g3tgeoConfig.C\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out   

    echo "... Running test with G4, geometry via TGeo, Native navigation" 
    root.exe -q "test_$EXAMPLE.C(\"g4Config.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   

    echo "... Running test with G4, geometry via TGeo, TGeo navigation" 
    root.exe -q "test_$EXAMPLE.C(\"g4tgeoConfig.C\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   

    echo "... Running test with G4, geometry via VMC,  Native navigation" 
    root.exe -q "test_$EXAMPLE.C(\"g4ConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_nat.out   

    echo "... Running test with G4, geometry via VMC,  TGeo navigation" 
    root.exe -q "test_$EXAMPLE.C(\"g4tgeoConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_tgeo.out   

    echo "... Running test with Fluka, geometry via TGeo" 
    root.exe -q "test_$EXAMPLE.C(\"flConfig.C\", kFALSE)" >& $OUT/test_fl_tgeo.out   

    echo "... Running test with Fluka, geometry via VMC" 
    root.exe -q "test_$EXAMPLE.C(\"flConfig.C\", kTRUE)" >& $OUT/test_fl_vmc.out   

  else
    # Run three macro + special configuration available only in E03 example
    echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
    root.exe -q "test_E03_1.C(\"g3tgeoConfig.C\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out   
    root.exe -q "test_E03_2.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
    cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
    root.exe -q "test_E03_3.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
    cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
    root.exe -q "test_E03_4.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
    cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out

    echo "... Running test with G3, geometry via VMC,  Native navigation" 
    root.exe -q "test_E03_1.C(\"g3Config.C\", kTRUE)" >& $OUT/test_g3_vmc_nat.out   
    root.exe -q "test_E03_2.C(\"g3Config.C\", kTRUE)" >& tmpfile   
    cat tmpfile >> $OUT/test_g3_vmc_nat.out
    root.exe -q "test_E03_3.C(\"g3Config.C\", kTRUE)" >& tmpfile   
    cat tmpfile >> $OUT/test_g3_vmc_nat.out
    root.exe -q "test_E03_4.C(\"g3Config.C\", kTRUE)" >& tmpfile   
    cat tmpfile >> $OUT/test_g3_vmc_nat.out

    echo "... Running test with G3, geometry via VMC,  TGeo navigation" 
    root.exe -q "test_E03_1.C(\"g3tgeoConfig.C\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out   
    root.exe -q "test_E03_2.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile  
    cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
    root.exe -q "test_E03_3.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile  
    cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
    root.exe -q "test_E03_4.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile  
    cat tmpfile >> $OUT/test_g3_vmc_tgeo.out

    echo "... Running test with G4, geometry via TGeo, Native navigation" 
    root.exe -q "test_E03_1.C(\"g4Config.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   
    root.exe -q "test_E03_2.C(\"g4Config.C\", kFALSE)" >& tmpfile  
    cat tmpfile >> $OUT/test_g4_tgeo_nat.out
    root.exe -q "test_E03_3.C(\"g4Config.C\", kFALSE)" >& tmpfile  
    cat tmpfile >> $OUT/test_g4_tgeo_nat.out
    root.exe -q "test_E03_4.C(\"g4Config.C\", kFALSE)" >& tmpfile  
    cat tmpfile >> $OUT/test_g4_tgeo_nat.out

    echo "... Running test with G4, geometry via TGeo, TGeo navigation" 
    root.exe -q "test_E03_1.C(\"g4tgeoConfig.C\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   
    root.exe -q "test_E03_2.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile   
    cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
    root.exe -q "test_E03_3.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile   
    cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
    root.exe -q "test_E03_4.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile   
    cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out

    echo "... Running test with G4, geometry via VMC,  Native navigation" 
    root.exe -q "test_E03_1.C(\"g4ConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_nat.out   
    root.exe -q "test_E03_2.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile  
    cat tmpfile >> $OUT/test_g4_vmc_nat.out
    root.exe -q "test_E03_3.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile  
    cat tmpfile >> $OUT/test_g4_vmc_nat.out
    root.exe -q "test_E03_4.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile  
    cat tmpfile >> $OUT/test_g4_vmc_nat.out

    echo "... Running test with G4, geometry via VMC,  TGeo navigation" 
    root.exe -q "test_E03_1.C(\"g4tgeoConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_tgeo.out   
    root.exe -q "test_E03_2.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile 
    cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
    root.exe -q "test_E03_3.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile 
    cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
    root.exe -q "test_E03_4.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile 
    cat tmpfile >> $OUT/test_g4_vmc_tgeo.out

    echo "... Running test with G4, geometry via G4,   Native navigation" 
    root.exe -q "test_E03_1.C(\"g4Config1.C\", kFALSE)" >& $OUT/test_g4_g4_nat.out   
    root.exe -q "test_E03_2.C(\"g4Config1.C\", kFALSE)" >& tmpfile   
    cat tmpfile >> $OUT/test_g4_g4_nat.out
    root.exe -q "test_E03_3.C(\"g4Config1.C\", kFALSE)" >& tmpfile   
    cat tmpfile >> $OUT/test_g4_g4_nat.out
    root.exe -q "test_E03_4.C(\"g4Config1.C\", kFALSE)" >& tmpfile   
    cat tmpfile >> $OUT/test_g4_g4_nat.out

    echo "... Running test with G4, geometry via TGeo, Native navigation, User physics list" 
    root.exe -q "test_E03_1.C(\"g4Config2.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat_pl.out   
    root.exe -q "test_E03_2.C(\"g4Config2.C\", kFALSE)" >& tmpfile    
    cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
    root.exe -q "test_E03_3.C(\"g4Config2.C\", kFALSE)" >& tmpfile    
    cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
    root.exe -q "test_E03_4.C(\"g4Config2.C\", kFALSE)" >& tmpfile    
    cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
    rm tmpfile

    echo "... Running test with Fluka, geometry via TGeo" 
    root.exe -q "test_E03_1.C(\"flConfig.C\", kFALSE)" >& $OUT/test_fl_tgeo.out   
    root.exe -q "test_E03_2.C(\"flConfig.C\", kFALSE)" >& tmpfile
    cat tmpfile >> $OUT/test_fl_tgeo.out

    echo "... Running test with Fluka, geometry via VMC" 
    root.exe -q "test_E03_1.C(\"flConfig.C\", kTRUE)" >& $OUT/test_fl_vmc.out   
    root.exe -q "test_E03_2.C(\"flConfig.C\", kTRUE)" >& tmpfile   
    cat tmpfile >> $OUT/test_fl_vmc.out

  fi  

  echo " "
done  
  
cd $CURDIR
