cc_library(
  name = "powerefi_lib",
  srcs = [
    "message_ids.h",
  ],
  hdrs = [
    "message_ids.h",
  ],
  visibility=[
  	"//visibility:public",
  ],
)

cc_proto_library(
  name = "powerefi_cc_protoapi",
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
  #proto_source_root = ["protos/"],
  deps = [":powerefi_protoapi_options"],
)

proto_library(
  name = "powerefi_protoapi_options",
  srcs = ["options.proto"],
  deps = ["@com_google_protobuf//:descriptor_proto"],
)