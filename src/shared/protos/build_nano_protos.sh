#!/bin/bash
# TODO (replace me with a syklark extension or .py script

if [ "$#" -lt 2 ]; then
  echo "Requires at least 2 arguments, destination dir and one .proto file."
  exit -1
fi

i=0
for var in "$@"
do
  if [ $i -eq 0 ]; then
    mkdir -p $var
    DEST_DIR=$var
    let "i++"
    continue
  fi
 
  PROTO_FILE=$(basename $var)
  echo "Building $PROTO_FILE target dir: $DEST_DIR"
  # We don't want intermediate files to step on the C++ proto gens output
  # so we move the files to a new build dir.
  BUILD_DIR=$(dirname "${var}")/nanopb_gen
  mkdir -p $BUILD_DIR
  cp $var $BUILD_DIR
  # Finally build the .proto file.
  protoc --plugin=protoc-gen-nanopb=/usr/local/bin/protoc-gen-nanopb --nanopb_out=. $BUILD_DIR/$PROTO_FILE
  # Use sed to remove include prefix, -I must be used in the Bazel CC rule.
  BASE_NAME=${PROTO_FILE%.*}
  sed 's/src\/shared\/protos\///g' $BUILD_DIR/${BASE_NAME}.pb.h > $DEST_DIR/${BASE_NAME}.pb.h
  # C sources don't need any adjustments, just copy them over.
  cp $BUILD_DIR/${BASE_NAME}.pb.c $DEST_DIR
  # Remove temp files
  rm $BUILD_DIR/$PROTO_FILE
done
