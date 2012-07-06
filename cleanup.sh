#!/bin/sh

files="
    CMakeCache.txt
    CMakeFiles
    Makefile
    cmake_install.cmake
    output/*
"

for file in ${files}
do
    echo "Deleting: ${file}"
    rm -rf ${file}
done

