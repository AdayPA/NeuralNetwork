#include "net.cpp"
#include "dataset.cpp"

#include <iostream>
#include <thread>

int main(void) {

    Dataset data("../data/input/xor.txt", "../data/input/topology.txt", "../data/output/data/","../data/output/log/","../data/output/pictures/", 1);
    std::vector<double> input = {0.0, 0.0};
    std::vector<double> output = data.getOutput(input);
    std::cout << output[0];

    return 0;
}