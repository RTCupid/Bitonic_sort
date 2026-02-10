inline void compare_and_swap(__global int* A, uint i, uint ixj, uint ascending)
{
    int ai = A[i];
    int aj = A[ixj];

    int swap = ascending ? (ai > aj) : (ai < aj);
    if (swap) {
        A[i]   = aj;
        A[ixj] = ai;
    }
}

__kernel void bitonic_stage(__global int* A, uint j, uint k)
{
    uint i = (uint)get_global_id(0);
    uint ixj = i ^ j;

    if (ixj > i) {
        uint ascending = ((i & k) == 0);
        compare_and_swap(A, i, ixj, ascending);
    }
}