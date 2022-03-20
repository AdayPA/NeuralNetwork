#include <thread>

#include "net.cpp"
#include "neuron.cpp"
#include "dataset.cpp"
#include "pbar.cpp"

int main() {
    Dataset data("../data/input/xor.txt", "../data/output/");
    std::vector<unsigned> topology;
    data.getTopology(topology);
    Net myNet(topology);
    int Epoch = 1;
    data.trainNN(Epoch, myNet, topology);
    return 0;
}