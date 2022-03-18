#ifndef NET_CPP_
#define NET_CPP_

#include "net.hpp"

Net::Net(const std::vector<unsigned> &topology) {
  unsigned numLayers = topology.size();
  for (unsigned layerNum = 0; layerNum < numLayers; layerNum++) {
    m_layers_.push_back(Layer{});
    for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; neuronNum++) {
      m_layers_.back().push_back(Neuron());
    }
  }
}

void Net::feedForward(const std::vector<double> &inputVals) {}
void Net::backProp(const std::vector<double> &targetVals) {}
void Net::getResults(std::vector<double> &resultVals) const {}

#endif