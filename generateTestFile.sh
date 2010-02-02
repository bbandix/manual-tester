#!/bin/bash
CPWD=$PWD
CONF="manualtester.conf"
if (( $# < 2 )); then
    echo
    echo "Usage: ./generateTestFile.sh testResultsDir layoutTestsDir [debug]"
    echo
    exit 1
fi
BUILD="Release"

if (( $# == 3 )); then
    BUILD="Debug"
fi

TESTRESULTS=$1
cd $TESTRESULTS && TESTRESULTS=$PWD && cd $CPWD
LAYOUTTESTS=$2
cd $LAYOUTTESTS && LAYOUTTESTS=$PWD
cd $LAYOUTTESTS && cd ..
RUNTESTS="$PWD/WebKitTools/Scripts/run-webkit-tests"
LAUNCHER="$PWD/WebKitBuild/$BUILD/bin/QtLauncher -graphicssystem raster -style windows"
echo $LAUNCHER
echo $LAYOUTTESTS
echo $TESTRESULTS
echo $RUNTESTS

cd $CPWD
echo $RUNTESTS > $CONF
echo $LAUNCHER >> $CONF
echo $TESTRESULTS >> $CONF 

find $TESTRESULTS -name "*-actual.txt" | while read f
do
    SPATH=`echo $f | sed "s:$TESTRESULTS::g" | sed "s:$(basename $f)::g"`
    find $LAYOUTTESTS -name "*html" | egrep "$SPATH$(basename $f -actual.txt).html" >>$CONF
    find $LAYOUTTESTS -name "*xhtml" | egrep "$SPATH$(basename $f -actual.txt).xhtml" >>$CONF
    find $LAYOUTTESTS -name "*xml" | egrep "$SPATH$(basename $f -actual.txt).xml" >>$CONF
    find $LAYOUTTESTS -name "*svg" | egrep "$SPATH$(basename $f -actual.txt).svg" >>$CONF
done

