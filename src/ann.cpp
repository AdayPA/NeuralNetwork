#include "net.cpp"
#include "dataset.cpp"

#include <iostream>
#include <thread>




int main(void) {

    Dataset data("../data/input/xor.txt", "../data/input/topology.txt", "../data/output/data/","../data/output/log/","../data/output/pictures/", 1);
    /*
    Dataset data("../data/input/xor.txt", "../data/output/");
    std::vector<unsigned> topology;
    data.getTopology(topology);
    Net myNet(topology);
    int Epoch = 1;
    data.trainNN(Epoch, myNet, topology);
    data.trainNN(Epoch, myNet, topology, "../data/input/topology.txt");
    data.logResults();
*/
    return 0;
}