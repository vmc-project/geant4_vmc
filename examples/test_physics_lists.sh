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
# Test all available Geant4 physics list.
# In the test output, there are log files for the test which failed
# and files with extracted warnings about unknown process names
# or missing data.
#
# By I. Hrivnacova, IPN Orsay

CURDIR=`pwd`
OUTDIR=$CURDIR/log/test_physics_lists

# When running on Mac with SIP enabled, the DYLD_LIBRARY_PATH must be defined
# via another env variable
RUN_ENV=""
if [[ ${ROOT_LD_LIBRARY_PATH} ]]
then
  RUN_ENV="env DYLD_LIBRARY_PATH=${ROOT_LD_LIBRARY_PATH} "
fi

# Define path to optional data files
#
# 11.2
# Get LEND data:
# wget ftp://gdo-nuclear.ucllnl.org/LEND_GND1.3/LEND_GND1.3_ENDF.BVII.1.tar.gz
DATAPATH=/Users/ivana/work/packages/geant4/data
export G4LENDDATA=$DATAPATH/LEND_GND1.3_ENDF.BVII.1
export G4PARTICLEHPDATA=$DATAPATH/G4TENDL1.4

# Remove previous log directory
rm -fr $OUTDIR

# Lists in 11.3
#
for PHYSICS_LIST in FTFP_BERT FTFP_BERT_TRV FTFP_BERT_ATL FTFP_BERT_HP FTFQGSP_BERT FTFP_INCLXX FTFP_INCLXX_HP FTF_BIC LBE QBBC QGSP_BERT QGSP_BERT_HP QGSP_BIC QGSP_BIC_HP QGSP_BIC_AllHP QGSP_FTFP_BERT QGSP_INCLXX QGSP_INCLXX_HP QGS_BIC Shielding ShieldingLEND ShieldingLIQMD ShieldingM NuBeam Shielding_HP ShieldingLIQMD_HP ShieldingM_HP FTFP_BERT_HPT FTFP_INCLXX_HPT QGSP_BERT_HPT QGSP_BIC_HPT QGSP_BIC_AllHPT QGSP_INCLXX_HPT Shielding_HPT ShieldingLIQMD_HPT ShieldingM_HPT
do
  for EM in _EMV _EMX _EMY _EMZ _LIV _PEN __GS __SS _EM0 _WVI __LE
  do

    if [ ! -d $OUTDIR ]; then
      mkdir -p $OUTDIR
    fi

    cd $CURDIR/E03
    G4PHYSICS_LIST=$PHYSICS_LIST
    if [ ! $EM = "_" ]; then
      G4PHYSICS_LIST="$G4PHYSICS_LIST""$EM"
    fi
    export G4PHYSICS_LIST=$G4PHYSICS_LIST

    # Run test
    echo "... Running test with $G4PHYSICS_LIST"
    $RUN_ENV root.exe -q -b load_g4a.C test_E03_pl.C\(\"g4ConfigEnv.C\"\) >& $OUTDIR/TMP.out
    EXIT_STATUS=$?

    # Extract warnings
    cat $OUTDIR/TMP.out | grep "Unknown process code" >& $OUTDIR/processCode_$G4PHYSICS_LIST.out
    cat $OUTDIR/TMP.out | grep "Unknown process control" >& $OUTDIR/processControl_$G4PHYSICS_LIST.out
    cat $OUTDIR/TMP.out | grep "G4Exception" >& $OUTDIR/exception_$G4PHYSICS_LIST.out
    # grep for DATA only if an execption was issued
    if [ -s $OUTDIR/exception_$G4PHYSICS_LIST.out  ]; then
      cat $OUTDIR/TMP.out | grep "DATA" >& $OUTDIR/missingData_$G4PHYSICS_LIST.out
    fi

    # Keep output only if program finished with an error or include G4Exception
    if [ "$EXIT_STATUS" = "0" ]; then
      if [ ! -s $OUTDIR/exception_$G4PHYSICS_LIST.out  ]; then
        #mv $OUTDIR/TMP.out $OUTDIR/normalExit_$G4PHYSICS_LIST.out
        rm $OUTDIR/TMP.out
      else
        mv $OUTDIR/TMP.out $OUTDIR/exception_$G4PHYSICS_LIST.out
      fi
    else
      mv $OUTDIR/TMP.out $OUTDIR/errorExit_$G4PHYSICS_LIST.out
    fi

    # Remove files with zero size
    #
    if [ ! -s $OUTDIR/processCode_$G4PHYSICS_LIST.out  ]; then
      rm $OUTDIR/processCode_$G4PHYSICS_LIST.out
    fi

    if [ ! -s $OUTDIR/processControl_$G4PHYSICS_LIST.out  ]; then
      rm $OUTDIR/processControl_$G4PHYSICS_LIST.out
    fi

    if [ ! -s $OUTDIR/exception_$G4PHYSICS_LIST.out  ]; then
      rm $OUTDIR/exception_$G4PHYSICS_LIST.out
    else
      # check missingData only if an execption was issued
      if [ ! -s $OUTDIR/missingData_$G4PHYSICS_LIST.out  ]; then
        rm $OUTDIR/missingData_$G4PHYSICS_LIST.out
      fi
    fi

    echo " "
  done
done

cd $CURDIR

# Clean-up generated files
rm -f */Example*.root
rm -f */gphysi.dat


