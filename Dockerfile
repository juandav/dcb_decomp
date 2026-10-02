# The build environment of the CI (.github/workflows/build.yaml): Ubuntu
# 24.04 with the MIPS binutils and cpp, Python 3.12 with requirements.txt,
# and the compilers and tools that tools/dl_deps.sh downloads, checked
# against tools/deps.sha256. The tools are x86 Linux binaries.
#
# The image holds no game data: tools/docker.sh runs it with the
# repository, disks/ included, mounted at /dcb.
FROM --platform=linux/amd64 ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive LANG=C.UTF-8

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
	binutils-mipsel-linux-gnu gcc-mipsel-linux-gnu \
	ca-certificates make python3 python3-venv unzip wget \
 && rm -rf /var/lib/apt/lists/*

# the Makefile and the tools run python3, which is the venv's
RUN python3 -m venv /opt/venv
ENV PATH=/opt/venv/bin:$PATH
COPY requirements.txt /opt/dcb/
RUN pip install --no-cache-dir -r /opt/dcb/requirements.txt

# the prebuilt tools go in /opt/dcb/bin, which BIN_DIR points the Makefile
# to, so that the mounted repository needs no bin/
COPY tools/dl_deps.sh tools/deps.sha256 /opt/dcb/tools/
RUN /opt/dcb/tools/dl_deps.sh
ENV BIN_DIR=/opt/dcb/bin

WORKDIR /dcb
