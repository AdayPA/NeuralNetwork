#ifndef DATASET_HPP_
#define DATASET_HPP_

#include <vector>
#include <fstream>
#include <sstream>

#include "net.hpp"
#include "tensor.hpp"

class Dataset {
public:
    Dataset(const std::string, const std::string, const std::string, const std::string, const std::string, int);
    ~Dataset();
    
    std::vector<double> getOutput(std::vector<double> &);

private:
    void getTopology(void);
    void getData(void);
    void trainNN(std::vector<unsigned>&, int, int);
    void drawData(std::string &);
    void drawLogs(void);
    void writeLogs(void);
    unsigned Count_lines (const std::string);
    std::fstream createDataFile(std::vector<unsigned>&, int);
    std::string Get_line (const std::string&, const int&);
    std::vector<std::string> Split (std::string, std::string);

    std::vector<std::vector<unsigned>> topology_;
    std::vector<Tensor> data_;
    std::vector<std::vector<std::string>> logs_;
    std::vector<std::string> delete_files_;
    std::ifstream topologyFile_;
    std::string outputDataNameFile_;
    std::string outputLogFile_;
    std::string outputLogNameFile_;
    std::string topologyNameFile_;
    std::string inputDataNameFile_;
    std::string outputPictures_;
    std::ifstream inputDataFile_;
   
    std::vector<Net> nn_;
};

#endif