#!/bin/sh
# $Id: $
# -------------------------------------------------------------------------
# Script to produce source and optionally binary distribution of geant4_vmc.
# Called by main Makefile.
#
# According to: 
# $ROOTSYS/build/unix/makedist.sh
# Author: Fons Rademakers, 29/2/2000
#
# Usage: makedist.sh [lib]
#
# By I.Hrivnacova, 7/10/2002

CURDIR=`pwd`

# gmake is called from geant4_vmc/source
cd ../..

MAKELIB=$1
VERSION=`cat geant4_vmc/config/version_number`
MACHINE=`uname`
OSREL=`uname -r`
if [ "$MAKELIB" = "lib" ] ; then  
  TYPE=$MACHINE.$OSREL.
else   
  TYPE=""
fi  
TARFILE=geant4_vmc.$VERSION.$TYPE"tar"

TAR=`which gtar`
dum=`echo $TAR | grep "no gtar"`
stat=$?
if [ "$TAR" = '' ] || [ $stat = 0 ]; then
   TAR="tar cvf"
   rm -f $TARFILE.gz
   EXCLUDE=
else 
   TAR=$TAR" zcvf"
   rm -f $TARFILE.gz
   TARFILE=$TARFILE".gz"
   EXCLUDE="--exclude CVS"
fi

$TAR $TARFILE $EXCLUDE geant4_vmc/README geant4_vmc/config  geant4_vmc/"source" \
   geant4_vmc/examples geant4_vmc/$MAKELIB
cd $CURDIR

exit 0
