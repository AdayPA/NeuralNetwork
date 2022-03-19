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
    inline void clear(void) { m_trainingDataFile.clear(); }
    inline void seek(void) { 
        m_trainingDataFile.seekg(0, std::ios::beg); 
        std::string unused;
        std::getline(m_trainingDataFile, unused);
        
        }
    // Returns the number of input values read from the file:
    unsigned getNextInputs(std::vector<double> &inputVals);
    unsigned getTargetOutputs(std::vector<double> &targetOutputVals);
    unsigned Count_lines (const std::string);

private:
     std::ifstream m_trainingDataFile;
     std::ofstream outfileError;
     std::ofstream outfileEvolution;
};

#endif