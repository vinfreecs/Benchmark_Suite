#pragma once
#include "mmio.hpp"
#include "vec.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>

class csr {
public:
  int rows, cols, nnz;
  VecND values;
  VecNI col_idx;
  VecNI row_start;
};

inline void sort_perm(int *arr, int *perm, int len, bool rev = false) {
  if (rev == false) {
    std::stable_sort(perm + 0, perm + len, [&](const int &a, const int &b) {
      return (arr[a] < arr[b]);
    });
  } else {
    std::stable_sort(perm + 0, perm + len, [&](const int &a, const int &b) {
      return (arr[a] > arr[b]);
    });
  }
}

void read_matrix(const char *file, csr &sm) {
  const char *filename = file;
  double *values_unsorted_;
  int *cols_unsorted_;
  int *rows_unsorted_;
  int rows_, cols_, nnz_;
  MM_typecode matcode;
  FILE *f;

  if ((f = fopen(filename, "r")) == NULL) {
    printf("Unable to open file");
  }

  if (mm_read_banner(f, &matcode) != 0) {
    printf("mm_read_unsymetric: Could not process Matrix Market banner ");
    printf(" in file [%s]\n", filename);
    // return -1;
  }

  fclose(f);
  bool symm_flag = mm_is_symmetric(matcode);
  // this is in coo format change to csr
  // assuming the vakues returen are unsorted
  if (mm_read_unsymmetric_sparse(filename, &rows_, &cols_, &nnz_,
                                 &values_unsorted_, &rows_unsorted_,
                                 &cols_unsorted_) < 0) {
    std::cerr << "There is a problem ! \n";
    return;
  }

  if (rows_ != cols_) {
    printf("Matrix not square. Currently only square matrices are supported\n");
    exit(1);
  }

  // If matrix market file is symmetric; create a general one out of it
  if (symm_flag) {
    // printf("Creating a general matrix out of a symmetric one\n");

    int ctr = 0;

    // this is needed since diagonals might be missing in some cases
    // and in symmetric only half of the data is stored so to combat that we
    // have to do this
    for (int idx = 0; idx < nnz_; ++idx) {
      ++ctr;
      if (rows_unsorted_[idx] != cols_unsorted_[idx]) {
        ++ctr;
      }
    }

    int new_nnz = ctr;

    int *row_general = new int[new_nnz];
    int *col_general = new int[new_nnz];
    double *val_general = new double[new_nnz];

    int idx_gen = 0;

    for (int idx = 0; idx < nnz_; ++idx) {
      row_general[idx_gen] = rows_unsorted_[idx];
      col_general[idx_gen] = cols_unsorted_[idx];
      val_general[idx_gen] = values_unsorted_[idx];
      ++idx_gen;

      if (rows_unsorted_[idx] != cols_unsorted_[idx]) {
        row_general[idx_gen] = cols_unsorted_[idx];
        col_general[idx_gen] = rows_unsorted_[idx];
        val_general[idx_gen] = values_unsorted_[idx];
        ++idx_gen;
      }
    }

    free(rows_unsorted_);
    free(cols_unsorted_);
    free(values_unsorted_);

    nnz_ = new_nnz;

    // assign right pointers for further proccesing
    rows_unsorted_ = row_general;
    cols_unsorted_ = col_general;
    values_unsorted_ = val_general;

    // as already assigned to unsorted maybe can be deleted verify TODO
    // delete[] row_general;
    // delete[] col_general;
    // delete[] val_general;
  }

  // permute the col and val according to row
  int *perm = new int[nnz_];

  // // pramga omp parallel for?
  for (int idx = 0; idx < nnz_; ++idx) {
    perm[idx] = idx;
  }

  sort_perm(rows_unsorted_, perm, nnz_);

  int *col = new int[nnz_];
  int *row = new int[nnz_];
  double *val = new double[nnz_];

#pragma omp parallel for schedule(static)
  for (int idx = 0; idx < nnz_; ++idx) {
    col[idx] = cols_unsorted_[perm[idx]];
    val[idx] = values_unsorted_[perm[idx]];
    row[idx] = rows_unsorted_[perm[idx]];
  }

  delete[] perm;
  delete[] cols_unsorted_;
  delete[] values_unsorted_;
  delete[] rows_unsorted_;

  sm.rows = rows_;
  sm.cols = cols_;
  sm.nnz = nnz_;
  sm.col_idx.resize(nnz_);
  sm.values.resize(nnz_);
  sm.row_start.resize(rows_ + 1);
#pragma omp parallel for schedule(static)
  for (int i = 0; i < nnz_; i++) {
    sm.values[i] = val[i];
    sm.col_idx[i] = col[i];
  }
#pragma omp parallel for schedule(static)
  for (int i = 0; i < rows_; i++) {
    sm.row_start[i] = 0;
  }
  for (int i = 0; i < nnz_; i++) {
    sm.row_start[row[i] + 1]++;
  }
  for (int i = 0; i < rows_; i++) {
    sm.row_start[i + 1] += sm.row_start[i];
  }

  delete[] val;
  delete[] row;
  delete[] col;
}

void calculate_b_c(csr &mat) {
  double B_c = (12 * mat.nnz + 20 * mat.rows + 8 * mat.cols) / (2 * mat.nnz);
  std::cout << "code balance : " << B_c << " \n";
  double nnc = mat.nnz / mat.cols;
  double alpha = 1 / nnc;
  std::cout << "alpha for this parse matrix is : " << alpha << std::endl;
}

// a simple y += S*x  a simple sparse matrix multiplication
// void spmv(VecND &lhs, csr &smat, VecND &rhs) {
// #pragma omp parallel for schedule(static)
//   for (int i = 0; i < smat.rows; i++) {
//     int start = smat.row_start[i];
//     int end = smat.row_start[i + 1];
//     for (int j = start; j < end; j++) {
//       lhs[i] += smat.values[j] * rhs[smat.col_idx[j]];
//     }
//   }
// }

void spmv(VecND &lhs, csr &smat, VecND &rhs) {
#pragma omp parallel for schedule(static)
  for (int i = 0; i < smat.rows; i++) {
    double sum = 0.0;
    int start = smat.row_start[i];
    int end = smat.row_start[i + 1];
#pragma omp simd
    for (int j = start; j < end; j++) {
      sum += smat.values[j] * rhs[smat.col_idx[j]];
    }
    lhs[i] = sum;
  }
}