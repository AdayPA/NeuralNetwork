CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic
TARGET := ann
SRCS := src/ann.cpp src/dataset.cpp src/net.cpp src/neuron.cpp
HDRS := src/dataset.hpp src/net.hpp src/neuron.hpp src/tensor.hpp
PYTHON := python3
PLOT_SCRIPT := tools/plot_xor.py
NETWORK_SCRIPT := tools/render_network.py
LOG_DIR := data/output/log
DATA_DIR := data/output/data
PICTURE_DIR := data/output/pictures
TOPOLOGY_FILE := data/input/topology.txt
XOR_OUTPUT := $(LOG_DIR)/xor.log
PREDICTIONS_FILE := predictions.txt
PLOT_OUT := xor_plot.png
DOT_OUT := network.dot
NETWORK_PNG := network.png
RUN_INPUTS := data/input/xor.txt data/input/topology.txt

.PHONY: all clean distclean run plot network dirs

all: $(TARGET)

$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

dirs: $(DATA_DIR) $(LOG_DIR) $(PICTURE_DIR)

$(DATA_DIR) $(LOG_DIR) $(PICTURE_DIR):
	mkdir -p $@

$(PREDICTIONS_FILE) $(XOR_OUTPUT): $(TARGET) $(RUN_INPUTS) | $(DATA_DIR) $(LOG_DIR) $(PICTURE_DIR)
	./$(TARGET) > $(PREDICTIONS_FILE)

run: $(PREDICTIONS_FILE) $(XOR_OUTPUT)

plot: $(PREDICTIONS_FILE) $(XOR_OUTPUT)
	$(PYTHON) $(PLOT_SCRIPT) --log $(XOR_OUTPUT) --predictions $(PREDICTIONS_FILE) --out $(PLOT_OUT)

$(DOT_OUT): $(TOPOLOGY_FILE)
	$(PYTHON) $(NETWORK_SCRIPT) --topology $(TOPOLOGY_FILE) --out $(DOT_OUT)

network: $(DOT_OUT)
	@command -v dot >/dev/null || { echo "Graphviz dot is required: install graphviz"; exit 1; }
	dot -Tpng $(DOT_OUT) -o $(NETWORK_PNG)

clean:
	rm -f $(TARGET) $(PREDICTIONS_FILE) $(PLOT_OUT) $(DOT_OUT) $(NETWORK_PNG)

distclean: clean
	rm -rf data/output
