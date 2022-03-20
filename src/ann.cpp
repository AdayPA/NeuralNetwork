#include "net.cpp"
#include "neuron.cpp"
#include "dataset.cpp"
#include "pbar.cpp"

#include <chrono>
#include <thread>
#include <process.h>

int main() {
    Dataset data("../data/input/xor.txt", "../data/output/");
    std::vector<unsigned> topology;
    data.getTopology(topology);
    unsigned total_lines = data.Count_lines("../data/input/test.txt");
    Net myNet(topology);
    std::vector<double> inputVals, targetVals, resultVals;
    int trainingPass = 0;
    double total_data = (total_lines - 1) / 2;
    pBar bar;
    int total_training = 0;
    int Epoch = 1;
    double increment =  100 / total_data;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < Epoch; i++) {
        auto epo = std::to_string(i);
        data.writeOutput("Epoch " + epo + "\n"); 
        while (!data.isEof()) {
            ++total_training;
            ++trainingPass;
            auto s = std::to_string(trainingPass);
            data.writeOutput("Data " + s);
            if (data.getNextInputs(inputVals) != topology[0]) {
                break;
            }
            data.writeEvolution(": Inputs:", inputVals);
            myNet.feedForward(inputVals);
            myNet.getResults(resultVals);
            data.writeEvolution("Outputs:", resultVals);
            data.getTargetOutputs(targetVals);
            data.writeEvolution("Targets:", targetVals);
            assert(targetVals.size() == topology.back());
            myNet.backProp(targetVals);
            data.writeOutputError(myNet.getRecentAverageError());
            // bar.update(increment);
            // bar.print();
        }
        trainingPass = 0;
        data.clear();
        data.seek();
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    data.draw();
    data.writeLogs(topology,myNet.getRecentAverageError(),elapsed_seconds.count(),total_training);
    std::cout << std::endl << "Done" << std::endl;
    system("start gnuplot -p ../pictures/gnuplot.txt");
    return 0;
}