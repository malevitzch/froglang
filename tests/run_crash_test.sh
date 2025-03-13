#!/bin/sh
cleanup() {
  rm -f "$exec_name" "${test_path}.o"
}
trap cleanup 0

executable_path=$1
test_path=$2

exec_name="exec"

$executable_path $test_path

exit $?
