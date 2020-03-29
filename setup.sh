#!/bin/bash
exit_status=0
let cores="`grep -c "processor" < /proc/cpuinfo`"
root="$(pwd)"
if [ ! -d "Catch2-2.6.1" ]; then
  git clone --branch v2.6.1 https://github.com/catchorg/Catch2.git Catch2-2.6.1
  if [ "$?" != "0" ]; then
    rm -rf Catch2-2.6.1
    exit_status=1
  fi
fi
if [ ! -d "pybind11-2.4.3" ]; then
  git clone --branch v2.4.3 https://github.com/pybind/pybind11.git pybind11-2.4.3
  if [ "$?" != "0" ]; then
    rm -rf pybind11-2.4.3
    exit_status=1
  fi
fi
if [ ! -d "Python-3.6.7" ]; then
  wget https://www.python.org/ftp/python/3.6.7/Python-3.6.7.tgz --no-check-certificate
  if [ "$?" == "0" ]; then
    gzip -d -c Python-3.6.7.tgz | tar -xf -
    pushd Python-3.6.7
    export CFLAGS="-fPIC"
    ./configure --prefix="$root/Python-3.6.7"
    make -j $cores
    make install
    unset CFLAGS
    popd
  else
    exit_status=1
  fi
  rm -rf Python-3.6.7.tgz
fi
exit $exit_status
