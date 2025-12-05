#pragma once
#include<iostream>
#include "mmio.h"
class csr{
  public:
    int rows, cols, nnz;
    double *values;
    int *col_idx;
    int *row_start;
};

void read_matrix(const char *file, csr &sm){
    const char *filename = file;
    if (mm_read_unsymmetric_sparse(filename, &sm.rows, &sm.cols, &sm.nnz, &sm.values,
                                   &sm.row_start, &sm.col_idx) < 0) {
      std::cerr << "There is a problem ! \n";
      return;
    }
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
    std::cout<<"succesfully read the sparse matrix\n";
    // std::cout<<"The number of zeros : "<<count<<std::endl;    
}

void spmv_vector_mult(csr &smat, double* rhs, double* lhs ){
  for (int i = 0; i < smat.rows; i++) {
    int start = smat.row_start[i];
    int end = smat.row_start[i + 1];
    for (int j = start; j < end; j++) {
      rhs[i] += smat.values[j]*lhs[smat.col_idx[j]];
    }
  }
}

