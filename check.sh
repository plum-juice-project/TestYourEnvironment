echo ""
echo "MPI VERSION:"
mpirun --version
echo ""
echo "GCC VERSION"
gcc --version
echo ""
echo "MAKE VERSION:"
make --version
echo ""
echo "GIT VERSION:"
git --version

echo ""
echo "Your system has $(nproc) logical cores"
echo ""

mpicc test.c -o test
mpirun -n 1 test
