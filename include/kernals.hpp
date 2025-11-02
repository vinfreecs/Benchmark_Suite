#pragma once
#include "grid.hpp"
void axpby(grid *ans, grid *x, grid *y, double a, double b) {
  // for (int i = 0; i < x->getRows(); i++) {
  //   for (int j = 0; j < x->getCols(); j++) {
  //     (*ans)(i,j) = a * (*x)(i,j) + b * (*y)(i,j);
  //   }
  // }

  // this is slightly faster
  size_t rows = x->getRows();
  size_t cols = x->getCols();
  #pragma omp parallel for schedule(static)
  for (int i = 0; i < rows * cols; i++) {
    (*ans)(i) = a * (*x)(i) + b * (*y)(i);
  }
}

void dot(double ans, grid *x, grid *y) {

  // for(int i =0; i<x->getRows();i++){
  //   for(int j =0 ;j<x->getCols();j++){
  //     (*ans)(i,j) = (*x)(i,j) * (*y)(i,j);
  //   }
  // }

  #pragma omp parallel for schedule(static)
  for (int i = 0; i < x->getRows() * x->getCols(); i++) {
    ans += (*x)(i) * (*y)(i);
  }
}