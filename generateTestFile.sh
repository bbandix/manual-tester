#!/bin/bash

if (( $# < 2 )); then
    echo
    echo "Usage: ./generateTestFile.sh testResultsDir layoutTestsDir"
    echo
    exit 1
fi
rm -f tests.entry
find $1 -name "*-actual.txt" | while read f
do
    SPATH=`echo $f | sed "s:$1::g" | sed "s:$(basename $f)::g"`
    find $2 -name "*html" | egrep "$SPATH`basename $f -actual.txt`.html">>tests.entry
    find $2 -name "*xhtml" | egrep "$SPATH`basename $f -actual.txt`.xhtml">>tests.entry
done

