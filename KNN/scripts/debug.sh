#!/bin/bash

rm -rf build
export CORES=`nproc`

if [[ $1 = "--intel" ]]; then
  export CXX=icpc
elif [[ $1 = "--clang" ]]; then
  export CXX=clang++
elif [[ $1 = "--llvm" ]]; then
  export CXX=clang++
  export CXX_FLAGS="-stdlib=libc++"
elif [[ $1 = "--gcc" ]]; then
  export CXX=g++
fi

mkdir build && cd build

cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_CXX_FLAGS="$CXX_FLAGS" -DBuildTests=ON
make -j${CORES}
make test
cd ..
