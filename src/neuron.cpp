#ifndef NEURON_CPP_
#define NEURON_CPP_

#include "neuron.hpp"

Neuron::Neuron(unsigned numOutputs, unsigned myIndex) {
  m_myIndex_ = myIndex;
  for (unsigned c = 0; c < numOutputs; c++) {
    m_outputWeights_.push_back(Connection());
    m_outputWeights_.back().weight = randomWeight();
  }
}

void Neuron::feedForward(const Layer &prevLayer) {
  double sum = 0.0;
  for (unsigned n = 0; n < prevLayer.size(); n++) {
    sum += prevLayer[n].getOutputVal() * prevLayer[n].m_outputWeights_[m_myIndex_].weight;
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

void Neuron::calcOutputGradients(double targetVal) {
  double delta = targetVal - m_outputVal_;
  m_gradient_ = delta * Neuron::transferFunctionDerivative(m_outputVal_);
}

void  Neuron::calcHiddenGradients(const Layer &nextLayer) {
  double dow = sumDOW(nextLayer);
  m_gradient_ = dow * Neuron::transferFunctionDerivative(m_outputVal_);
}

double Neuron::sumDOW(const Layer &nextLayer) const {
  double sum = 0.0;
  for (unsigned n = 0; n < nextLayer.size() - 1; n++) {
    sum += m_outputWeights_[n].weight * nextLayer[n].m_gradient_;
  }
  return sum;
}



#endif