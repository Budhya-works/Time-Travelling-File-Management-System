#!/bin/bash

# Name of the output executable
OUTPUT="start"

# List of source and header files
SRC_FILES="main.cpp"
HEADER_FILES="hash.hpp fileStructure.hpp heap.hpp tree.hpp"

# Compile command
echo "Compiling project..."

g++ -std=c++17 -Wall -Wextra -O2 $SRC_FILES -o $OUTPUT

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Run the program using: ./start"
else
    echo "Compilation failed!"
    exit 1
fi
