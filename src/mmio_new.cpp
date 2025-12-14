#include "mmio.h"
#include "vec.hpp"
#include <cstdio>
#include <string>

// TODO verify
// TODO a templated version for float too?
int mm_read_unsymmetric_sparse_mord(const char *fname, int &M_, int &N_,
                                    int &nz_, VecND &val_, VecNI &I_,
                                    VecNI &J_) {

  FILE *f;
  MM_typecode matcode;
  int M, N, nz;
  int i;

  if ((f = std::fopen(fname, "r")) == NULL) {
    return -1;
  }

  if (mm_read_banner(f, &matcode) != 0) {
    printf("mm_read_unsymetric: Could not process Matrix Market banner ");
    printf(" in file [%s]\n", fname);
    return -1;
  }
  if (!(mm_is_real(matcode) && mm_is_matrix(matcode) &&
        mm_is_sparse(matcode))) {
    fprintf(stderr, "Sorry, this application does not support ");
    fprintf(stderr, "Market Market type: [%s]\n", mm_typecode_to_str(matcode));
    return -1;
  }

  if (mm_read_mtx_crd_size(f, &M, &N, &nz) != 0) {
    fprintf(stderr,
            "read_unsymmetric_sparse(): could not parse matrix size.\n");
    return -1;
  }

  M_ = M;
  N_ = N;
  nz_ = nz;

  I_.resize(nz);
  J_.resize(nz);
  val_.resize(nz);

  // TODO move or directly modify the give vectors??
  for (i = 0; i < nz; i++) {
    fscanf(f, "%d %d %lg\n", &I_[i], &J_[i], &val_[i]);
    I_[i]--; /* adjust from 1-based to 0-based */
    J_[i]--;
  }
  fclose(f);
  return 0;
}