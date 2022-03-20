#ifndef DATASET_CPP_
#define DATASET_CPP_

#include <iomanip>
#include <ctime>
#include <sstream>

#include "dataset.hpp"

Dataset::Dataset(const std::string input, const std::string folder) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
    auto str = oss.str();
    m_trainingDataFile.open(input.c_str());
    std::string tempError = folder + "AverageError_" + str + ".txt";
    std::string tempEvo = folder + "Evolution_" + str + ".txt";
    nameOutputFile_ = "AverageError_" + str + ".txt";
    outputFile_ = tempError;
    outfileError.open(tempError.c_str());
    outfileEvolution.open(tempEvo.c_str());
    gnuFile_.open("../pictures/gnuplot1.txt");
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
    gnuFile_ << "set output '../pictures/" + nameOutputFile_ + ".png'" << std::endl;
    gnuFile_ << "reset" << std::endl;
    gnuFile_ << "set xrange [0:"+ lines +"]" << std::endl;
    gnuFile_ << "set title \"" + nameOutputFile_ + "\"" << std::endl;
    gnuFile_ << "set xlabel \"X\"" << std::endl;
    gnuFile_ << "set ylabel \"Y\"" << std::endl;
    gnuFile_ << "set grid" << std::endl;
    gnuFile_ << "plot \"" + outputFile_ + "\" title \"\" with points pt 7 " << std::endl;
}

#endif