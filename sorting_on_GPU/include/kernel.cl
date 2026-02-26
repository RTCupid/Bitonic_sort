#define LOCAL_SIZE 256

inline void compare_and_swap_local(__local int* A, uint i, uint ixj, uint ascending) {
    int ai = A[i];
    int aj = A[ixj];

    int swap = ascending ? (ai > aj) : (ai < aj);
    if (swap) {
        A[i]   = aj;
        A[ixj] = ai;
    }
}

// === Kernel 1: Intra-Block Sorting (Local Memory) ===
__kernel void bitonic_sort_local(__global int* A,  __local int* shared, uint n) {
    uint gid = get_global_id(0);
    uint lid = get_local_id(0);
    uint group_id = get_group_id(0);

    if (gid < n) {
        shared[lid] = A[gid];
    } else {
        shared[lid] = INT_MAX;
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    for (uint k = 2; k <= LOCAL_SIZE; k <<= 1) {
        for (uint j = k >> 1; j > 0; j >>= 1) {
            uint ixj = lid ^ j;

            if (ixj > lid) {
                uint ascending = ((gid & k) == 0);
                compare_and_swap_local(shared, lid, ixj, ascending);
            }
            barrier(CLK_LOCAL_MEM_FENCE); // Synchronization within a group
        }
    }

    if (gid < n) {
        A[gid] = shared[lid];
    }
}

// === Kernel 2: Block Merging (Global Memory) ===
__kernel void bitonic_sort_global(__global int* A, uint k, uint j, uint n) {
    uint gid = get_global_id(0);
    if (gid >= n) return;

    uint ixj = gid ^ j;

    if (ixj > gid && ixj < n) {
        int ai = A[gid];
        int aj = A[ixj];

        uint ascending = ((gid & k) == 0);

        if (ascending ? (ai > aj) : (ai < aj)) {
            A[gid] = aj;
            A[ixj] = ai;
        }
    }
}
