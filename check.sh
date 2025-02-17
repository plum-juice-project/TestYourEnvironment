#!/bin/bash

echo ''
echo 'MPI VERSION:'
if ! mpirun --version; then
    echo "Error: MPI is not installed or not working correctly."
fi

echo ""
echo 'GCC VERSION:'
if ! gcc --version; then
    echo "Error: GCC is not installed or not working correctly."
fi

echo ""
echo 'MAKE VERSION:'
if ! make --version; then
    echo "Error: Make is not installed or not working correctly."
fi

echo ''
echo 'GIT VERSION:'
if ! git --version; then
    echo "Error: Git is not installed or not working correctly."
fi

echo ''
echo "We are running with 4 cores"
echo ''

# Compile test.c with error check
if ! mpicc test.c -o test; then
    echo "Error: Compilation failed. Check test.c for errors."
    exit 1
fi

# Run the compiled program with 4 processes
if ! mpirun -n 4 ./test; then
    echo "Error: MPI program failed to execute."
    exit 1
fi
