#!/bin/bash

#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2019 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# Process the Geant4 VMC packages source code files with clang-format.
# The script should be run from the top geant4_vmc directory.
#
# To apply clang-format on a single file:
# clang-format -style=file -i path/filename
#
# I. Hrivnacova 17/04/2019
#

if [ ! -d g4root ]; then
  echo "The script should be run from the top geant4_vmc directory."
  exit 1;
fi

for DIR in examples g4root source
do
  echo "... processing directory $DIR"
  for FILE in `find $DIR -iname "*.h" -o -iname "*.icc" -o -iname "*.cxx"`
  do 
    if [[ $FILE != *"Doxy"* ]]; then
      echo "...... processing file $FILE"
      clang-format -style=file -i $FILE
    fi
    #else 
    #  echo "###### skipping file for Doxygen $FILE"
    #fi
  done
done
