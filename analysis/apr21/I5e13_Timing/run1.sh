#!/bin/bash

DT=2
for RUN in 44734 44733 44731 44727 44725 44730 44728 44732
#for RUN in 44731 44727 44725 44730 44728 44732
do
    ./a.out ${RUN} ${DT}
    root -b -q walk.C\(${RUN},${DT}\)
    ./a.out ${RUN} ${DT}
    root -b -q deltatime.C\(${RUN},${DT}\)
    ./a.out ${RUN} ${DT}
    root -b -q deltatime.C\(${RUN},${DT}\)
done
