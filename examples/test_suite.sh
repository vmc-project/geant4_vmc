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
# Run tests for all VMC examples and regenerate output files
# Usage:
# test_suite.sh [--g3=on|off] [--g4=on|off][--builddir=dir]
#
# By I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
OUTDIR=$CURDIR/log
PASSED="0"
FAILED="0"

# Set 1 to 0 if you want to skip given MC or ExGarfield test
TESTG3="1"
TESTG4="1"
BUILDDIR=""

# Run Garfield only with Root 5
TESTGARFIELD="1"
ROOT_VERSION=`root-config --version`
if [ ${ROOT_VERSION:0:1} = "6" ]; then
  TESTGARFIELD="0"
fi

# Root command with loading g3/g4 libraries
RUNG3="root.exe -b -q load_g3.C"
RUNG4="root.exe -b -q load_g4.C"
RUNG3a="root.exe -b -q load_g3a.C"
RUNG3b="root.exe -b -q load_g3b.C"
RUNG4a="root.exe -b -q load_g4a.C"
RUNG4b="root.exe -b -q load_g4b.C"

# Process script arguments
for arg in "${@}"
do
  #echo "got: $arg"
  case $arg in
    "--g3=on"  ) TESTG3="1" ;;
    "--g3=off" ) TESTG3="0" ;;
    "--g4=on"  ) TESTG4="1" ;;
    "--g4=off" ) TESTG4="0" ;;
    "--garfield=on"  ) TESTGARFIELD="1" ;;
    "--garfield=off" ) TESTGARFIELD="0" ;;
     --builddir=* ) BUILDDIR=${arg#--builddir=} ;;
    * ) echo "Unsupported option $arg chosen."
        echo "Usage:"
        echo "test_suite.sh [--g3=on|off] [--g4=on|off] [--builddir=dir]"
        exit 1
        ;;
  esac
done

# Recreate log directory only if running test for both G3 and G4
if [ "$TESTG3" = "1" -a  "$TESTG4" = "1" ]; then
  rm -fr $OUTDIR
fi

# Set path to shared libraries if --builddir is provided via the option
if [ "x${BUILDDIR}" != "x" ]; then
  LIBS_FROM_BUILDDIR=$(find ${BUILDDIR} -iname "*.so" -exec dirname {} \; | tr '\r\n' ':')
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LIBS_FROM_BUILDDIR}
fi

for EXAMPLE in E01 E02 E03/E03a E06 A01 ExGarfield Gflash TR
do
  OUT=$OUTDIR/$EXAMPLE
  if [ ! -d $OUT ]; then
    mkdir -p $OUT
  fi

  # skip ExGarfield if switch off
  if [ "$EXAMPLE" = "ExGarfield" -a "$TESTGARFIELD" = "0" ]; then
    continue 1
  fi

  cd $CURDIR/$EXAMPLE

  echo "... Example $EXAMPLE"
  
  if [ "$EXAMPLE" = "E01" -o "$EXAMPLE" = "E02" -o "$EXAMPLE" = "E06" ]; then
  
    if [ "$TESTG3" = "1" ]; then
      echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
      TMP_FAILED="0"
      $RUNG3 "test_$EXAMPLE.C(\"g3tgeoConfig.C\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      # stack popper test
      if [ "$EXAMPLE" = "E06" ]; then
        #echo "... Running test with G3, geometry via TGeo, TGeo navigation, with Feedback photons" 
        $RUNG3 "test_E06_2.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      fi
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G3, geometry via VMC,  Native navigation" 
      $RUNG3 "test_$EXAMPLE.C(\"g3Config.C\", kTRUE)" >& $OUT/test_g3_vmc_nat.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G3, geometry via VMC,  TGeo navigation" 
      $RUNG3 "test_$EXAMPLE.C(\"g3tgeoConfig.C\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  

    if [ "$TESTG4" = "1" ]; then
      echo "... Running test with G4, geometry via TGeo, Native navigation" 
      TMP_FAILED="0"
      $RUNG4 "test_$EXAMPLE.C(\"g4Config.C\",kFALSE)" >& $OUT/test_g4_tgeo_nat.out
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      # stack popper test
      if [ "$EXAMPLE" = "E06" ]; then
        #echo "... Running test with G4, geometry via TGeo, Native navigation, with stackPopper" 
        $RUNG4 "test_E06_2.C(\"g4Config1.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      fi
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via TGeo, TGeo navigation" 
      TMP_FAILED="0"
      $RUNG4 "test_$EXAMPLE.C(\"g4tgeoConfig.C\",kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
      # stack popper test
      if [ "$EXAMPLE" = "E06" ]; then
        #echo "... Running test with G4, geometry via TGeo, TGeo navigation, with stackPopper" 
        $RUNG4 "test_E06_2.C(\"g4tgeoConfig1.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      fi
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via VMC,  Native navigation" 
      $RUNG4 "test_$EXAMPLE.C(\"g4ConfigOld.C\",kTRUE)" >& $OUT/test_g4_vmc_nat.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via VMC,  TGeo navigation" 
      echo $RUNG4 "test_$EXAMPLE.C(\"g4tgeoConfigOld.C\",kTRUE)" >& $OUT/test_g4_vmc_tgeo.out
      $RUNG4 "test_$EXAMPLE.C(\"g4tgeoConfigOld.C\",kTRUE)" >& $OUT/test_g4_vmc_tgeo.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  
  fi  

  if [ "$EXAMPLE" = "E03" ]; then 
    for OPTION in E03a E03b
    do
      OUT=$OUTDIR/$EXAMPLE/$OPTION
      if [ ! -d $OUT ]; then
        mkdir -p $OUT
      fi

      cd $CURDIR/$EXAMPLE
      echo "... Example $EXAMPLE/$OPTION"

      if [ "$OPTION" = "E03a" ]; then
        RUNG3_OPT=$RUNG3a
        RUNG4_OPT=$RUNG4a
      fi
      if [ "$OPTION" = "E03b" ]; then
        RUNG3_OPT=$RUNG3b
        RUNG4_OPT=$RUNG4b
      fi

      # Run three macro + special configuration available only in E03 example
      if [ "$TESTG3" = "1" ]; then
        echo "... Running test with G3, geometry via TGeo, TGeo navigation"
        TMP_FAILED="0"
        $RUNG3_OPT "test_E03_1.C(\"g3tgeoConfig.C\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        $RUNG3_OPT "test_E03_2.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
        $RUNG3_OPT "test_E03_3.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
        $RUNG3_OPT "test_E03_4.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
        $RUNG3_OPT "test_E03_5.C(\"g3tgeoConfig.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
        if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

        echo "... Running test with G3, geometry via VMC,  Native navigation"
        TMP_FAILED="0"
        $RUNG3_OPT "test_E03_1.C(\"g3Config.C\", kTRUE)" >& $OUT/test_g3_vmc_nat.out
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        $RUNG3_OPT "test_E03_2.C(\"g3Config.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_vmc_nat.out
        $RUNG3_OPT "test_E03_3.C(\"g3Config.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_vmc_nat.out
        $RUNG3_OPT "test_E03_4.C(\"g3Config.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_vmc_nat.out
        $RUNG3_OPT "test_E03_5.C(\"g3Config.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_vmc_nat.out
        if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

        echo "... Running test with G3, geometry via VMC,  TGeo navigation"
        TMP_FAILED="0"
        $RUNG3_OPT "test_E03_1.C(\"g3tgeoConfig.C\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        $RUNG3_OPT "test_E03_2.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
        $RUNG3_OPT "test_E03_3.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
        $RUNG3_OPT "test_E03_4.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
        $RUNG3_OPT "test_E03_5.C(\"g3tgeoConfig.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
        if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
      fi

      if [ "$TESTG4" = "1" ]; then
        echo "... Running test with G4, geometry via TGeo, Native navigation"
        TMP_FAILED="0"
        $RUNG4_OPT "test_E03_1.C(\"g4Config.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        $RUNG4_OPT "test_E03_2.C(\"g4Config.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_nat.out
        $RUNG4_OPT "test_E03_3.C(\"g4Config.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_nat.out
        $RUNG4_OPT "test_E03_4.C(\"g4Config.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_nat.out
        $RUNG4_OPT "test_E03_5.C(\"g4Config4.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_nat.out
        $RUNG4_OPT "test_E03_6.C(\"g4Config5.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_nat.out
        if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

        echo "... Running test with G4, geometry via TGeo, TGeo navigation"
        TMP_FAILED="0"
        $RUNG4_OPT "test_E03_1.C(\"g4tgeoConfig.C\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        $RUNG4_OPT "test_E03_2.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
        $RUNG4_OPT "test_E03_3.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
        $RUNG4_OPT "test_E03_4.C(\"g4tgeoConfig.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
        $RUNG4_OPT "test_E03_5.C(\"g4tgeoConfig4.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
        $RUNG4_OPT "test_E03_6.C(\"g4tgeoConfig5.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
        if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

        echo "... Running test with G4, geometry via VMC,  Native navigation"
        TMP_FAILED="0"
        $RUNG4_OPT "test_E03_1.C(\"g4ConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_nat.out
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        $RUNG4_OPT "test_E03_2.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_vmc_nat.out
        $RUNG4_OPT "test_E03_3.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_vmc_nat.out
        $RUNG4_OPT "test_E03_4.C(\"g4ConfigOld.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_vmc_nat.out
        if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

        echo "... Running test with G4, geometry via VMC,  TGeo navigation"
        TMP_FAILED="0"
        $RUNG4_OPT "test_E03_1.C(\"g4tgeoConfigOld.C\", kTRUE)" >& $OUT/test_g4_vmc_tgeo.out
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        $RUNG4_OPT "test_E03_2.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
        $RUNG4_OPT "test_E03_3.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
        $RUNG4_OPT "test_E03_4.C(\"g4tgeoConfigOld.C\", kTRUE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
        if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

        echo "... Running test with G4, geometry via G4,   Native navigation"
        TMP_FAILED="0"
        $RUNG4_OPT "test_E03_1.C(\"g4Config1.C\", kFALSE)" >& $OUT/test_g4_g4_nat.out
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        $RUNG4_OPT "test_E03_2.C(\"g4Config1.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_g4_nat.out
        $RUNG4_OPT "test_E03_3.C(\"g4Config1.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_g4_nat.out
        $RUNG4_OPT "test_E03_4.C(\"g4Config1.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_g4_nat.out
        if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

        echo "... Running test with G4, geometry via TGeo, Native navigation, User physics list"
        TMP_FAILED="0"
        $RUNG4_OPT "test_E03_1.C(\"g4Config2.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat_pl.out
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        $RUNG4_OPT "test_E03_2.C(\"g4Config2.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
        $RUNG4_OPT "test_E03_3.C(\"g4Config2.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
        $RUNG4_OPT "test_E03_4.C(\"g4Config2.C\", kFALSE)" >& tmpfile
        if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
        cat tmpfile >> $OUT/test_g4_tgeo_nat_pl.out
        rm tmpfile
        if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
      fi
    done
  fi   

  if [ "$EXAMPLE" = "A01" ]; then 
    if [ "$TESTG3" = "1" ]; then
      echo "... Running test with G3, geometry via TGeo, TGeo navigation" 
      $RUNG3 "test_A01_1.C(\"g3tgeoConfig.C\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  

    if [ "$TESTG4" = "1" ]; then
      echo "... Running test with G4, geometry via TGeo, Native navigation" 
      TMP_FAILED="0"
      $RUNG4 "test_A01_1.C(\"g4Config.C\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $RUNG4 "test_A01_2.C(\"g4Config2.C\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      rm tmpfile
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
 
      echo "... Running test with G4, geometry via TGeo, TGeo navigation" 
      TMP_FAILED="0"
      $RUNG4 "test_A01_1.C(\"g4tgeoConfig.C\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $RUNG4 "test_A01_2.C(\"g4tgeoConfig2.C\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      rm tmpfile
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via G4, Native navigation" 
      TMP_FAILED="0"
      $RUNG4 "test_A01_1.C(\"g4Config1.C\", kFALSE)" >& $OUT/test_g4_g4_nat.out
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $RUNG4 "test_A01_2.C(\"g4Config1.C\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_g4_nat.out
      rm tmpfile
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  
  fi

  # new examples (without old geometry definition)
  #
  if [ "$EXAMPLE" = "ExGarfield" -o "$EXAMPLE" = "Gflash"  -o "$EXAMPLE" = "TR" ]; then

    if [ "$TESTG3" = "1" ]; then
      echo "... Running test with G3, geometry via TGeo, TGeo navigation"
      TMP_FAILED="0"
      $RUNG3 "test_$EXAMPLE.C(\"g3tgeoConfig.C\")" >& $OUT/test_g3_tgeo_tgeo.out
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi

    if [ "$TESTG4" = "1" ]; then
      echo "... Running test with G4, geometry via TGeo, Native navigation"
      TMP_FAILED="0"
      $RUNG4 "test_$EXAMPLE.C(\"g4Config.C\")" >& $OUT/test_g4_tgeo_nat.out
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via TGeo, TGeo navigation"
      TMP_FAILED="0"
      $RUNG4 "test_$EXAMPLE.C(\"g4tgeoConfig.C\")" >& $OUT/test_g4_tgeo_tgeo.out
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

# Clean-up generated files
rm -f */Example*.root
rm -f */gphysi.dat

exit $FAILED
