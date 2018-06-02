#Compile .proto files for C, using nanopb plug-in
genrule(
  name = "nano_protos",
  srcs = [
    "isp.proto",
    "options.proto",
  ],
  outs = [
    "nanopb_gen/isp.pb.c",
    "nanopb_gen/isp.pb.h",
    "nanopb_gen/options.pb.c",
    "nanopb_gen/options.pb.h",
  ],
  tools = [
    "build_nano_protos.sh",
  ],
  # First parameter is output destination, the rest input .protos
  cmd = "./$(location build_nano_protos.sh) $(@D)/nanopb_gen $(SRCS)",
)

#Build all nanopb output C files and helper functions.
cc_library(
  name = "powerefi_embedded_protoapi",
  srcs = [
    "decoder_callbacks.c",
    "nanopb_gen/options.pb.c",
    "nanopb_gen/isp.pb.c",
  ],
  hdrs = [
    "decoder_callbacks.h",
    "message_ids.h",
    "nanopb_gen/options.pb.h",
    "nanopb_gen/isp.pb.h",
  ],
  defines = [
    "PB_FIELD_16BIT=1", #  Must also match nanopb.BUILD
  ],
  copts = [
    "-Iexternal/com_github_nanopb_nanopb/",
  ],
  visibility = [
  	"//visibility:public",
  ],
  deps = [
    "@com_github_nanopb_nanopb//:nanopb",
  ],
)

#Build 'unit' tests for protoapi
cc_test(
  name = "powerefi_protoapi_tests",
  srcs = [
    "powerefi_embedded_protoapi_tests.cc",
    "decoder_callbacks_tests.cc",
  ],
  copts = [
    "-Iexternal/com_github_nanopb_nanopb/",
    "-Isrc/shared/protos/",
  ],
  deps = [
    "@googletest//:gtest",
    ":powerefi_cc_protoapi",
    ":powerefi_embedded_protoapi",
  ],
)

cc_library(
  name = "powerefi_cc_protoapi",
  hdrs = [
    "message_ids.h",
  ],
  deps = [
    "powerefi_cc_protolib",
  ],
  visibility = [
    "//visibility:public",
  ],
)

cc_proto_library(
  name = "powerefi_cc_protolib",
  deps = ["powerefi_protoapi"],
  #protoc = "@protobuf//:protoc",
  #default_runtime = "@protobuf//:protobuf",
  visibility = [
    "//visibility:public",
  ],
)

proto_library(
  name = "powerefi_protoapi",
  srcs = ["isp.proto"],
  deps = [":powerefi_protoapi_options"],
)

proto_library(
  name = "powerefi_protoapi_options",
  srcs = ["options.proto"],
  deps = ["@com_google_protobuf//:descriptor_proto"],
)