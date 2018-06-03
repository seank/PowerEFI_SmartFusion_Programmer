#!/bin/make

# Helper for eclipseCDT

all: tests

tests:
	bazel build src/shared/protos:powerefi_protoapi_tests

nanopb_g4m:
	bash src/shared/protos/build_nano_protos.sh src/shared/protos/nanopb_gen src/shared/protos/isp.proto src/shared/protos/options.proto
