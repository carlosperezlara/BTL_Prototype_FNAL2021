#!/bin/bash

DT=2
for RUN in 44612 44615 44619 44620 44621 44622 44623 44624 44626 44628
do
    ./a.out ${RUN} ${DT}
    root -b -q walk.C\(${RUN},${DT}\)
    ./a.out ${RUN} ${DT}
    root -b -q deltatime.C\(${RUN},${DT}\)
    ./a.out ${RUN} ${DT}
    root -b -q deltatime.C\(${RUN},${DT}\)
    root -b -q amplitude.C\(${RUN},${DT}\)
done
