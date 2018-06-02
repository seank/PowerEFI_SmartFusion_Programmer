#!/bin/make

# Helper for eclipseCDT

all: tests

tests:
	bazel build src/shared/protos:powerefi_protoapi_tests
	