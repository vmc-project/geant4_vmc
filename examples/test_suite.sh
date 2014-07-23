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
# Usage:
# test_suite.sh [--g3=on|off] [--g4=on|off]
#
# By I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
OUTDIR=$CURDIR/log
PASSED="0"
FAILED="0"

# Set 1 to 0 if you want to skip given MC
TESTG3=1
TESTG4=1

# Process script arguments
for arg in "${@}"
do
  #echo "got: $arg"
  case $arg in
    "--g3=on"  ) TESTG3=1 ;;
    "--g3=off" ) TESTG3=0 ;;
    "--g4=on"  ) TESTG4=1 ;;
    "--g4=off" ) TESTG4=0 ;;
    * ) echo "Unsupported option $arg chosen."
        echo "Usage:"
        echo "test_suite.sh [--g3=on|off] [--g4=on|off]"
        exit 1
        ;;
  esac
done

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
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G3, geometry via VMC,  Native navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g3Config.C\", kTRUE)" >& $OUT/test_g3_vmc_nat.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G3, geometry via VMC,  TGeo navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g3tgeoConfig.C\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  

    if [ "$TESTG4" = "1" ]; then
      echo "... Running test with G4, geometry via TGeo, Native navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4Config.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
 
      echo "... Running test with G4, geometry via TGeo, TGeo navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4tgeoConfig.C\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via VMC,  Native navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4ConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_nat.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via VMC,  TGeo navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4tgeoConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_tgeo.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  
  fi  

  if [ "$EXAMPLE" = "E03" ]; then 
    # Run three macro + special configuration available only in E03 example
    if [ "$TESTG3" = "1" ]; then
      echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
      TMP_FAILED="0"
      root.exe -b -q "test_E03_1.C(\"g3tgeoConfig.C\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      root.exe -b -q "test_E03_2.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      root.exe -b -q "test_E03_3.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      root.exe -b -q "test_E03_4.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      root.exe -b -q "test_E03_5.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G3, geometry via VMC,  Native navigation" 
      TMP_FAILED="0"
      root.exe -b -q "test_E03_1.C(\"g3Config.C\", kTRUE)" >& $OUT/test_g3_vmc_nat.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      root.exe -b -q "test_E03_2.C(\"g3Config.C\", kTRUE)" >& tmpfile   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      root.exe -b -q "test_E03_3.C(\"g3Config.C\", kTRUE)" >& tmpfile   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      root.exe -b -q "test_E03_4.C(\"g3Config.C\", kTRUE)" >& tmpfile   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      root.exe -b -q "test_E03_5.C(\"g3Config.C\", kTRUE)" >& tmpfile   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G3, geometry via VMC,  TGeo navigation" 
      TMP_FAILED="0"
      root.exe -b -q "test_E03_1.C(\"g3tgeoConfig.C\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      root.exe -b -q "test_E03_2.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      root.exe -b -q "test_E03_3.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      root.exe -b -q "test_E03_4.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      root.exe -b -q "test_E03_5.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  

    if [ "$TESTG4" = "1" ]; then
      echo "... Running test with G4, geometry via TGeo, Native navigation" 
      TMP_FAILED="0"
      root.exe -b -q "test_E03_1.C(\"g4Config.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      root.exe -b -q "test_E03_2.C(\"g4Config.C\", kFALSE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      root.exe -b -q "test_E03_3.C(\"g4Config.C\", kFALSE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      root.exe -b -q "test_E03_4.C(\"g4Config.C\", kFALSE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      root.exe -b -q "test_E03_5.C(\"g4Config4.C\", kFALSE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via TGeo, TGeo navigation" 
      TMP_FAILED="0"
      root.exe -b -q "test_E03_1.C(\"g4tgeoConfig.C\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      root.exe -b -q "test_E03_2.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      root.exe -b -q "test_E03_3.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      root.exe -b -q "test_E03_4.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      root.exe -b -q "test_E03_5.C(\"g4tgeoConfig4.C\", kFALSE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via VMC,  Native navigation" 
      TMP_FAILED="0"
      root.exe -b -q "test_E03_1.C(\"g4ConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_nat.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      root.exe -b -q "test_E03_2.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_nat.out
      root.exe -b -q "test_E03_3.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_nat.out
      root.exe -b -q "test_E03_4.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile  
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_nat.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via VMC,  TGeo navigation" 
      TMP_FAILED="0"
      root.exe -b -q "test_E03_1.C(\"g4tgeoConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_tgeo.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      root.exe -b -q "test_E03_2.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile 
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
      root.exe -b -q "test_E03_3.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile 
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
      root.exe -b -q "test_E03_4.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile 
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via G4,   Native navigation" 
      TMP_FAILED="0"
      root.exe -b -q "test_E03_1.C(\"g4Config1.C\", kFALSE)" >& $OUT/test_g4_g4_nat.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      root.exe -b -q "test_E03_2.C(\"g4Config1.C\", kFALSE)" >& tmpfile   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_g4_nat.out
      root.exe -b -q "test_E03_3.C(\"g4Config1.C\", kFALSE)" >& tmpfile   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_g4_nat.out
      root.exe -b -q "test_E03_4.C(\"g4Config1.C\", kFALSE)" >& tmpfile   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_g4_nat.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via TGeo, Native navigation, User physics list" 
      TMP_FAILED="0"
      root.exe -b -q "test_E03_1.C(\"g4Config2.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat_pl.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      root.exe -b -q "test_E03_2.C(\"g4Config2.C\", kFALSE)" >& tmpfile      
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
      root.exe -b -q "test_E03_3.C(\"g4Config2.C\", kFALSE)" >& tmpfile    
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
      root.exe -b -q "test_E03_4.C(\"g4Config2.C\", kFALSE)" >& tmpfile    
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
      rm tmpfile
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi 
  fi   

  if [ "$EXAMPLE" = "A01" ]; then 
    if [ "$TESTG3" = "1" ]; then
      echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g3tgeoConfig.C\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  

    if [ "$TESTG4" = "1" ]; then
      echo "... Running test with G4, geometry via TGeo, Native navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4Config.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
 
      echo "... Running test with G4, geometry via TGeo, TGeo navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4tgeoConfig.C\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via G4, Native navigation" 
      root.exe -b -q "test_$EXAMPLE.C(\"g4Config1.C\", kFALSE)" >& $OUT/test_g4_g4_nat.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  
  fi  
  echo " "
done  
  
# Print summary message
if [ "$FAILED" -eq "0" -a  "$PASSED" -ne "0" ]; then
  echo "... All ($PASSED) tests passed successfully."
elif [ "$FAILED" -ne "0" -a  "$PASSED" -eq "0" ]; then
  echo "... All ($FAILED) tests failed."
else
  echo "... $PASSED tests passed successfully."
  echo "... $FAILED tests failed."
fi
echo " "

cd $CURDIR

exit $FAILED
