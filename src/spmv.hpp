#pragma once
#include "mmio.h"
#include "vec.hpp"
#include <iostream>
#include <iterator>
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
  if (mm_read_unsymmetric_sparse(filename, &rows_, &cols_, &nnz_, &values_,
                                 &row_start_, &col_idx_) < 0) {
    std::cerr << "There is a problem ! \n";
    return;
  }
  sm.rows = rows_;
  sm.cols = cols_;
  sm.nnz = nnz_;
  sm.col_idx.insert(sm.col_idx.end(), col_idx_, col_idx_ + cols_);
  sm.row_start.insert(sm.row_start.end(), row_start_, row_start_ + rows_);
  sm.values.insert(sm.values.end(), values_, values_ + nnz_);

  // int count = 0;
  // for (int i = 0; i < sm.rows; i++) {
  //   int start = sm.row_start[i];
  //   int end = sm.row_start[i + 1];
  //   for (int j = start; j < end; j++) {
  //     if(sm.values[j] == 0){
  //       ++count;
  //       if (count<100) {
  //         std::cout << "Values : " << sm.values[j] << std::endl;
  //         std::cout << "cols : " << sm.col_idx[j] << std::endl;
  //         continue;
  //       }
  //     }
  //   }
  // }
  std::cout << "succesfully read the sparse matrix\n";
  // std::cout<<"The number of zeros : "<<count<<std::endl;
}

void spmv_vector_mult(csr &smat, VecND rhs, VecND lhs) {
  for (int i = 0; i < smat.rows; i++) {
    int start = smat.row_start[i];
    int end = smat.row_start[i + 1];
    for (int j = start; j < end; j++) {
      rhs[i] += smat.values[j] * lhs[smat.col_idx[j]];
    }
  }
}
