#!/bin/bash

if (( $# < 2 )); then
    echo
    echo "Usage: ./generateTestFile.sh testResultsDir layoutTestsDir"
    echo
    exit 1
fi

find $1 -name "*-actual.txt" | egrep -v "*xhtml*" | while read f
do
    find $2 -name "$(basename $f -actual.txt).html" 
done

find $1 -name "*-actual.txt" | egrep "*xhtml*" | while read f
do
    find $2 -name "$(basename $f -actual.txt).xhtml" 
done