#ifndef NEURON_CPP_
#define NEURON_CPP_

#include "neuron.hpp"

Neuron::Neuron(unsigned numOutputs, unsigned myIndex) {
  m_myIndex = myIndex;
  for (unsigned c = 0; c < numOutputs; c++) {
    m_outputWeights_.push_back(Connection());
    m_outputWeights_.back().weight = randomWeight();
  }
}

void Neuron::feedForward(const Layer &prevLayer) {
  double sum = 0.0;
  for (unsigned n = 0; n < prevLayer.size(); n++) {
    sum += prevLayer[n].getOutputVal() * prevLayer[n].m_outputWeights_[m_myIndex].weight;
  }
  m_outputVal_ = Neuron::transferFunction(sum);
}

double Neuron::transferFunction(double x) {
  // tanh 
  return tanh(x);
}

double Neuron::transferFunctionDerivative(double x) {
  // tanh derivation
  return 1.0 - x * x;
}

#endif