#ifndef NET_HPP_
#define NET_HPP_

#include <vector>

class Net {
  public:
    Net(topology);
    void feedForward(const std::vector<double> &inputVals);
    void backProp(const std::vector<double> &targetVals;
    void getResults(std::vector<double> &resultVals) const;
  private:
    
};

#endif