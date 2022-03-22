#ifndef DATASET_CPP_
#define DATASET_CPP_

#include <process.h>

#include <iomanip>
#include <ctime>
#include <sstream>
#include <cstring>
#include <cassert>
#include <chrono>

#include "dataset.hpp"
#include "pbar.cpp"
#include "neuron.cpp"

Dataset::Dataset(const std::string input, const std::string folder) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
    auto str = oss.str();
    m_trainingDataFile.open(input.c_str());
    std::string tempError = folder + "data/AverageError_" + str + ".txt";
    std::string tempEvo = folder + "data/Evolution_" + str + ".txt";
    nameOutputFile_ = "AverageError_" + str + ".txt";
    outputFile_ = tempError;
    outfileError.open(tempError.c_str());
    outfileEvolution.open(tempEvo.c_str());
    gnuLogFile_.open("../data/output/pictures/gnulogplot.txt");
    gnuFile_.open("../data/output/pictures/gnuplot.txt");
    inputFile_ = input;
}

Dataset::~Dataset() {
    m_trainingDataFile.close();
    outfileError.close();
    gnuFile_.close();
}

void Dataset::writeOutput(std::string text) {
    outfileEvolution << text;
}

void Dataset::writeEvolution(std::string label, std::vector<double> &v) {
    outfileEvolution << label << " ";
    for (unsigned i = 0; i < v.size(); ++i) {
        outfileEvolution << v[i] << " ";
    }
    outfileEvolution << std::endl;
}

void Dataset::getTopology(std::vector<unsigned> &topology) {
    std::string line;
    std::string label;
    getline(m_trainingDataFile, line);
    std::stringstream ss(line);
    ss >> label;
    if (this->isEof() || label.compare("topology:") != 0) {
        abort();
    }
    while (!ss.eof()) {
        unsigned n;
        ss >> n;
        topology.push_back(n);
    }
    return;
}

std::string Dataset::Get_line(const std::string& filename, const int& line_number) {
  std::ifstream inputfile(filename);
  auto temp(1);
  std::string line;
  while( (!(inputfile.eof())) && (temp < line_number)) {
    std::getline(inputfile, line);
    ++temp;
  } 
  std::getline(inputfile, line);
  return line;
}

void Dataset::addTopology(std::vector<unsigned> &topology, const std::string topology_file) {
    std::string line;
    std::string label;
    std::ifstream temp_topology;
    temp_topology.open(topology_file.c_str());
    getline(temp_topology, line);
    std::stringstream ss(line);
    ss >> label;
    if (this->isEof() || label.compare("topology:") != 0) {
        abort();
    }
    while (!ss.eof()) {
        unsigned n;
        ss >> n;
        topology.push_back(n);
    }
    return;
}

unsigned Dataset::getNextInputs(std::vector<double> &inputVals) {
    inputVals.clear();
    std::string line;
    getline(m_trainingDataFile, line);
    std::stringstream ss(line);
    std::string label;
    ss>> label;
    if (label.compare("in:") == 0) {
        double oneValue;
        while (ss >> oneValue) {
            inputVals.push_back(oneValue);
        }
    }
    return inputVals.size();
}

unsigned Dataset::getTargetOutputs(std::vector<double> &targetOutputVals) {
    targetOutputVals.clear();
    std::string line;
    getline(m_trainingDataFile, line);
    std::stringstream ss(line);
    std::string label;
    ss>> label;
    if (label.compare("out:") == 0) {
        double oneValue;
        while (ss >> oneValue) {
            targetOutputVals.push_back(oneValue);
        }
    }
    return targetOutputVals.size();
}

void Dataset::writeOutputError(const double value) {
    outfileError << value << std::endl;
}

unsigned Dataset::Count_lines (const std::string file) {
    unsigned lines = 0;
    std::ifstream file_to_count(file);
    std::string unused;
    while (std::getline(file_to_count, unused)) {
        ++lines;
    }
    return lines;
}

void Dataset::seek(void) { 
    m_trainingDataFile.seekg(0, std::ios::beg); 
    std::string unused;
    std::getline(m_trainingDataFile, unused);
}

void Dataset::draw(void) {
    int total_lines = Count_lines(outputFile_);
    auto lines = std::to_string(total_lines + (total_lines * 5 / 100));
    gnuFile_ << "set terminal pngcairo enhanced font \"arial,10\" fontscale 1.0 size 1080,500" << std::endl;
    gnuFile_ << "set output '../data/output/pictures/" + nameOutputFile_ + ".png'" << std::endl;
    gnuFile_ << "reset" << std::endl;
    gnuFile_ << "set xrange [0:"+ lines +"]" << std::endl;
    gnuFile_ << "set title \"" + nameOutputFile_ + "\"" << std::endl;
    gnuFile_ << "set xlabel \"X\"" << std::endl;
    gnuFile_ << "set ylabel \"Y\"" << std::endl;
    gnuFile_ << "set grid" << std::endl;
    gnuFile_ << "plot \"" + outputFile_ + "\" title \"\" with line" << std::endl;
}

void Dataset::writeLogs(std::vector<unsigned> &topology, double error, double time, int &training) {
    std::vector<std::string> temp = Split(inputFile_, "/");
    std::vector<std::string> temp2 = Split(temp.back(), ".");
    std::string log_file = "../data/output/logs/" + temp2.front() + ".log";
    const int n = log_file.length() + 1;
    char filename[n];
    std::strcpy(filename, log_file.c_str());
    std::fstream appendFileToWorkWith;
    appendFileToWorkWith.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    // If file does not exist, Create new file
    if (!appendFileToWorkWith )  {
        appendFileToWorkWith.open(filename,  std::fstream::in | std::fstream::out | std::fstream::trunc);
    }     // use existing file
    for (int i = 0; i < topology.size(); i++) {
        appendFileToWorkWith << topology[i] << " ";
    }
    topology.clear();
    appendFileToWorkWith << " | " << error << " | " << training << " | " << time << " | " << nameOutputFile_ <<"\n";
    appendFileToWorkWith.close();
    appendFileToWorkWith.close();
}

void Dataset::trainNN(unsigned epoch, Net &myNet, std::vector<unsigned> &topology, const std::string topology_file) {
    unsigned topology_counter = Count_lines(topology_file);
    std::vector<unsigned> topology_new;
    std::string line;
    std::string label;
    std::ifstream temp_topology;
    temp_topology.open(topology_file.c_str());
    for (int i = 0; i < topology_counter; i++) {
        getline(temp_topology, line);
        std::stringstream ss(line);
        ss >> label;
        if (this->isEof() || label.compare("topology:") != 0) {
            abort();
        }
        while (!ss.eof()) {
            unsigned n;
            ss >> n;
            topology_new.push_back(n);
        }
        trainNN(epoch, myNet, topology_new);
        topology_new.clear();
    }
}

void Dataset::trainNN(unsigned epoch, Net &myNet, std::vector<unsigned> &topology) {
    std::vector<double> inputVals, targetVals, resultVals;
    int trainingPass = 0;
    int total_training = 0;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < epoch; i++) {
        auto epo = std::to_string(i);
        writeOutput("Epoch " + epo + "\n"); 
        while (!isEof()) {
            ++total_training;
            ++trainingPass;
            auto s = std::to_string(trainingPass);
            writeOutput("Data " + s);
            if (getNextInputs(inputVals) != topology[0]) {
                break;
            }
            writeEvolution(": Inputs:", inputVals);
            myNet.feedForward(inputVals);
            myNet.getResults(resultVals);
            writeEvolution("Outputs:", resultVals);
            getTargetOutputs(targetVals);
            writeEvolution("Targets:", targetVals);
            assert(targetVals.size() == topology.back());
            myNet.backProp(targetVals);
            writeOutputError(myNet.getRecentAverageError());
        }
        trainingPass = 0;
        clear();
        seek();
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    draw();
    writeLogs(topology,myNet.getRecentAverageError(),elapsed_seconds.count(),total_training);
    system("start gnuplot -p ../data/output/pictures/gnuplot.txt");
}

void Dataset::logResults(void) {
    std::ifstream temp_topology;
    temp_topology.open("../data/output/logs/xor.log");
    std::string line;
    std::vector<std::string> clear_space;
    std::vector<std::string> temp = Split(inputFile_, "/");
    std::vector<std::string> temp2 = Split(temp.back(), ".");
    std::string log_file = "../data/output/logs/" + temp2.front() + ".dat";
    const int n = log_file.length() + 1;
    char filename[n];
    std::strcpy(filename, log_file.c_str());
    std::fstream appendFileToWorkWith;
    appendFileToWorkWith.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    if (!appendFileToWorkWith )  {
        appendFileToWorkWith.open(filename,  std::fstream::in | std::fstream::out | std::fstream::trunc);
    }     
    for (int i = 0; i < Count_lines("../data/output/logs/xor.log"); i++) {
        getline(temp_topology,line);
        std::vector<std::string> temp_1 = Split(line, "|");
        std::vector<std::string> temp2_1 = Split(temp_1.front(), " ");
        appendFileToWorkWith << temp_1[3] << temp_1[1];
        for (int j = 0; j < temp2_1.size() - 1; j++) {
            appendFileToWorkWith << temp2_1[j] << "-";
        }
        clear_space = Split(temp_1[2], " ");
        appendFileToWorkWith << temp2_1.back() << "-" << clear_space[0]<< "\n";
        clear_space.clear();
    }
    appendFileToWorkWith.close();
    //drawLogs();
    //system("start gnuplot -p ../data/output/pictures/gnulogplot.txt");
}

void Dataset::drawLogs(void) {
    gnuLogFile_ << "set terminal pngcairo enhanced font \"arial,10\" fontscale 1.0 size 1080,500" << std::endl;
    gnuLogFile_ << "set output '../data/output/" + nameOutputFile_ + ".png'" << std::endl;
    gnuLogFile_ << "reset" << std::endl;
    gnuLogFile_ << "set title \"" + nameOutputFile_ + "\"" << std::endl;
    gnuLogFile_ << "set xlabel \"Time\"" << std::endl;
    gnuLogFile_ << "set ylabel \"Error\"" << std::endl;
    gnuLogFile_ << "set grid" << std::endl;
    gnuLogFile_ << "plot 'xor.dat' w point pt 7, '' with labels center offset 3.4,.5 notitle" << std::endl;
}

std::vector<std::string> Dataset::Split (std::string str, std::string delim) {
  /// @brief this func split in 2 the string and store them in vector, 
  //         depending of the char
  std::vector<std::string> tokens;
  size_t prev = 0, pos = 0;
  do {
    pos = str.find(delim, prev);
    if (pos == std::string::npos) pos = str.length();
    std::string token = str.substr(prev, pos-prev);
    if (!token.empty()) tokens.push_back(token);
    prev = pos + delim.length();
  }
  while (pos < str.length() && prev < str.length());
  return tokens;
}

#endif