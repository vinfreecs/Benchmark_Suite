#pragma once

class grid {
public:
  grid(size_t n, size_t m) : rows(n), cols(m) {
    // data.fill(0.0);
    data = new double[rows * cols];
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < rows * cols; i++) {
      data[i] = 0.1;
    }
  }

  ~grid() { delete[] data; }

  double &operator()(size_t i, size_t j) { return data[i * cols + j]; }

  const double &operator()(size_t i, size_t j) const {
    return data[i * cols + j];
  }

  double &operator()(size_t i) { return data[i]; }
  const double &operator()(size_t i) const { return data[i]; }

  size_t getRows() const { return rows; }
  size_t getCols() const { return cols; }

private:
  size_t rows, cols;
  double *data;
};
