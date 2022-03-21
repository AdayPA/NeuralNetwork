#ifndef NET_HPP_
#define NET_HPP_

#include <vector>
#include <iostream>
#include <cassert>

#include "neuron.cpp"

typedef std::vector<Neuron> Layer;

class Net {
  public:
    Net(const std::vector<unsigned> &);
    void setTopology(const std::vector<unsigned> &);
    void feedForward(const std::vector<double> &);
    void backProp(const std::vector<double> &);
    void getResults(std::vector<double> &) const;
    inline double getRecentAverageError(void) const { return m_recentAverageError_; }
  private:
    std::vector<Layer> m_layers_; // m_layers_[layerNum][neuronNum]
    double m_error_;
    double m_recentAverageError_;
    double m_recentAverageSmoothingFactor_;
};

#endif