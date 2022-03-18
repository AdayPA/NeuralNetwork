#ifndef NEURON_CPP_
#define NEURON_CPP_

#include "neuron.hpp"

Neuron::Neuron(unsigned numOutputs) {
  for (unsigned c = 0; c < numOutputs; c++) {
    m_outputWeights_.push_back(Connection());
    m_outputWeights_.back().weight = randomWeight();
  }
}


#endif