#ifndef NEURON_HPP_
#define NEURON_HPP_

#include <vector>
#include <cstdlib>

struct Connection {
  double weight;
  double deltaWeight;
};

class Neuron {
  public:
    Neuron(unsigned);

  private:
    inline static double randomWeight(void) { return rand() / double(RAND_MAX); };
    double m_outputVal_;
    std::vector<Connection> m_outputWeights_;
};

#endif