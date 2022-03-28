#ifndef NET_HPP_
#define NET_HPP_

#include <vector>

#include "neuron.hpp"

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
    double m_recentAverageError_ = 0.0;
    double m_recentAverageSmoothingFactor_ = 100.0;
};

#endif
