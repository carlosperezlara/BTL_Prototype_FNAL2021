#!/bin/bash

DT=2
for RUN in 44499 44500 44501 44506 44510 44511 44514 44521 44522
do
    ./a.out ${RUN} ${DT}
    root -b -q walk.C\(${RUN},${DT}\)
    ./a.out ${RUN} ${DT}
    root -b -q deltatime.C\(${RUN},${DT}\)
    ./a.out ${RUN} ${DT}
    root -b -q deltatime.C\(${RUN},${DT}\)
    root -b -q amplitude.C\(${RUN},${DT}\)
done
