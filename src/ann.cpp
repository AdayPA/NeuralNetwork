#include "net.cpp"
#include "dataset.cpp"

int main() {

    Dataset data("../data/input/xor.txt", "../data/output/", "../data/input/topology.txt", 2);
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