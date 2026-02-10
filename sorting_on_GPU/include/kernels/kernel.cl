__kernel void bitonic_sort(
    __global TYPE *A) {
  for (int k = 2; k <= A.size(); k *= 2) {
    for (int j = k/2; j >= 1; j /= 2) {
      for (int i = 0; i < k/2; ++i)
        compare_and_swap(A[i], A[i + j]);
    }
  }
}
