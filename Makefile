# Include the nanopb provided Makefile rules
include ./src/shared/third_party/nanopb/extra/nanopb.mk

PROTODIR=src/shared/protos

# Build rule for the protocol
$(PROTODIR)/isp.pb.h: $(PROTODIR)/isp.proto
	$(PROTOC) $(PROTOC_OPTS) --nanopb_out=. $(PROTODIR)/isp.proto

$(PROTODIR)/options.pb.h: $(PROTODIR)/options.proto
		$(PROTOC) $(PROTOC_OPTS) --nanopb_out=. $(PROTODIR)/options.proto

all: $(PROTODIR)/options.pb.h $(PROTODIR)/isp.pb.c
