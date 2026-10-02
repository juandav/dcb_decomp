#!/bin/sh
# Run a command in the Dockerfile's build environment, with this repository
# (disks/ included) mounted at /dcb; a shell without one:
#
#   tools/docker.sh make VERSION=jp generate
#
# It builds the image first (dcb_decomp, or $DCB_IMAGE), which is quick once
# Docker has it cached, and runs as the calling user, so that what the build
# writes stays the user's. VERSION is passed on when it is set.

set -e

TOP="$(dirname "$(dirname "$(readlink -f -- "$0")")")"
IMAGE="${DCB_IMAGE:-dcb_decomp}"

docker build -q -t "$IMAGE" "$TOP" > /dev/null

[ $# -gt 0 ] || set -- bash
tty=
if [ -t 0 ] && [ -t 1 ]; then
	tty=-it
fi

exec docker run --rm $tty -v "$TOP:/dcb" -w /dcb -u "$(id -u):$(id -g)" \
	-e VERSION "$IMAGE" "$@"
