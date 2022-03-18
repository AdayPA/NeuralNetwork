#include "net.cpp"
#include "neuron.cpp"

int main() {
  Net myNet(topology);

  std::vector<double> inputVals;
  myNet.feedForward(inputVals);
  
  std::vector<double> targetVals;
  myNet.backProp(targetVals);

  std::vector<double> resultVals;
  myNet.getResults(resultVals);
}