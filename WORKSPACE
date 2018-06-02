workspace(name = "PowerEFI_SmartFusion_Programmer")

new_git_repository(
    name = "googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.0",
    build_file = "src/gmock.BUILD",
)

git_repository(
    name = "google_styleguide",
    remote = "https://github.com/google/styleguide.git",
)

new_git_repository(
    name = "com_github_nanopb_nanopb",
    remote = "https://github.com/nanopb/nanopb.git",
    commit = "2b0db997d7e85b2cc4ae4ee5654e63f6426b18a1",
    build_file = "src/nanopb.BUILD",
)

# proto_library, cc_proto_library, and java_proto_library rules implicitly
# depend on @com_google_protobuf for protoc and proto runtimes.
# This statement defines the @com_google_protobuf repo.
http_archive(
    name = "com_google_protobuf",
    sha256 = "cef7f1b5a7c5fba672bec2a319246e8feba471f04dcebfe362d55930ee7c1c30",
    strip_prefix = "protobuf-3.5.0",
    urls = ["https://github.com/google/protobuf/archive/v3.5.0.zip"],
)