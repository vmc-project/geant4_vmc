#!/bin/sh 
# $Id$

#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2007 - 2011 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
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
export G4LEVELGAMMADATA=$DATAPATH/PhotonEvaporation2.0
export G4NEUTRONXSDATA=$DATAPATH/G4NEUTRONXS1.0
export G4LEDATA=$DATAPATH/G4EMLOW6.19
export G4NEUTRONHPDATA=$DATAPATH/G4NDL3.14
export G4RADIOACTIVEDATA=$DATAPATH/RadioactiveDecay3.3
export G4ABLADATA=$DATAPATH/G4ABLA3.0
export G4LENDDATA=$DATAPATH/ENDF.B-VII.0


# Remove previous log directory
rm -fr $OUTDIR

# The list of available PLs is printed by geant4_vmc when a non-existing
# PL is selected
# Lists in 9.5.b01
for PHYSICS_LIST in CHIPS FTFP_BERT FTFP_BERT_EMV FTFP_BERT_EMX FTFP_BERT_TRV FTF_BIC LBE LHEP LHEP_EMV QBBC QGSC_BERT QGSP QGSP_BERT QGSP_BERT_EMV QGSP_BERT_EMX QGSP_BERT_HP QGSP_BERT_NOLEP QGSP_BERT_TRV QGSP_BERT_CHIPS QGSP_BIC QGSP_BIC_EMY QGSP_BIC_HP QGSP_FTFP_BERT QGS_BIC QGSP_INCL_ABLA Shielding ShieldingLEND
do
  if [ ! -d $OUTDIR ]; then
    mkdir -p $OUTDIR
  fi
  
  cd $CURDIR/E03
  export G4PHYSICS_LIST=$PHYSICS_LIST

  # Run test
  echo "... Running test with $PHYSICS_LIST" 
  root.exe -q "test_E03_pl.C(\"g4ConfigEnv.C\")" >& $OUTDIR/TMP.out 
  EXIT_STATUS=$?
  
  # Extract warnings
  cat $OUTDIR/TMP.out | grep "DATA" >& $OUTDIR/missingData_$PHYSICS_LIST.out
  cat $OUTDIR/TMP.out | grep "Unknown process code" >& $OUTDIR/processCode_$PHYSICS_LIST.out
  cat $OUTDIR/TMP.out | grep "Unknown process control" >& $OUTDIR/processControl_$PHYSICS_LIST.out

  # Keep output only if program finished with an error
  if [ "$EXIT_STATUS" = "0" ]; then
    rm $OUTDIR/TMP.out
  else
    mv $OUTDIR/TMP.out $OUTDIR/errorExit_$PHYSICS_LIST.out
  fi   
  
  # Remove files with zero size
  #
  if [ ! -s $OUTDIR/missingData_$PHYSICS_LIST.out  ]; then 
    rm $OUTDIR/missingData_$PHYSICS_LIST.out
  fi

  if [ ! -s $OUTDIR/processCode_$PHYSICS_LIST.out  ]; then 
    rm $OUTDIR/processCode_$PHYSICS_LIST.out
  fi

  if [ ! -s $OUTDIR/processControl_$PHYSICS_LIST.out  ]; then 
    rm $OUTDIR/processControl_$PHYSICS_LIST.out
  fi
  
  echo " "
done  
  
cd $CURDIR
