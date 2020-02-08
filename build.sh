#!/bin/bash

BUILD=build

if [[ ! -d "$BUILD" ]]
then
    mkdir "$BUILD"
fi

cd "$BUILD"
cmake ..

if [ $? -ne 0 ]
then
	exit 1
fi

make

if [ $? -ne 0 ]
then
	exit 1
fi

./randomx-service -help