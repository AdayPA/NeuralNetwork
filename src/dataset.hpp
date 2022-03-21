#ifndef DATASET_HPP_
#define DATASET_HPP_

#include <vector>
#include <fstream>
#include <sstream>

#include "net.hpp"

class Dataset {
public:
    Dataset(const std::string, const std::string );
    ~Dataset();
    bool isEof(void) { return m_trainingDataFile.eof(); }
    void getTopology(std::vector<unsigned> &);
    void addTopology(std::vector<unsigned> &, const std::string);
    void writeOutputError(const double);
    void writeOutput(std::string);
    void writeEvolution(std::string, std::vector<double> &);
    void seek(void);
    void draw(void);
    void trainNN(unsigned, Net &, std::vector<unsigned> &);
    void trainNN(unsigned, Net &, std::vector<unsigned> &, const std::string);
    void writeLogs(std::vector<unsigned> &, double , double , int &);
    inline void clear(void) { m_trainingDataFile.clear(); }
    unsigned getNextInputs(std::vector<double> &);
    unsigned getTargetOutputs(std::vector<double> &);
    unsigned Count_lines (const std::string);
    std::vector<std::string> Split (std::string, std::string);
    std::string Get_line (const std::string&, const int&);

private:
     std::ifstream m_trainingDataFile;
     std::ofstream outfileError;
     std::ofstream outfileEvolution;
     std::ofstream gnuFile_;
     std::string outputFile_;
     std::string inputFile_;
     std::string nameOutputFile_;
};

#endif