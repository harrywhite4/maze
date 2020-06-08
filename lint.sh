#!/usr/bin/env bash
files=$(find ./src -regextype egrep -regex '.*\.(cpp|hpp)')
clang-tidy -p=build ${files}
