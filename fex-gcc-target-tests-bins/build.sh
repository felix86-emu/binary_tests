#!/bin/bash
echo "Building 64-bit"
for f in *.c
do
  VAR=`grep '/\* { dg-do run } \*/' $f`
  if [ -z "$VAR" ]
  then
    echo "Skiping $f: not a run test"
  else
    echo "Building $f ..."
    # take action on each file. $f store current file name
    NAME=$f
    ARGS=`cat $NAME | sed -n -e "s/\/\* { dg-options \"//p" | sed "s/\".*//"`

    gcc $NAME $ARGS -o $NAME.gcc-target-test-64 &
  fi
done
echo "DONE 64-bit"

echo "Building 32-bit"
for f in *.c
do
  VAR=`grep '/\* { dg-do run } \*/' $f`
  if [ -z "$VAR" ]
  then
    echo "Skiping $f: not a run test"
  else
    echo "Building $f ..."
    # take action on each file. $f store current file name
    NAME=$f
    ARGS=`cat $NAME | sed -n -e "s/\/\* { dg-options \"//p" | sed "s/\".*//"`

    gcc $NAME -m32 $ARGS -o $NAME.gcc-target-test-32 &
  fi
done
echo "DONE 32-bit"
