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
# Usage:
# test_suite_exe.sh [--g3=on|off] [--g4=on|off] [--builddir=dir]
#
# By I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
OUTDIR=$CURDIR/log_exe
G3EXEDIR=$CURDIR/../../geant3_install/bin
G4EXEDIR=$CURDIR/../../geant4_vmc_install/bin
PASSED="0"
FAILED="0"

# Set 1 to 0 if you want to skip given MC
TESTG3=1
TESTG4=1
BUILDDIR=""

# Process script arguments
for arg in "${@}"
do
  #echo "got: $arg"
  case $arg in
    "--g3=on"  ) TESTG3=1 ;;
    "--g3=off" ) TESTG3=0 ;;
    "--g4=on"  ) TESTG4=1 ;;
    "--g4=off" ) TESTG4=0 ;;
     --builddir=* ) BUILDDIR=${arg#--builddir=} ;;
    * ) echo "Unsupported option $arg chosen."
        echo "Usage:"
        echo "test_suite_exe.sh [--g3=on|off] [--g4=on|off] [--builddir=dir]"
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

for EXAMPLE in E01 E02 E03 E06 A01
do
  OUT=$OUTDIR/$EXAMPLE
  if [ ! -d $OUT ]; then
    mkdir -p $OUT
  fi

  cd $CURDIR/$EXAMPLE

  if [ "x${BUILDDIR}" != "x" ]; then
    G3EXEDIR=${BUILDDIR}/examples/$EXAMPLE
    G4EXEDIR=${BUILDDIR}/examples/$EXAMPLE
  fi

  echo "... Example $EXAMPLE"
  
  if [ "$EXAMPLE" != "E03" -a "$EXAMPLE" != "A01" ]; then 
    if [ "$TESTG3" = "1" ]; then
      EXE=$G3EXEDIR"/g3vmc_test"$EXAMPLE
      echo "... Running test with G3, geometry via TGeo, TGeo navigation"
      $EXE -g3g TGeant3TGeo -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G3, geometry via VMC,  Native navigation"
      $EXE -g3g TGeant3 -rm "test_$EXAMPLE.C(\"\", kTRUE)" >& $OUT/test_g3_vmc_nat.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G3, geometry via VMC,  TGeo navigation"
      $EXE -g3g TGeant3TGeo -rm "test_$EXAMPLE.C(\"\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  
    if [ "$TESTG4" = "1" ]; then
      EXE=$G4EXEDIR"/g4vmc_test"$EXAMPLE
      echo "... Running test with G4, geometry via TGeo, Native navigation"
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via TGeo, TGeo navigation"
      $EXE -g4g geomRoot -g4vm "" -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via VMC,  Native navigation"
      $EXE -g4g geomVMCtoGeant4 -g4vm "" -rm "test_$EXAMPLE.C(\"\", kTRUE)" >& $OUT/test_g4_vmc_nat.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via VMC,  TGeo navigation"
      $EXE -g4g geomVMCtoRoot -g4vm "" -rm "test_$EXAMPLE.C(\"\", kTRUE)" >& $OUT/test_g4_vmc_tgeo.out
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  
  fi  

  if [ "$EXAMPLE" = "E03" ]; then 
    # Run all macros + special configuration available only in E03 test
    if [ "$TESTG3" = "1" ]; then
      EXE=$G3EXEDIR"/g3vmc_test"$EXAMPLE
      echo "... Running test with G3, geometry via TGeo, TGeo navigation vvv"
      TMP_FAILED="0"
      $EXE -g3g TGeant3TGeo -rm "test_E03_1.C(\"\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $EXE -g3g TGeant3TGeo -rm "test_E03_2.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="$?" ; fi
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_3.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_4.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_5.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_tgeo_tgeo.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G3, geometry via VMC,  Native navigation"
      TMP_FAILED="0"
      $EXE -g3g TGeant3 -rm "test_E03_1.C(\"\", kTRUE)" >& $OUT/test_g3_vmc_nat.out
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $EXE -g3g TGeant3 -rm "test_E03_2.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      $EXE -g3g TGeant3 -rm "test_E03_3.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      $EXE -g3g TGeant3 -rm "test_E03_4.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      $EXE -g3g TGeant3 -rm "test_E03_5.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_nat.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G3, geometry via VMC,  TGeo navigation"
      TMP_FAILED="0"
      $EXE -g3g TGeant3TGeo -rm "test_E03_1.C(\"\", kTRUE)" >& $OUT/test_g3_vmc_tgeo.out
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $EXE -g3g TGeant3TGeo -rm "test_E03_2.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_3.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_4.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      $EXE -g3g TGeant3TGeo -rm "test_E03_5.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g3_vmc_tgeo.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  
    # Run all macros + special configuration available only in E03 test
    if [ "$TESTG4" = "1" ]; then
      EXE=$G4EXEDIR"/g4vmc_test"$EXAMPLE
      echo "... Running test with G4, geometry via TGeo, Native navigation"
      TMP_FAILED="0"
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_E03_1.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_E03_2.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_E03_3.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_E03_4.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      $EXE -g4g geomRootToGeant4 -g4sp specialCuts+specialControls -g4vm "" -rm "test_E03_5.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_nat.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via TGeo, TGeo navigation"
      TMP_FAILED="0"
      $EXE -g4g geomRoot -g4vm "" -rm "test_E03_1.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $EXE -g4g geomRoot -g4vm "" -rm "test_E03_2.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      $EXE -g4g geomRoot -g4vm "" -rm "test_E03_3.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      $EXE -g4g geomRoot -g4vm "" -rm "test_E03_4.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      $EXE -g4g geomRoot  -g4sp specialCuts+specialControls -g4vm "" -rm "test_E03_5.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_tgeo_tgeo.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via VMC,  Native navigation"
      TMP_FAILED="0"
      $EXE -g4g geomVMCtoGeant4 -g4vm "" -rm "test_E03_1.C(\"\", kTRUE)" >& $OUT/test_g4_vmc_nat.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $EXE -g4g geomVMCtoGeant4 -g4vm "" -rm "test_E03_2.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_nat.out
      $EXE -g4g geomVMCtoGeant4 -g4vm "" -rm "test_E03_3.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_nat.out
      $EXE -g4g geomVMCtoGeant4 -g4vm "" -rm "test_E03_4.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_nat.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via VMC,  TGeo navigation"
      TMP_FAILED="0"
      $EXE -g4g geomVMCtoRoot -g4vm "" -rm "test_E03_1.C(\"\", kTRUE)" >& $OUT/test_g4_vmc_tgeo.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $EXE -g4g geomVMCtoRoot -g4vm "" -rm "test_E03_2.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
      $EXE -g4g geomVMCtoRoot -g4vm "" -rm "test_E03_3.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
      $EXE -g4g geomVMCtoRoot -g4vm "" -rm "test_E03_4.C(\"\", kTRUE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_vmc_tgeo.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via G4,   Native navigation"
      TMP_FAILED="0"
      $EXE -g4uc geometry -g4vm "" -rm "test_E03_1.C(\"\", kFALSE)" >& $OUT/test_g4_g4_nat.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $EXE -g4uc geometry -g4vm "" -rm "test_E03_2.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_g4_nat.out
      $EXE -g4uc geometry -g4vm "" -rm "test_E03_3.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_g4_nat.out
      $EXE -g4uc geometry -g4vm "" -rm "test_E03_4.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_g4_nat.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4,  geometry via TGeo, Native navigation, User physics list"
      TMP_FAILED="0"
      $EXE -g4uc physics-list -g4vm "" -rm "test_E03_1.C(\"\", kFALSE)" >& $OUT/test_g4_g4_nat_pl.out   
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      $EXE -g4uc physics-list -g4vm "" -rm "test_E03_2.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_g4_nat_pl.out
      $EXE -g4uc physics-list -g4vm "" -rm "test_E03_3.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_g4_nat_pl.out
      $EXE -g4uc physics-list -g4vm "" -rm "test_E03_4.C(\"\", kFALSE)" >& tmpfile
      if [ "$?" -ne "0" ]; then TMP_FAILED="1" ; fi
      cat tmpfile >> $OUT/test_g4_g4_nat_pl.out
      if [ "$TMP_FAILED" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi
  fi    

  if [ "$EXAMPLE" = "A01" ]; then 
    if [ "$TESTG3" = "1" ]; then
      EXE=$G3EXEDIR"/g3vmc_test"$EXAMPLE
      echo "... Running test with G3, geometry via TGeo, TGeo navigation"
      $EXE -g3g TGeant3TGeo -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g3_tgeo_tgeo.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi
    fi  
    if [ "$TESTG4" = "1" ]; then
      EXE=$G4EXEDIR"/g4vmc_test"$EXAMPLE
      echo "... Running test with G4, geometry via TGeo, Native navigation"
      $EXE -g4g geomRootToGeant4 -g4vm "" -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_nat.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via TGeo, TGeo navigation"
      $EXE -g4g geomRoot -g4vm "" -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g4_tgeo_tgeo.out   
      if [ "$?" -ne "0" ]; then FAILED=`expr $FAILED + 1`; else PASSED=`expr $PASSED + 1`; fi

      echo "... Running test with G4, geometry via G4, Native navigation"
      $EXE -g4uc geometry -g4vm "" -rm "test_$EXAMPLE.C(\"\", kFALSE)" >& $OUT/test_g4_g4_nat.out   
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
