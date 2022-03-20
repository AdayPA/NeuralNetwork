#ifndef DATASET_HPP_
#define DATASET_HPP_

#include <vector>
#include <fstream>
#include <sstream>

class Dataset {
public:
    Dataset(const std::string, const std::string );
    ~Dataset();
    bool isEof(void) { return m_trainingDataFile.eof(); }
    void getTopology(std::vector<unsigned> &topology);
    void writeOutputError(const double);
    void writeOutput(std::string);
    void writeEvolution(std::string, std::vector<double> &);
    void seek(void);
    void draw(void);
    inline void clear(void) { m_trainingDataFile.clear(); }
    // Returns the number of input values read from the file:
    unsigned getNextInputs(std::vector<double> &inputVals);
    unsigned getTargetOutputs(std::vector<double> &targetOutputVals);
    unsigned Count_lines (const std::string);

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