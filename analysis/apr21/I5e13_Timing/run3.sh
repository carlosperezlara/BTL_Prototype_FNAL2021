#!/bin/bash

DT=2
for RUN in 44688 44689 44694 44695 44696 44697 44702 44704 44705 44706 44707 44708 44711 44713
#for RUN in 44708
do
    ./a.out ${RUN} ${DT}
    root -b -q walk.C\(${RUN},${DT}\)
    ./a.out ${RUN} ${DT}
    root -b -q deltatime.C\(${RUN},${DT}\)
    ./a.out ${RUN} ${DT}
    root -b -q deltatime.C\(${RUN},${DT}\)
    #root -b -q amplitude.C\(${RUN},${DT}\)
done
