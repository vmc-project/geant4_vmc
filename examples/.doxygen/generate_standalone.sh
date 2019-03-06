#!/bin/sh

# Script to generate standalone Doxygen documentation
# for the examples with non unique class names.

CURDIR=`pwd`

BACK_PATH="../../../doc/examples_html"

# generate documentation
# {1} example directory
generate() {
  echo "processing ${1}"
  cd ../${1}
  EXAMPLE_NAME=${PWD##*/}
  echo "Generating Doxyfile for example: ${EXAMPLE_NAME}"
  cat $CURDIR/Doxyfile_standalone | sed s/"EXAMPLE_NAME"/"${EXAMPLE_NAME}"/g | sed sY"BACK_PATH"Y"${BACK_PATH}"Yg > Doxyfile
  doxygen >& $CURDIR/doxygen_${EXAMPLE_NAME}.out
  rm Doxyfile
  cd $CURDIR
}

# process examples with standalone documentation
for DIR in E03/E03a; do
  generate ${DIR} ${BACK_PATH2}
done

cd $CURDIR
