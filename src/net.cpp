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
    }
    m_layers_.back().back().setOutputVal(1.0);
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

void Net::backProp(const std::vector<double> &targetVals) {
  // RMS
  Layer &outputLayer = m_layers_.back();
  m_error_ = 0.0;
  for (unsigned n = 0; n < outputLayer.size() - 1; n++) {
    double delta = targetVals[n] - outputLayer[n].getOutputVal();
    m_error_ += delta * delta;
  }
  m_error_ /= outputLayer.size() - 1;
  m_error_ = sqrt(m_error_);
  m_recentAverageError_ = (m_recentAverageError_ * m_recentAverageSmoothingFactor_ + m_error_) / (m_recentAverageSmoothingFactor_ + 1.0);
  for (unsigned n = 0; n < outputLayer.size() - 1; n++) {
    outputLayer[n].calcOutputGradients(targetVals[n]);
  }
  for (unsigned layerNum = m_layers_.size() - 2; layerNum > 0; layerNum--) {
    Layer &hiddenLayer = m_layers_[layerNum];
    Layer &nextLayer = m_layers_[layerNum + 1];
    for (unsigned n = 0; n < hiddenLayer.size(); n++) {
      hiddenLayer[n].calcHiddenGradients(nextLayer);
    }  
  }
  for (unsigned layerNum = m_layers_.size() - 1; layerNum > 0; layerNum--) {
    Layer &layer = m_layers_[layerNum];
    Layer &prevLayer = m_layers_[layerNum - 1];
    for (unsigned n = 0; n < layer.size() - 1; n++) {
      layer[n].updateInputWeights(prevLayer);
    }
  }
}

void Net::getResults(std::vector<double> &resultVals) const {
  resultVals.clear();
  for (unsigned n = 0; n < m_layers_.back().size() - 1; n++) {
    resultVals.push_back(m_layers_.back()[n].getOutputVal());
  }
}

#endif