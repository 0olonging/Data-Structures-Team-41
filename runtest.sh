#!/bin/bash

TEST_DIR="tests"

for test_file in "$TEST_DIR"/*.txt
do
    echo
    echo "========================================"
    echo "Running $(basename "$test_file")"
    echo "========================================"

    echo
    echo "[MAP]"
    ./tree_debug map "$test_file"

    echo
    echo "[RB]"
    ./tree_debug rb "$test_file"

    echo
    echo "[AVL]"
    ./tree_debug avl "$test_file"

    echo
done
