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

# Define path to data files
#
DATAPATH=/work/packages/geant4/data
#export G4ABLADATA=$DATAPATH/G4ABLA3.0
#export G4LEDATA=$DATAPATH/G4EMLOW6.35
#export G4LEVELGAMMADATA=$DATAPATH/PhotonEvaporation3.0
#export G4NEUTRONHPDATA=$DATAPATH/G4NDL4.4
#export G4NEUTRONXSDATA=$DATAPATH/G4NEUTRONXS1.4
#export G4PIIDATA=$DATAPATH/G4PII1.3
#export G4RADIOACTIVEDATA=$DATAPATH/RadioactiveDecay4.0
#export G4REALSURFACEDATA=$DATAPATH/RealSurface1.0
#export G4SAIDXSDATA=$DATAPATH/G4SAIDDATA1.1
export G4LENDDATA=$DATAPATH/ENDF.B-VII.0

# Remove previous log directory
rm -fr $OUTDIR

# The list of available PLs is printed by geant4_vmc when a non-existing
# PL is selected
# Lists in 9.6
for PHYSICS_LIST in FTFP_BERT FTFP_BERT_TRV FTFP_BERT_HP FTFP_INCLXX FTFP_INCLXX_HP FTF_BIC LBE QBBC QGSP_BERT QGSP_BERT_HP QGSP_BIC QGSP_BIC_HP QGSP_FTFP_BERT QGSP_INCLXX QGSP_INCLXX_HP QGS_BIC Shielding ShieldingLEND
#for PHYSICS_LIST in LBE
do
  for EM in _ _EMV _EMX _EMY _EMZ _LIV _PEN
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
    root.exe -q -b load_g4.C test_E03_pl.C\(\"g4ConfigEnv.C\"\) >& $OUTDIR/TMP.out 
    EXIT_STATUS=$?
  
    # Extract warnings
    cat $OUTDIR/TMP.out | grep "DATA" >& $OUTDIR/missingData_$G4PHYSICS_LIST.out
    cat $OUTDIR/TMP.out | grep "Unknown process code" >& $OUTDIR/processCode_$G4PHYSICS_LIST.out
    cat $OUTDIR/TMP.out | grep "Unknown process control" >& $OUTDIR/processControl_$G4PHYSICS_LIST.out
    cat $OUTDIR/TMP.out | grep "G4Exception" >& $OUTDIR/exception_$G4PHYSICS_LIST.out

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
    if [ ! -s $OUTDIR/missingData_$G4PHYSICS_LIST.out  ]; then 
      rm $OUTDIR/missingData_$G4PHYSICS_LIST.out
    fi

    if [ ! -s $OUTDIR/processCode_$G4PHYSICS_LIST.out  ]; then 
      rm $OUTDIR/processCode_$G4PHYSICS_LIST.out
    fi

    if [ ! -s $OUTDIR/processControl_$G4PHYSICS_LIST.out  ]; then 
      rm $OUTDIR/processControl_$G4PHYSICS_LIST.out
    fi

    if [ ! -s $OUTDIR/exception_$G4PHYSICS_LIST.out  ]; then 
      rm $OUTDIR/exception_$G4PHYSICS_LIST.out
    fi
  
    echo " "
  done
done  
  
cd $CURDIR
