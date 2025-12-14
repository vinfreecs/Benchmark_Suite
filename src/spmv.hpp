#pragma once
#include "mmio.h"
#include "vec.hpp"
#include <cstddef>
#include <iostream>

class csr {
public:
  int rows, cols, nnz;
  VecND values;
  VecNI col_idx;
  VecNI row_start;
};

void read_matrix(const char *file, csr &sm) {
  // TODO this is a workaaround update to accomadate vector
  const char *filename = file;
  double *values_;
  int *col_idx_;
  int *row_start_;
  int rows_, cols_, nnz_;
  // this is in coo format change to csr
  if (mm_read_unsymmetric_sparse(filename, &rows_, &cols_, &nnz_, &values_,
                                 &row_start_, &col_idx_) < 0) {
    std::cerr << "There is a problem ! \n";
    return;
  }
  // sm.rows = rows_;
  // sm.cols = cols_;
  // sm.nnz = nnz_;
  // sm.col_idx.insert(sm.col_idx.end(), col_idx_, col_idx_ + cols_);
  // sm.row_start.insert(sm.row_start.end(), row_start_, row_start_ + rows_);
  // sm.values.insert(sm.values.end(), values_, values_ + nnz_);

  if (mm_read_unsymmetric_sparse_mord(filename, sm.rows, sm.cols, sm.nnz,
                                      sm.values, sm.row_start, sm.col_idx)) {
    std::cerr << "There is a problem ! \n";
    return;
  }

  if (sm.rows == rows_) {
    std::cout << "rows are correct " << sm.rows << " " << rows_ << " \n";
  }
  if (sm.cols == cols_) {
    std::cout << "cols are correct " << sm.cols << " " << cols_ << " \n";
  }
  if (sm.nnz == nnz_) {
    std::cout << "nnz are correct " << sm.nnz << " " << nnz_ << " \n";
  }

  size_t count = 0;
  bool match = true;
  for (int i = 0; i < sm.nnz; i++) { // Iterate up to NNZ, not Rows
    if (sm.values[i] != values_[i]) {
      std::cout << "Value mismatch at index " << i << "\n";
      match = false;
      break;
    }
    if (sm.row_start[i] != row_start_[i]) {
      std::cout << "Row Index mismatch at index " << i << "\n";
      match = false;
      break;
    }
    if (sm.col_idx[i] != col_idx_[i]) {
      std::cout << "Col Index mismatch at index " << i << "\n";
      match = false;
      break;
    }
    count++;
  }

  if (match) {
    std::cout << "Successfully read matrix. C++ and C implementations match.\n";
    std::cout << "Checked " << count << " non-zeros.\n";
  }

  std::cout << "succesfully read the sparse matrix\n";
  std::cout << "the cpp function is correct\n - count is " << count << "\n";
}

// a simple y += S*x  a simple sparse matrix multiplication
void spmv_vector_mult(csr &smat, VecND &rhs, VecND &lhs) {
#pragma omp parallel for schedule(static)
  for (int i = 0; i < smat.rows; i++) {
    int start = smat.row_start[i];
    int end = smat.row_start[i + 1];
    for (int j = start; j < end; j++) {
      rhs[i] += smat.values[j] * lhs[smat.col_idx[j]];
    }
  }
}
