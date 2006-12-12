#!/bin/sh 
# $Id: test_suite.sh,v 1.4 2005/11/18 21:34:44 brun Exp $
#
# Run tests for all VMC examples and regenerate output files
#
# by I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
  
for EXAMPLE in E01 E02 E03 E06
do
  cd $CURDIR/$EXAMPLE

  echo "... Example $EXAMPLE"
  
  echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
  root.exe -q "test_$EXAMPLE.C(\"g3tgeoConfig.C\", kFALSE)" >& test_g3_tgeo_tgeo.out   

  echo "... Running test with G3, geometry via VMC,  Native navigation" 
  root.exe -q "test_$EXAMPLE.C(\"g3Config.C\", kTRUE)" >& test_g3_vmc_nat.out   

  echo "... Running test with G3, geometry via VMC,  TGeo navigation" 
  root.exe -q "test_$EXAMPLE.C(\"g3tgeoConfig.C\", kTRUE)" >& test_g3_vmc_tgeo.out   

  echo "... Running test with G4, geometry via TGeo, Native navigation" 
  root.exe -q "test_$EXAMPLE.C(\"g4Config.C\", kFALSE)" >& test_g4_tgeo_nat.out   

  echo "... Running test with G4, geometry via TGeo, TGeo navigation" 
  root.exe -q "test_$EXAMPLE.C(\"g4tgeoConfig.C\", kFALSE)" >& test_g4_tgeo_tgeo.out   

  echo "... Running test with G4, geometry via VMC,  Native navigation" 
  root.exe -q "test_$EXAMPLE.C(\"g4ConfigOld.C\", kTRUE)" >& test_g4_vmc_nat.out   

  echo "... Running test with G4, geometry via VMC,  TGeo navigation" 
  root.exe -q "test_$EXAMPLE.C(\"g4tgeoConfigOld.C\", kTRUE)" >& test_g4_vmc_tgeo.out   

  # configuration available only in E03 example
  if [ "$EXAMPLE" = "E03" ]; then 

    echo "... Running test with G4, geometry via G4,   Native navigation" 
    root.exe -q "test_$EXAMPLE.C(\"g4Config1.C\", kFALSE)" >& test_g4_g4_nat.out   

    echo "... Running test with G4, geometry via TGeo, Native navigation, User physics list" 
    root.exe -q "test_$EXAMPLE.C(\"g4Config2.C\", kFALSE)" >& test_g4_tgeo_nat_pl.out   
  fi

  echo " "
done  
  
cd $CURDIR
