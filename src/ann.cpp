#include "net.cpp"
#include "neuron.cpp"
#include "dataset.cpp"
#include "pbar.cpp"

#include <chrono>
#include <thread>
#include <process.h>

int main() {
    Dataset data("../data/input/test.txt", "../data/output/");
    // e.g., { 3, 2, 1 }
    std::vector<unsigned> topology;
    data.getTopology(topology);
    unsigned total_lines = data.Count_lines("../data/input/test.txt");
    Net myNet(topology);
    std::vector<double> inputVals, targetVals, resultVals;
    int trainingPass = 0;
    double total_data = (total_lines - 1) / 2;
    pBar bar;
    int Epoch = 3;
    double increment =  100 / total_data;
    for (int i = 0; i < Epoch; i++) {
        auto epo = std::to_string(i);
        data.writeOutput("Epoch " + epo + "\n"); 
        while (!data.isEof()) {
            ++trainingPass;
            auto s = std::to_string(trainingPass);
            data.writeOutput("Data " + s);
            // Get new input data and feed it forward:
            if (data.getNextInputs(inputVals) != topology[0]) {
                break;
            }
            data.writeEvolution(": Inputs:", inputVals);
            myNet.feedForward(inputVals);
            // Collect the net's actual output results:
            myNet.getResults(resultVals);
            data.writeEvolution("Outputs:", resultVals);
            // Train the net what the outputs should have been:
            data.getTargetOutputs(targetVals);
            data.writeEvolution("Targets:", targetVals);
            assert(targetVals.size() == topology.back());
            myNet.backProp(targetVals);
            // Report how well the training is working, average over recent samples:
            data.writeOutputError(myNet.getRecentAverageError());
            // bar.update(increment);
            // bar.print();
        }
        trainingPass = 0;
        data.clear();
        data.seek();
    }
    data.draw();
    std::cout << std::endl << "Done" << std::endl;
    system("start gnuplot -p ../pictures/gnuplot1.txt");
    return 0;
}