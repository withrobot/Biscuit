#!/bin/sh

if [ -f builddep.sh ] && [ -f builddep.sh ]
then
./builddep.sh
    if [ -d build ]
    then
	rm -r build
    fi
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr .. && cpack ..
    sudo make install
fi
