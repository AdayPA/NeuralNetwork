#ifndef DATASET_CPP_
#define DATASET_CPP_

#include <unistd.h>
#include <process.h>

#include <iomanip>
#include <ctime>
#include <sstream>
#include <cstring>
#include <string>
#include <cassert>
#include <chrono>
#include <cstdio>

#include "dataset.hpp"
#include "pbar.cpp"
#include "neuron.cpp"
#include "net.cpp"

Dataset::Dataset (  const std::string input_data, 
                    const std::string input_topology, 
                    const std::string output_data,
                    const std::string output_log,
                    const std::string output_pictures, int Epoch) {
    topologyFile_.open(input_topology.c_str());
    inputDataFile_.open(input_data.c_str());
    topologyNameFile_ = input_topology;  
    inputDataNameFile_ = input_data;
    outputDataNameFile_ = output_data;
    outputPictures_ = output_pictures;
    outputLogFile_ = output_log;
    getTopology();
    getData();
    logs_.resize(topology_.size());
    for (int i = 0; i < topology_.size(); i++) {
        trainNN(topology_[i], Epoch, i);
    }
    writeLogs();
    drawLogs();
}

std::vector<double>  Dataset::getOutput(std::vector<double> & input) {
    std::vector<double> output;
    nn_[0].feedForward(input);
    nn_[0].getResults(output);
    return output;
}

void Dataset::drawLogs(void) {
    std::vector<std::string> temp = Split(outputLogNameFile_, "/");
    temp = Split(temp.back(), ".");
    std::string log_draw = outputLogFile_ + temp[0] + ".txt";
    const int n = log_draw.length() + 1;
    char filename[n];
    std::strcpy(filename, log_draw.c_str());
    std::fstream appendFileToWorkWith;
    appendFileToWorkWith.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    if (!appendFileToWorkWith )  {
        appendFileToWorkWith.open(filename,  std::fstream::in | std::fstream::out | std::fstream::trunc);
    }
    std::ofstream ofs;
    ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    appendFileToWorkWith << "set terminal pngcairo enhanced font \"arial,10\" fontscale 1.0 size 1080,500" << std::endl;
    appendFileToWorkWith << "set output '"+ outputPictures_ + temp[0] + ".png'" << std::endl;
    appendFileToWorkWith << "reset" << std::endl;
    appendFileToWorkWith << "set title \"" + temp[0] + "\"" << std::endl;
    appendFileToWorkWith << "set xlabel \"Time\"" << std::endl;
    appendFileToWorkWith << "set ylabel \"Error\"" << std::endl;
    appendFileToWorkWith << "set grid" << std::endl;
    appendFileToWorkWith << "plot '" + outputLogNameFile_ +"' w point pt 7, '' with labels center offset 3.4,.5 notitle" << std::endl;
    std::string temppp = "start gnuplot -p " + log_draw + "&";
    const char * plot = temppp.c_str();
    system(plot);
    appendFileToWorkWith.close();
    remove(filename);
}


void Dataset::trainNN(std::vector<unsigned> &topology, int epoch, int index) {
    Net mynet(topology);
    std::vector<double> resultVals;
    std::fstream appendFileToWorkWith = createDataFile(topology, index);
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < epoch; i++ ) {
        for (int j = 0; j < data_.size(); j++) {
            mynet.feedForward(data_[j].input_);
            mynet.getResults(resultVals);
            mynet.backProp(data_[j].output_);
            appendFileToWorkWith << mynet.getRecentAverageError() << "\n";
        }
    }
    appendFileToWorkWith.close();
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    logs_[index].push_back(std::to_string(mynet.getRecentAverageError()));
    logs_[index].push_back(std::to_string(elapsed_seconds.count()));
    drawData(logs_[index][0]);
    nn_.push_back(mynet);
}

void Dataset::drawData(std::string &file) {
    std::vector<std::string> temp = Split(file, "/");
    std::vector<std::string> gnu_name = Split(temp[4],".");
    std::string gnufile = outputPictures_ + temp[4];
    delete_files_.push_back(gnufile);
    const int n = gnufile.length() + 1;
    char filename[n];
    std::strcpy(filename, gnufile.c_str());
    std::fstream appendFileToWorkWith;
    appendFileToWorkWith.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    if (!appendFileToWorkWith )  {
        appendFileToWorkWith.open(filename,  std::fstream::in | std::fstream::out | std::fstream::trunc);
    }
    int total_lines = data_.size();
    auto lines = std::to_string(total_lines + (total_lines * 5 / 100));
    appendFileToWorkWith << "set terminal pngcairo enhanced font \"arial,10\" fontscale 1.0 size 1080,500" << std::endl;
    appendFileToWorkWith << "set output '"+ outputPictures_ + gnu_name[0] + ".png'" << std::endl;
    appendFileToWorkWith << "reset" << std::endl;
    appendFileToWorkWith << "set xrange [0:"+ lines +"]" << std::endl;
    appendFileToWorkWith << "set title \"" + gnu_name[0] + "\"" << std::endl;
    appendFileToWorkWith << "set xlabel \"X\"" << std::endl;
    appendFileToWorkWith << "set ylabel \"Y\"" << std::endl;
    appendFileToWorkWith << "set grid" << std::endl;
    appendFileToWorkWith << "plot \"" + file + "\" title \"\" with line" << std::endl;
    std::string temppp = "start gnuplot -p " + gnufile + "&";
    const char * plot = temppp.c_str();
    system(plot);
    appendFileToWorkWith.close();
    remove(filename);
}

std::fstream Dataset::createDataFile(std::vector<unsigned> &topology, int index) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
    auto str = oss.str();
    std::string topology_temp;
    for (int i = 0; i < topology.size()-1; i++) {
        topology_temp += std::to_string(topology[i]) + "-";
    }
    topology_temp += std::to_string(topology.back());
    std::string tempError = outputDataNameFile_ + "AverageError_" + topology_temp + "_" + str + ".txt";
    logs_[index].push_back(tempError);
    logs_[index].push_back(topology_temp);
    const int n = tempError.length() + 1;
    char filename[n];
    std::strcpy(filename, tempError.c_str());
    std::fstream appendFileToWorkWith;
    appendFileToWorkWith.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    if (!appendFileToWorkWith )  {
        appendFileToWorkWith.open(filename,  std::fstream::in | std::fstream::out | std::fstream::trunc);
    } 
    return appendFileToWorkWith;
}

void Dataset::writeLogs(void) {
    std::vector<std::string> temp = Split(inputDataNameFile_, "/");
    std::vector<std::string> temp2 = Split(temp.back(), ".");
    std::string log_file = outputLogFile_ + temp2.front() + ".log";
    outputLogNameFile_ = log_file;
    const int n = log_file.length() + 1;
    char filename[n];
    std::strcpy(filename, log_file.c_str());
    std::fstream logFile;
    logFile.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    if (!logFile )  {
        logFile.open(filename,  std::fstream::in | std::fstream::out | std::fstream::trunc);
    }
    for (int i = 0; i < logs_.size(); i++) {
        logFile << logs_[i][3] << " "  << logs_[i][2] << " " << logs_[i][1] << " "  << logs_[i][0] << "\n";
    }
    logFile.close();
}

void Dataset::getData(void) {
    std::vector<std::string> temp_split;
    std::vector<double> temp_data;
    Tensor temp;
    for (int i = 1; i < Count_lines(inputDataNameFile_) ; i+=2) {
        temp_split = Split(Get_line(inputDataNameFile_,i)," ");
        for (int j = 0; j < temp_split.size(); j++) {
            temp.input_.push_back(std::stod(temp_split[j]));
        }
        temp_split.clear();
        temp_split = Split(Get_line(inputDataNameFile_,i+1)," ");
        for (int j = 0; j < temp_split.size(); j++) {
            temp.output_.push_back(std::stod(temp_split[j]));
        }
        temp_split.clear();
        data_.push_back(temp);
        temp.clear();
    }
}

void Dataset::getTopology(void) {
    std::vector<unsigned> topology_vect;
    for (int i = 1; i <= Count_lines(topologyNameFile_); i++) {
        std::string topology = Get_line(topologyNameFile_, i);
        std::vector<std::string> temp = Split(topology, " ");
        for (int j = 0; j < temp.size(); j++) {
            topology_vect.push_back(std::stoul(temp[j],nullptr,0));
        }
        topology_.push_back(topology_vect);
        topology_vect.clear();
    }
}

Dataset::~Dataset() {
    topologyFile_.close();
    inputDataFile_.close();
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

unsigned Dataset::Count_lines (const std::string file) {
    unsigned lines = 0;
    std::ifstream file_to_count(file);
    std::string unused;
    while (std::getline(file_to_count, unused)) {
        ++lines;
    }
    return lines;
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