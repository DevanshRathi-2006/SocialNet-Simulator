#!/bin/bash

# Script to compile socialnet.cpp with g++
# Outputs executable named 'socialnet'
# Stops and prints errors if compilation fails

g++ -std=c++17 -Wall -Wextra -O2 socialnet.cpp -o socialnet

if [ $? -eq 0 ]; then
    echo "Compilation successful. Run ./socialnet to start."
else
    echo "Compilation failed. Check error messages above."
    exit 1
fi
