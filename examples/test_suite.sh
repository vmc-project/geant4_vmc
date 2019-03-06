#!/bin/bash
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
#
# Usage:
# test_suite.sh [--g3=on|off] [--g4=on|off] [--garfield=on|off] [--examples="E01 E03 ..."]
#               [--builddir=dir] [--help|-h] [--debug] [--cmp-ref]
#
# By I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
OUTDIR=$CURDIR/log

# Temporary files go here
TMPDIR="/tmp/GEANT4_VMC"

# The reference log of the test suite
LOGREFDIR="$CURDIR/log_ref"

PASSED="0"
FAILED="0"

# If that's ==0 means not in test
CURRENT_TEST="0"
CURRENT_LOG=""
CURRENT_TEST_CASE="0"

# Control debug output
DEBUG="0"

# Set 1 to 0 if you want to skip given MC or ExGarfield test
TESTG3="1"
TESTG4="1"
BUILDDIR=""

# Run Garfield optionally
TESTGARFIELD="1"

# Root command with loading g3/g4 libraries
RUNG3="root.exe -b -q load_g3.C"
RUNG4="root.exe -b -q load_g4.C"
RUNG3_E03a="root.exe -b -q load_g3a.C"
RUNG3_E03b="root.exe -b -q load_g3b.C"
RUNG4_E03a="root.exe -b -q load_g4a.C"
RUNG4_E03b="root.exe -b -q load_g4b.C"

# The default list of examples (all)
ALL_EXAMPLES="E01 E02 E03 E06 A01 ExGarfield Gflash Monopole TR"
EXAMPLES="$ALL_EXAMPLES"

STREAM_START_RED="\033[0;31m"
STREAM_START_GREEN="\033[0;32m"
STREAM_END_COLOR="\033[0m"

function start_test()
{
  [[ "$CURRENT_TEST" != "0" ]] && { echo "Currently in test $CURRENT_TEST which needs to be finished first. Check this script and call \"finish_test\" first": exit 1; }
  local user_message="$1"
  CURRENT_TEST=`expr $FAILED + $PASSED + 1`
  CURRENT_LOG=$TMPDIR/test_$CURRENT_TEST.out
  touch $CURRENT_LOG

  if [[ "$DEBUG" == "1" ]]
  then
    echo "#######################"
    echo "---> START test $CURRENT_TEST"
  fi
  echo -n "$user_message"
}

function run_test_case()
{
  if [[ "CURRENT_TEST" == "0" ]]
  then
    echo "ERROR: Not in test. Call \"start_test\" first"
  else
    if [[ "$#" != "1" ]]
    then
      echo "Exactly 1 argument requried for run_test_case(), namely a run string"
    else
      local run_string="$1"
      local log_file_tmp=$TMPDIR/test_${CURRENT_TEST}_tmp.out
      CURRENT_TEST_CASE=`expr $CURRENT_TEST_CASE + 1`
      if [[ "$DEBUG" == "1" ]]
      then
        echo "Test case $CURRENT_TEST_CASE"
        echo "Run command: $run_string"
      fi
      $run_string &> $log_file_tmp
      CURRENT_TEST_STATUS="$?"
      cat $log_file_tmp >> $CURRENT_LOG
    fi
  fi
}

function finish_test()
{
  local log_file="$1"
  local exit_status_string="${STREAM_START_GREEN}passed${STREAM_END_COLOR}"
  if [ "$CURRENT_TEST_STATUS" -ne "0" ]
  then
    FAILED=`expr $FAILED + 1`
    exit_status_string="${STREAM_START_RED}failed${STREAM_END_COLOR}"
  else
    PASSED=`expr $PASSED + 1`
  fi

  echo -e " ... $exit_status_string"

  # Write log if requested
  if [[ "$log_file" != "" && "$CURRENT_LOG" != "" ]]
  then
    cp $CURRENT_LOG $log_file
    if [[ "$DEBUG" == "1" ]]
    then
      echo "Log file written to $log_file"
    fi
  fi
  CURRENT_TEST="0"
  CURRENT_LOG=""
  CURRENT_TEST_CASE="0"
}

# If user has prodced a test run this is compared to what is found in log_ref
function compare_to_ref()
{
  if [[ ! -d "$OUTDIR" || ! -d "$LOGREFDIR" ]]
  then
    echo "Could not find both $OUTDIR and $LOGREFDIR. The latter should exist by default."
  else
    diff -r $OUTDIR $LOGREFDIR
  fi
}

function print_help()
{
  echo "Usage:"
  echo "test_suite.sh [--g3=on|off] [--g4=on|off] [--garfield=on|off] [--examples=\"E01 E03 ...\"] [--builddir=dir] [--help|-h] [--debug] [--cmp-ref]"
}

# Process script arguments
for arg in "${@}"
do
  #echo "got: $arg"
  case $arg in
    "--g3=on"        ) TESTG3="1" ;;
    "--g3=off"       ) TESTG3="0" ;;
    "--g4=on"        ) TESTG4="1" ;;
    "--g4=off"       ) TESTG4="0" ;;
    "--garfield=on"  ) TESTGARFIELD="1" ;;
    "--garfield=off" ) TESTGARFIELD="0" ;;
     --examples=*    ) EXAMPLES=${arg#--examples=} ;;
     --builddir=*    ) BUILDDIR=${arg#--builddir=} ;;
    "--help" | "-h" ) print_help
                      exit 0
                      ;;
    "--debug"        ) DEBUG="1" ;;
    "--cmp-ref"      ) compare_to_ref
                       exit 0
                       ;;
    *                ) echo "Unsupported option $arg chosen."
                       print_help
                       exit 1
                       ;;
  esac
done

# Recreate log directory only if running test for both G3 and G4
if [ "$TESTG3" = "1" -a  "$TESTG4" = "1" -a "$EXAMPLES" = "$ALL_EXAMPLES" ]; then
  rm -fr $OUTDIR
fi

# Set path to shared libraries if --builddir is provided via the option
if [ "x${BUILDDIR}" != "x" ]; then
  LIBS_FROM_BUILDDIR=$(find ${BUILDDIR} -iname "*.so" -exec dirname {} \; | tr '\r\n' ':')
  LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LIBS_FROM_BUILDDIR}
fi

# Create tmp dir from scratch
rm -rf $TMPDIR &> /dev/null
mkdir $TMPDIR


for EXAMPLE in $EXAMPLES
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
      start_test "... Running test with G3, geometry via TGeo, TGeo navigation"
      run_test_case "$RUNG3 test_$EXAMPLE.C(\"g3tgeoConfig.C\",kFALSE)"
      # stack popper test
      if [ "$EXAMPLE" = "E06" ]; then
        #echo "... Running test with G3, geometry via TGeo, TGeo navigation, with Feedback photons"
        run_test_case "$RUNG3 test_E06_2.C(\"g3tgeoConfig.C\",kFALSE)"
      fi
      finish_test "$OUT/test_g3_tgeo_tgeo.out"

      start_test "... Running test with G3, geometry via VMC,  Native navigation"
      run_test_case "$RUNG3 test_$EXAMPLE.C(\"g3Config.C\",kTRUE)"
      finish_test "$OUT/test_g3_vmc_nat.out"

      start_test "... Running test with G3, geometry via VMC,  TGeo navigation"
      run_test_case "$RUNG3 test_$EXAMPLE.C(\"g3tgeoConfig.C\",kTRUE)"
      finish_test "$OUT/test_g3_vmc_tgeo.out"
    fi

    if [ "$TESTG4" = "1" ]; then
      start_test "... Running test with G4, geometry via TGeo, Native navigation"
      run_test_case "$RUNG4 test_$EXAMPLE.C(\"g4Config.C\",kFALSE)"
      # stack popper test
      if [ "$EXAMPLE" = "E06" ]; then
        #echo "... Running test with G4, geometry via TGeo, Native navigation, with stackPopper"
        run_test_case "$RUNG4 test_E06_2.C(\"g4Config1.C\",kFALSE)"
      fi
      finish_test "$OUT/test_g4_tgeo_nat.out"

      start_test "... Running test with G4, geometry via TGeo, TGeo navigation"
      run_test_case "$RUNG4 test_$EXAMPLE.C(\"g4tgeoConfig.C\",kFALSE)"
      # stack popper test
      if [ "$EXAMPLE" = "E06" ]; then
        #echo "... Running test with G4, geometry via TGeo, TGeo navigation, with stackPopper"
        run_test_case "$RUNG4 test_E06_2.C(\"g4tgeoConfig1.C\",kFALSE)"
      fi
      finish_test "$OUT/test_g4_tgeo_tgeo.out"

      start_test "... Running test with G4, geometry via VMC, Native navigation"
      run_test_case "$RUNG4 test_$EXAMPLE.C(\"g4ConfigOld.C\",kTRUE)"
      finish_test "$OUT/test_g4_vmc_nat.out"

      start_test "... Running test with G4, geometry via VMC, TGeo navigation"
      #echo $RUNG4 "test_$EXAMPLE.C(\"g4tgeoConfigOld.C\",kTRUE)" >& $OUT/test_g4_vmc_tgeo.out
      run_test_case "$RUNG4 test_$EXAMPLE.C(\"g4tgeoConfigOld.C\",kTRUE)"
      finish_test "$OUT/test_g4_vmc_tgeo.out"
    fi
  fi

  if [ "$EXAMPLE" = "E03" ]; then

    for OPTION in E03a E03b
    do

      OUT_SUB=$OUT/$OPTION
      if [ ! -d $OUT_SUB ]; then
        mkdir -p $OUT_SUB
      fi

      #cd $CURDIR/$EXAMPLE
      echo "... Example $EXAMPLE/$OPTION"

      if [ "$OPTION" = "E03a" ]; then
        RUNG3_OPT=$RUNG3_E03a
        RUNG4_OPT=$RUNG4_E03a
      fi
      if [ "$OPTION" = "E03b" ]; then
        RUNG3_OPT=$RUNG3_E03b
        RUNG4_OPT=$RUNG4_E03b
      fi

      # Run three macro + special configuration available only in E03 example
      if [ "$TESTG3" = "1" ]; then
        start_test "... Running test with G3, geometry via TGeo, TGeo navigation"
        run_test_case "$RUNG3_OPT test_E03_1.C(\"g3tgeoConfig.C\",kFALSE)"
        run_test_case "$RUNG3_OPT test_E03_2.C(\"g3tgeoConfig.C\",kFALSE)"
        run_test_case "$RUNG3_OPT test_E03_3.C(\"g3tgeoConfig.C\",kFALSE)"
        run_test_case "$RUNG3_OPT test_E03_4.C(\"g3tgeoConfig.C\",kFALSE)"
        run_test_case "$RUNG3_OPT test_E03_5.C(\"g3tgeoConfig.C\",kFALSE)"
        finish_test "$OUT_SUB/test_g3_tgeo_tgeo.out"

        start_test "... Running test with G3, geometry via VMC, Native navigation"
        run_test_case "$RUNG3_OPT test_E03_1.C(\"g3Config.C\",kTRUE)"
        run_test_case "$RUNG3_OPT test_E03_2.C(\"g3Config.C\",kTRUE)"
        run_test_case "$RUNG3_OPT test_E03_3.C(\"g3Config.C\",kTRUE)"
        run_test_case "$RUNG3_OPT test_E03_4.C(\"g3Config.C\",kTRUE)"
        run_test_case "$RUNG3_OPT test_E03_5.C(\"g3Config.C\",kTRUE)"
        finish_test "$OUT_SUB/test_g3_vmc_nat.out"

        start_test "... Running test with G3, geometry via VMC, TGeo navigation"
        run_test_case "$RUNG3_OPT test_E03_1.C(\"g3tgeoConfig.C\",kTRUE)"
        run_test_case "$RUNG3_OPT test_E03_2.C(\"g3tgeoConfig.C\",kTRUE)"
        run_test_case "$RUNG3_OPT test_E03_3.C(\"g3tgeoConfig.C\",kTRUE)"
        run_test_case "$RUNG3_OPT test_E03_4.C(\"g3tgeoConfig.C\",kTRUE)"
        run_test_case "$RUNG3_OPT test_E03_5.C(\"g3tgeoConfig.C\",kTRUE)"
        finish_test "$OUT_SUB/test_g3_vmc_tgeo.out"
      fi

      if [ "$TESTG4" = "1" ]; then
        start_test "... Running test with G4, geometry via TGeo, Native navigation"
        run_test_case "$RUNG4_OPT test_E03_1.C(\"g4Config.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_2.C(\"g4Config.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_3.C(\"g4Config.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_4.C(\"g4Config.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_5.C(\"g4Config4.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_6.C(\"g4Config5.C\",kFALSE)"
        finish_test "$OUT_SUB/test_g4_tgeo_nat.out"

        start_test "... Running test with G4, geometry via TGeo, TGeo navigation"
        run_test_case "$RUNG4_OPT test_E03_1.C(\"g4tgeoConfig.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_2.C(\"g4tgeoConfig.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_3.C(\"g4tgeoConfig.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_4.C(\"g4tgeoConfig.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_5.C(\"g4tgeoConfig4.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_6.C(\"g4tgeoConfig5.C\",kFALSE)"
        finish_test "$OUT_SUB/test_g4_tgeo_tgeo.out"

        start_test "... Running test with G4, geometry via TGeo, TGeo navigation"
        run_test_case "$RUNG4_OPT test_E03_1.C(\"g4ConfigOld.C\",kTRUE)"
        run_test_case "$RUNG4_OPT test_E03_2.C(\"g4ConfigOld.C\",kTRUE)"
        run_test_case "$RUNG4_OPT test_E03_3.C(\"g4ConfigOld.C\",kTRUE)"
        run_test_case "$RUNG4_OPT test_E03_4.C(\"g4ConfigOld.C\",kTRUE)"
        finish_test "$OUT_SUB/test_g4_vmc_nat.out"

        start_test "... Running test with G4, geometry via VMC, TGeo navigation"
        run_test_case "$RUNG4_OPT test_E03_1.C(\"g4tgeoConfigOld.C\",kTRUE)"
        run_test_case "$RUNG4_OPT test_E03_2.C(\"g4tgeoConfigOld.C\",kTRUE)"
        run_test_case "$RUNG4_OPT test_E03_3.C(\"g4tgeoConfigOld.C\",kTRUE)"
        run_test_case "$RUNG4_OPT test_E03_4.C(\"g4tgeoConfigOld.C\",kTRUE)"
        finish_test "$OUT_SUB/test_g4_vmc_tgeo.out"

        start_test "... Running test with G4, geometry via G4, Native navigation"
        run_test_case "$RUNG4_OPT test_E03_1.C(\"g4Config1.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_2.C(\"g4Config1.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_3.C(\"g4Config1.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_4.C(\"g4Config1.C\",kFALSE)"
        finish_test "$OUT_SUB/test_g4_g4_nat.out"

        start_test "... Running test with G4, geometry via TGeo, Native navigation, User physics list"
        run_test_case "$RUNG4_OPT test_E03_1.C(\"g4Config2.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_2.C(\"g4Config2.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_3.C(\"g4Config2.C\",kFALSE)"
        run_test_case "$RUNG4_OPT test_E03_4.C(\"g4Config2.C\",kFALSE)"
        finish_test "$OUT_SUB/test_g4_tgeo_nat_pl.out"
      fi
    done
  fi

  if [ "$EXAMPLE" = "A01" ]; then
    if [ "$TESTG3" = "1" ]; then
      start_test "... Running test with G3, geometry via TGeo, TGeo navigation"
      run_test_case "$RUNG3 test_A01_1.C(\"g3tgeoConfig.C\",kFALSE)"
      finish_test "$OUT/test_g3_tgeo_tgeo.out"
    fi

    if [ "$TESTG4" = "1" ]; then
      start_test "... Running test with G4, geometry via TGeo, Native navigation"
      run_test_case "$RUNG4 test_A01_1.C(\"g4Config.C\",kFALSE)"
      run_test_case "$RUNG4 test_A01_2.C(\"g4Config2.C\",kFALSE)"
      finish_test "$OUT/test_g4_tgeo_nat.out"

      start_test "... Running test with G4, geometry via TGeo, TGeo navigation"
      run_test_case "$RUNG4 test_A01_1.C(\"g4tgeoConfig.C\",kFALSE)"
      run_test_case "$RUNG4 test_A01_2.C(\"g4tgeoConfig2.C\",kFALSE)"
      finish_test "$OUT/test_g4_tgeo_tgeo.out"

      start_test "... Running test with G4, geometry via G4, Native navigation"
      run_test_case "$RUNG4 test_A01_1.C(\"g4Config1.C\",kFALSE)"
      run_test_case "$RUNG4 test_A01_2.C(\"g4Config1.C\",kFALSE)"
      finish_test "$OUT/test_g4_g4_nat.out"
    fi
  fi

  # new examples (without old geometry definition)
  #
  if [ "$EXAMPLE" = "ExGarfield" -o "$EXAMPLE" = "Gflash"  -o "$EXAMPLE" = "Monopole" -o "$EXAMPLE" = "TR" ]; then

    if [ "$TESTG3" = "1" -a "$EXAMPLE" != "Monopole" ]; then
      start_test "... Running test with G3, geometry via TGeo, TGeo navigation"
      run_test_case "$RUNG3 test_$EXAMPLE.C(\"g3tgeoConfig.C\")"
      finish_test "$OUT/test_g3_tgeo_tgeo.out"
    fi

    if [ "$TESTG4" = "1" ]; then
      start_test "... Running test with G4, geometry via TGeo, Native navigation"
      run_test_case "$RUNG4 test_$EXAMPLE.C(\"g4Config.C\")"
      finish_test "$OUT/test_g4_tgeo_nat.out"

      start_test "... Running test with G4, geometry via TGeo, TGeo navigation"
      run_test_case "$RUNG4 test_$EXAMPLE.C(\"g4tgeoConfig.C\")"
      finish_test "$OUT/test_g4_tgeo_tgeo.out"
    fi
  fi

  echo " "
done

# remove tmp dir
rm -rf $TMPDIR &> /dev/null

echo " "
echo "Test summary"
echo " "
# Print summary message
if [ "$FAILED" -eq "0" -a  "$PASSED" -ne "0" ]; then
  echo -e "... ${STREAM_START_GREEN}All ($PASSED) tests passed successfully.${STREAM_END_COLOR}"
elif [ "$FAILED" -ne "0" -a  "$PASSED" -eq "0" ]; then
  echo -e "... ${STREAM_START_RED}All ($FAILED) tests failed.${STREAM_END_COLOR}"
else
  echo -e "... ${STREAM_START_GREEN}$PASSED tests passed successfully.${STREAM_END_COLOR}"
  echo -e "... ${STREAM_START_RED}$FAILED tests failed.${STREAM_END_COLOR}"
fi
echo " "

cd $CURDIR

# Clean-up generated files
rm -f */Example*.root
rm -f */gphysi.dat

exit $FAILED
