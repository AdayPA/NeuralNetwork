#ifndef TENSOR_CPP_
#define TENSOR_CPP_

#include <vector>

struct Tensor {
  std::vector<double> input_;
  std::vector<double> output_;
  void clear(void) { input_.clear(); output_.clear(); };
};

#endif