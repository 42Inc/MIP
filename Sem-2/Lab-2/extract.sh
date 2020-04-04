#!/bin/bash
MAX_STEPS=10
if [[ $# < 2 ]];then
  exit 1
fi
rm -f $2
for (( i=2; i <= $MAX_STEPS; ++i )); do
  $1 $i 2>> $2
done