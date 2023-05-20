#!/bin/bash

PROJ_NAME=sort-visualizer

cd build/

if [ "$1" == "reset" ]
then
  cmake ..
  make "$PROJ_NAME"_run; src/"$PROJ_NAME"_run
else
  make "$PROJ_NAME"_run; src/"$PROJ_NAME"_run
fi

cd ..
