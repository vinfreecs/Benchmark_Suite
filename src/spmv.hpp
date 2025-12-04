#pragma once
class csr{
  public:
    int rows, cols, nnz;
    double *values;
    int *col_idx;
    int *row_start;
};

void read_matrix(const char *file){
    const char *filename = file;
    csr sm;
    if (mm_read_unsymmetric_sparse(filename, &sm.rows, &sm.cols, &sm.nnz, &sm.values,
                                   &sm.row_start, &sm.col_idx) < 0) {
      std::cerr << "There is a problem ! \n";
    }
    int count = 0;
    for (int i = 0; i < sm.rows; i++) {
      int start = sm.row_start[i];
      int end = sm.row_start[i + 1];
      for (int j = start; j < end; j++) {
        if(sm.values[j] == 0){
          ++count;
          if (count<100) {
            std::cout << "Values : " << sm.values[j] << std::endl;
            std::cout << "cols : " << sm.col_idx[j] << std::endl;
            continue;
          }
        }
      }
    }
    std::cout<<"The number of zeros : "<<count<<std::endl;
    std::cout<<"Rows : "<<sm.rows<<" Cols : "<<sm.cols<<" nnz : "<<sm.nnz<<std::endl;

}
