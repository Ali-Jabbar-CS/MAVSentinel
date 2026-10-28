FROM ubuntu:26.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
build-essential ca-certificates clang-tidy \
clang-format clangd cmake gdb git ninja-build python3 python3-venv \
&& rm -rf /var/lib/apt/lists/*
USER ubuntu
WORKDIR /workspaces/MAVSentinel
