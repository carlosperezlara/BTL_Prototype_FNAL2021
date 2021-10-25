A=${1}
B=${2}

echo "checking difference between ${A} and ${B}"
for X in \
    /include/testbeam.h /include/DRSWAVE.h /include/energy.h /include/timing.h /include/mcp.h \
    /src/testbeam.cxx   /src/DRSWAVE.cxx   /src/energy.cxx   /src/timing.cxx   /src/mcp.cxx
do
    echo "============================="
    echo "file   ${A}/${X}  VS  ${B}/${X}"
    diff ${A}/${X} ${B}/${X}
    echo "============================="
done
