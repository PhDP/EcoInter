#!/bin/bash

rm -rf tmp_install && mkdir tmp_install && cd tmp_install
cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DBuildTests=OFF
make
sudo make install

cd .. && rm -rf tmp_install
