#ifndef NEURON_HPP_
#define NEURON_HPP_

#include <vector>
#include <cstdlib>
#include <cmath>

#include "net.hpp"

class Neuron;

typedef std::vector<Neuron> Layer;

struct Connection {
  double weight;
  double deltaWeight;
};

class Neuron {
  public:
    Neuron(unsigned, unsigned);
    inline void setOutputVal(double val) { m_outputVal_ = val; };
    inline double getOutputVal(void) const { return m_outputVal_; };
    void feedForward(const Layer &prevLayer);
    void calcOutputGradients(double);
    void calcHiddenGradients(const Layer &);
    void updateInputWeights(Layer &);

  private:
    static double transferFunction(double);
    static double transferFunctionDerivative(double);
    inline static double randomWeight(void) { return rand() / double(RAND_MAX); };
    double sumDOW(const Layer &) const;
    double m_outputVal_;
    unsigned m_myIndex_;
    double m_gradient_;
    std::vector<Connection> m_outputWeights_;
};

#endif