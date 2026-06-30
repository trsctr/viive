#!/bin/bash
CLEAN=""
if [[ "$1" == "--clean" ]]; then
    CLEAN="--clean-first"
fi

cmake --build build --target viive_tests --config Debug $CLEAN
./build/viive_tests_artefacts/Debug/viive_tests.exe
