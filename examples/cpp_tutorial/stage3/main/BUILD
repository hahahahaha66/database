load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cpp"],
    hdrs = ["hello-greet.h"],
)

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cpp"],
    deps = [
        ":hello-greet",
        "//lib:hello-time",
    ],
)