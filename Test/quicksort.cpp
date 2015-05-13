#include "quicksort.h"

int partition(int *idx, double *compares, int p, int r)
{
  int mid = (p+r)/2;
  int x = compares[mid];
  int t;

  t = compares[p]; compares[p] = compares[mid]; compares[mid] = t;
  t = idx[p]; idx[p] = idx[mid]; idx[mid] = t;

  int k = p;
  int l = r+1;

  do k++; while ((compares[k] <= x) && (k < r));
  do l--; while (compares[l] > x);

  while (k < l) {
    t = compares[k]; compares[k] = compares[l]; compares[l] = t;
    t = idx[k]; idx[k] = idx[l]; idx[l] = t;
    do k++; while (compares[k] <= x);
    do l--; while (compares[l] > x);
  }
  t = compares[p]; compares[p] = compares[l]; compares[l] = t;
  t = idx[p]; idx[p] = idx[l]; idx[l] = t;
  return l;
}


void _quicksort(int *idx, double *compares, int p, int r) {
    if (p < r) {
        int q = partition(idx, compares, p, r);

        #pragma omp task untied
        _quicksort(idx, compares, p, q-1);

        #pragma omp task untied
        _quicksort(idx, compares, q+1, r);
	}
}

void quicksort(int *idx, double *compares, int len) {
    #pragma omp single
    _quicksort(idx, compares, 0, len-1);
}


















































