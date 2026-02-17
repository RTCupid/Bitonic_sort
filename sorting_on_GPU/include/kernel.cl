inline void compare_and_swap(__global int* A, uint i, uint ixj, uint ascending) {
    int ai = A[i];
    int aj = A[ixj];

    int swap = ascending ? (ai > aj) : (ai < aj);
    if (swap) {
        A[i]   = aj;
        A[ixj] = ai;
    }
}

inline void compare_and_swap_local(__local int* A, uint i, uint ixj, uint ascending) {
    int ai = A[i];
    int aj = A[ixj];

    int swap = ascending ? (ai > aj) : (ai < aj);
    if (swap) {
        A[i]   = aj;
        A[ixj] = ai;
    }
}

__kernel void bitonic_sort(__global int* A, uint k, uint n,
                            uint use_local_memory /*1 if k <= LOCAL_SIZE*/,
                            uint work_group_size) {
    uint gid = get_global_id(0);
    uint lid = get_local_id(0);
    uint group_id = get_group_id(0);

    uint block_offset = group_id * work_group_size;

    __local int shared[256]; // Must match local_size on the host

    if (use_local_memory) {
        if (gid < n) {
            shared[lid] = A[gid];
        } else {
            shared[lid] = INT_MAX;
        }
        barrier(CLK_LOCAL_MEM_FENCE);

        for (uint j = k >> 1; j > 0; j >>= 1) {
            uint ixj = lid ^ j;

            if (ixj > lid && ixj < work_group_size) {
                uint global_i = block_offset + lid;
                uint ascending = ((global_i & k) == 0);

                compare_and_swap_local(shared, lid, ixj, ascending);
            }
            barrier(CLK_LOCAL_MEM_FENCE); // Synchronization within a group
        }

        if (gid < n) {
            A[gid] = shared[lid];
        }
    } else {
        if (gid >= n) return;

        for (uint j = k >> 1; j > 0; j >>= 1) {
            uint ixj = gid ^ j;

            if (ixj > gid && ixj < n) {
                uint ascending = ((gid & k) == 0);
                compare_and_swap(A, gid, ixj, ascending);
            }
        }
    }
}
