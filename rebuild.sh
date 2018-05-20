#!/bin/bash
rm -rf ./build
mkdir ./build
cd ./build
cmake -DCMAKE_BUILD_TYPE=Release ..
make

echo
echo 
echo "Author: Devendra Rai, dr3sg@virginia.edu, OR devenrai@gmail.com"
echo "Implementation of the Solution Provided under the Terms of MIT License."
echo 
echo 
