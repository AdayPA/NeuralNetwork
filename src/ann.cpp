#include "net.cpp"
#include "neuron.cpp"

int main() {

  std::vector<unsigned> topology;   // e.g {3,2,1} -> topology_example.png
  Net myNet(topology);

  std::vector<double> inputVals;
  myNet.feedForward(inputVals);

  std::vector<double> targetVals;
  myNet.backProp(targetVals);

  std::vector<double> resultVals;
  myNet.getResults(resultVals);
}