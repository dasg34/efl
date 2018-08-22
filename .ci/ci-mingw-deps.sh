#!/bin/sh

set -e
. .ci/travis.sh
travis_fold efl-mingw efl-mingw
docker exec --env MAKEFLAGS="-j5 -rR" --env EIO_MONITOR_POLL=1 $(cat $HOME/cid) sh -c .ci/ci-mingw-cross-build.sh
travis_endfold efl-mingw
