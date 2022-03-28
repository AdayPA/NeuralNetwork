#include "net.hpp"
#include "dataset.hpp"

#include <algorithm>
#include <iostream>

int main(void) {

    Dataset data("data/input/xor.txt", "data/input/topology.txt", "data/output/data/", "data/output/log/", "data/output/pictures/", 10000);
    const std::vector<std::vector<double>> samples = {
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 0.0},
        {1.0, 1.0}
    };

    for (const auto &input : samples) {
        std::vector<double> output = data.getOutput(input);
        const double prediction = std::clamp(output[0], 0.0, 1.0);
        std::cout << input[0] << " " << input[1] << " -> " << prediction << "\n";
    }

    return 0;
}
