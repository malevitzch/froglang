#!/bin/bash

cleanup() {
  rm -f "$exec_name" "${test_path}_output" "${test_path}.o"
}
trap cleanup EXIT

if [ "$#" -ne "3" ]; then
  echo "Usage: $0 <executable_path> <test_path> <expected_output_path>"
  echo "You provided $# arguments, but 3 are required."
  exit 1
fi

executable_path=$1
test_path=$2
expected_output_path=$3

if [ ! -x "$executable_path" ]; then
  echo "Error: $executable_path is not executable."
  exit 1
fi
if [ ! -f "$test_path" ]; then
  echo "Error: Test file $test_path does not exist."
  exit 1
fi
if [ ! -f "$expected_output_path" ]; then
  echo "Error: Expected output file $expected_output_path does not exist."
  exit 1
fi

exec_name="exec"

$executable_path $test_path

./$exec_name > "${test_path}_output"

if ! diff -w "$expected_output_path" "${test_path}_output"; then
  echo "Test failed: Output differs from expected."
  exit 1
fi
