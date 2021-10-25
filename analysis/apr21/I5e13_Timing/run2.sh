#!/bin/bash

for DT in 1 2 5 10 15 20 30 50 100
#for DT in 1
do
    ./a.out 44734 ${DT}
    root -b -q walk.C\(44734,${DT}\)
    ./a.out 44734 ${DT}
    root -b -q deltatime.C\(44734,${DT}\)
    ./a.out 44734 ${DT}
    root -b -q deltatime.C\(44734,${DT}\)
done
