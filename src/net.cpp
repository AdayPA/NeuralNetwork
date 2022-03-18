#ifndef NET_CPP_
#define NET_CPP_

#include "net.hpp"

Net::Net(const std::vector<unsigned> &topology) {
  unsigned numLayers = topology.size();
  for (unsigned layerNum = 0; layerNum < numLayers; layerNum++) {
    m_layers_.push_back(Layer{});
    unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];
    for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; neuronNum++) {
      m_layers_.back().push_back(Neuron(numOutputs));
      std::cout << "Made a Neuron!" << std::endl;
    }
  }
}

void Net::feedForward(const std::vector<double> &inputVals) {}
void Net::backProp(const std::vector<double> &targetVals) {}
void Net::getResults(std::vector<double> &resultVals) const {}

#endif