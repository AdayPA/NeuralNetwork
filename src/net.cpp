#ifndef NET_CPP_
#define NET_CPP_

#include "net.hpp"

Net::Net(const std::vector<unsigned> &topology) {
  unsigned numLayers = topology.size();
  for (unsigned layerNum = 0; layerNum < numLayers; layerNum++) {
    m_layers_.push_back(Layer{});
    unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];
    for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; neuronNum++) {
      m_layers_.back().push_back(Neuron(numOutputs, neuronNum));
      std::cout << "Made a Neuron!" << std::endl;
    }
  }
}

void Net::feedForward(const std::vector<double> &inputVals) {
  assert(inputVals.size() == m_layers_[0].size() - 1);
  for (unsigned i = 0; i < inputVals.size(); i++) {
    m_layers_[0][i].setOutputVal(inputVals[i]);
  }
  for (unsigned layerNum = 1; layerNum < m_layers_.size(); layerNum++) {
    Layer &prevLayer = m_layers_[layerNum - 1];
    for (unsigned n = 0; n < m_layers_[layerNum].size() - 1; n++) {
      m_layers_[layerNum][n].feedForward(prevLayer);
    }
  }
}
void Net::backProp(const std::vector<double> &targetVals) {}
void Net::getResults(std::vector<double> &resultVals) const {}

#endif