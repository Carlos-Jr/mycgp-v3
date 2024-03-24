#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <omp.h>
#include <functional>

enum CircuitParameters {
  SIZE,
  DEPTH,
  ENTROPY
};

class Gate {
public:
    std::string logic_function;
    std::vector<int> inputs;
    std::vector<bool> invert_inputs;
    bool active;
    std::vector<int> input_states;
    std::vector<int> output_states;
    double entropy;
    

    Gate(std::string logic_function, std::vector<int> inputs, std::vector<bool> invert_inputs)
        : logic_function(logic_function), inputs(inputs), invert_inputs(invert_inputs), active(false), entropy(0) {}

    Gate(const Gate& other)
        : logic_function(other.logic_function), inputs(other.inputs), invert_inputs(other.invert_inputs), active(other.active), input_states(other.input_states), output_states(other.output_states), entropy(other.entropy) {}
        
    std::string to_string() {
        std::stringstream ss;
        ss << "{\n";
        ss << "  \"Function\": \"" << logic_function << "\",\n";
        ss << "  \"Inputs\": [";
        for (size_t i = 0; i < inputs.size(); ++i) {
            ss << inputs[i];
            if (i < inputs.size() - 1) ss << ", ";
        }
        ss << "]\n";
        ss << "  \"Invert\": [";
        for (size_t i = 0; i < invert_inputs.size(); ++i) {
            ss << invert_inputs[i];
            if (i < invert_inputs.size() - 1) ss << ", ";
        }
        ss << "],\n";
        ss << "  \"input states\": [";
        for (size_t i = 0; i < input_states.size(); ++i) {
            ss << input_states[i];
            if (i < input_states.size() - 1) ss << ", ";
        }
        ss << "],\n";
        ss << "  \"output states\": [";
        for (size_t i = 0; i < output_states.size(); ++i) {
            ss << output_states[i];
            if (i < output_states.size() - 1) ss << ", ";
        }
        ss << "]\n";
        ss << "}";
        return ss.str();
    }
};

class Circuit {
public:
    std::vector<Gate*> gates;
    int n_inputs;
    int n_outputs;
    float parameters[3];
    std::map<std::string, int> nodeMapping;
    std::vector<int> output_nodes;

    Circuit() : n_inputs(0), n_outputs(0) {}

    Circuit(const Circuit& other)
        : n_inputs(other.n_inputs), n_outputs(other.n_outputs) {
        std::copy(std::begin(other.parameters), std::end(other.parameters), std::begin(parameters));
        nodeMapping = other.nodeMapping;
        output_nodes = other.output_nodes;

        gates.resize(other.gates.size(), nullptr);
        for (size_t i = 0; i < other.gates.size(); ++i) {
            if (other.gates[i] != nullptr) {
                gates[i] = new Gate(*other.gates[i]);
            }
        }
    }

    Circuit& operator=(const Circuit& other) {
        if (this == &other) return *this;

        n_inputs = other.n_inputs;
        n_outputs = other.n_outputs;
        std::copy(std::begin(other.parameters), std::end(other.parameters), std::begin(parameters));
        nodeMapping = other.nodeMapping;
        output_nodes = other.output_nodes;

        for (auto& gate : gates) {
            delete gate;
        }
        gates.clear();
        gates.resize(other.gates.size(), nullptr);
        for (size_t i = 0; i < other.gates.size(); ++i) {
            if (other.gates[i] != nullptr) {
                gates[i] = new Gate(*other.gates[i]);
            }
        }

        return *this;
    }

    ~Circuit() {
        for (auto& gate : gates) {
            delete gate;
        }
    }
    
    void loadFromVerilog(const std::string& filepath) {
        std::cout << "Loading Verilog..." << std::endl;
        std::ifstream file(filepath);
        std::string line;
        std::vector<std::string> lines;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        std::string accumulatedLineWire;
        std::string accumulatedLineInput;
        std::string accumulatedLineOutput;
        bool accumulatingWire = false;
        bool accumulatingInput = false;
        bool accumulatingOutput = false;
        std::vector<std::string> nodesList;
        std::vector<std::string> inputs;
        std::vector<std::string> outputs;

        for (const auto& line : lines) {
            if (line.find("wire") != std::string::npos || accumulatingWire) {
                accumulatingWire = true;
                accumulatedLineWire += line.substr(0, line.size()) + " ";
                if (line.find(";") != std::string::npos) {
                    accumulatingWire = false;
                    accumulatedLineWire.erase(std::remove(accumulatedLineWire.begin(), accumulatedLineWire.end(), '\n'), accumulatedLineWire.end());
                    accumulatedLineWire.erase(std::remove(accumulatedLineWire.begin(), accumulatedLineWire.end(), ';'), accumulatedLineWire.end());
                    accumulatedLineWire = accumulatedLineWire.substr(accumulatedLineWire.find("n"));
                    accumulatedLineWire.erase(std::remove_if(accumulatedLineWire.begin(), accumulatedLineWire.end(), ::isspace), accumulatedLineWire.end());
                    
                    std::stringstream ss(accumulatedLineWire);
                    std::string token;
                    while (std::getline(ss, token, ',')) {
                        nodesList.push_back(token);
                    }
                    accumulatedLineWire.clear();
                }
            } else if (line.find("input") != std::string::npos || accumulatingInput) {
                accumulatingInput = true;
                accumulatedLineInput += line.substr(0, line.size()) + " ";
                if (line.find(";") != std::string::npos) {
                    accumulatingInput = false;
                    accumulatedLineInput.erase(std::remove(accumulatedLineInput.begin(), accumulatedLineInput.end(), '\n'), accumulatedLineInput.end());
                    accumulatedLineInput.erase(std::remove(accumulatedLineInput.begin(), accumulatedLineInput.end(), ';'), accumulatedLineInput.end());
                    accumulatedLineInput = accumulatedLineInput.substr(accumulatedLineInput.find("pi"));
                    accumulatedLineInput.erase(std::remove_if(accumulatedLineInput.begin(), accumulatedLineInput.end(), ::isspace), accumulatedLineInput.end());
                    std::stringstream ss(accumulatedLineInput);
                    std::string token;
                    while (std::getline(ss, token, ',')) {
                        inputs.push_back(token);
                    }
                    accumulatedLineInput.clear();
                }
            } else if (line.find("output") != std::string::npos || accumulatingOutput) {
                accumulatingOutput = true;
                accumulatedLineOutput += line.substr(0, line.size()) + " ";
                if (line.find(";") != std::string::npos) {
                    accumulatingOutput = false;
                    accumulatedLineOutput.erase(std::remove(accumulatedLineOutput.begin(), accumulatedLineOutput.end(), '\n'), accumulatedLineOutput.end());
                    accumulatedLineOutput.erase(std::remove(accumulatedLineOutput.begin(), accumulatedLineOutput.end(), ';'), accumulatedLineOutput.end());
                    accumulatedLineOutput = accumulatedLineOutput.substr(accumulatedLineOutput.find("po"));
                    accumulatedLineOutput.erase(std::remove_if(accumulatedLineOutput.begin(), accumulatedLineOutput.end(), ::isspace), accumulatedLineOutput.end());
                    std::stringstream ss(accumulatedLineOutput);
                    std::string token;
                    while (std::getline(ss, token, ',')) {
                        outputs.push_back(token);
                    }
                    accumulatedLineOutput.clear();
                }
            }
        }

        n_inputs = inputs.size();
        n_outputs = outputs.size();

        std::cout << "    Nodes list:";
        for (const auto& node : nodesList) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
        std::cout << "    Inputs list:";
        for (const auto& n : inputs) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
        std::cout << "    Outputs list:";
        for (const auto& n : outputs) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
        
        nodesList.insert(nodesList.end(), outputs.begin(), outputs.end());

        gates.resize(nodesList.size(), nullptr);

        for (size_t i = 0; i < nodesList.size(); ++i) {
            nodeMapping[nodesList[i]] = i;
        }

        for (size_t i = 0; i < inputs.size(); i++)
        {
            nodeMapping[inputs[i]] = -i-1;
        }

        std::cout << "    Node Mapping:\n";
        for (const auto& pair : nodeMapping) {
            std::cout <<"        "<<pair.first << " -> " << pair.second << std::endl;
        }

        std::cout << "    Output gates:";
        for (const auto& n : outputs) {
            output_nodes.push_back(nodeMapping[n]);
            std::cout << nodeMapping[n] << " ";
        }
        std::cout << std::endl;

        for (const auto& line : lines) {
            if (line.find("assign") != std::string::npos) {
                std::string cleanedLine = line;
                cleanedLine.erase(std::remove(cleanedLine.begin(), cleanedLine.end(), ' '), cleanedLine.end());
                cleanedLine.erase(std::remove(cleanedLine.begin(), cleanedLine.end(), ';'), cleanedLine.end());
                cleanedLine.erase(std::remove(cleanedLine.begin(), cleanedLine.end(), '\n'), cleanedLine.end());
                cleanedLine.erase(0, 6); // Remove "assign"

                std::stringstream ss(cleanedLine);
                std::string equationParts[2];
                std::getline(ss, equationParts[0], '=');
                std::getline(ss, equationParts[1], '=');

                std::string logic_function = "?";
                if (equationParts[1].find("&") != std::string::npos) {
                    logic_function = "&";
                }else if (equationParts[1].find("|") != std::string::npos) {
                    logic_function = "|";
                }

                std::vector<int> gateInputs;
                std::vector<bool> invertGateInputs;
                std::stringstream ssInputs(equationParts[1]);
                std::string token;
                while (std::getline(ssInputs, token, logic_function[0])) {
                    bool invert = false;
                    if (token.find("~") != std::string::npos) {
                        invert = true;
                        token.erase(token.find("~"), 1);
                    }
                    gateInputs.push_back(nodeMapping[token]);
                    invertGateInputs.push_back(invert);
                }

                Gate* thisGate = new Gate(logic_function, gateInputs, invertGateInputs);
                std::string outputNode = equationParts[0];
                outputNode.erase(std::remove_if(outputNode.begin(), outputNode.end(), [](unsigned char c) { return !std::isalnum(c); }), outputNode.end());
                int gateNumber = nodeMapping[outputNode];
                gates[gateNumber] = thisGate;
            }
        }

        std::cout << filepath << " loaded!\n" << std::endl;
    }

    void update() {
        this->parameters[ENTROPY] = 0;
        this->parameters[DEPTH] = 0;
        float entropy = 0;
        int n_combinations = std::pow(2, n_inputs);
        
        for (auto& gate : gates) {
            gate->input_states.clear();
            gate->output_states.clear();
            gate->input_states.resize(n_combinations, 0);
            gate->output_states.resize(n_combinations, 0);
            gate->active = false;
        }

        //Calculo de Depth
        unsigned int activeSize = 0;
        std::vector<int> depth(gates.size(), 0);
        std::function<int(int)> dfs = [&](int node) {
            if (depth[node] > 0) return depth[node];
            if (gates[node] == nullptr) return 0; // No gate connected
            
            gates[node]->active = true;
            ++activeSize;

            int maxDepth = 0;
            for (int input : gates[node]->inputs) {
                if (input < 0) { // Input node
                    maxDepth = std::max(maxDepth, 1);
                } else {
                    maxDepth = std::max(maxDepth, dfs(input) + 1);
                }
            }
            depth[node] = maxDepth;
            return maxDepth;
        };

        int maxPath = 0;
        for (int outputNode : output_nodes) {
            if (outputNode >= 0) { // Skip if it's an input signal
                maxPath = std::max(maxPath, dfs(outputNode));
            }
        }

        this->parameters[DEPTH] = maxPath;
        this->parameters[SIZE] = activeSize;

        // Simulação do circuito e calculo dos estados
        #pragma omp parallel for
        for (int combination_input = 0; combination_input < n_combinations; ++combination_input) {
            for (auto& gate : gates) {
                if(gate->active){
                    std::vector<bool> gateInputStates(gate->inputs.size(), false);

                    for (size_t i = 0; i < gate->inputs.size(); ++i) {
                        bool value = false;
                        if (gate->inputs[i] < 0) {
                            value = (combination_input >> (-gate->inputs[i] - 1)) & 1;
                        } else {
                            value = gates[gate->inputs[i]]->output_states[combination_input] > 0;
                        }

                        if (gate->invert_inputs[i]) {
                            value = !value;
                        }

                        gateInputStates[gate->inputs.size() - i - 1] = value;
                    }

                    int inputStateNumber = 0;
                    for (bool state : gateInputStates) {
                        inputStateNumber = (inputStateNumber << 1) | (state ? 1 : 0);
                    }
                    gate->input_states[combination_input] = inputStateNumber;

                    if (gate->logic_function == "&") {
                        gate->output_states[combination_input] = std::all_of(gateInputStates.begin(), gateInputStates.end(), [](bool b) { return b; }) ? 1 : 0;
                    } else if (gate->logic_function == "|") {
                        gate->output_states[combination_input] = std::any_of(gateInputStates.begin(), gateInputStates.end(), [](bool b) { return b; }) ? 1 : 0;
                    } else {
                        throw std::runtime_error("Logic gate is not AND or OR");
                    }
                }
            }
        }
        
        //Calculo da Entropia total do circuito
        #pragma omp parallel for reduction(+:entropy)
        for (auto& gate : gates) {
            std::map<int, int> uniqueInputs;
            for (int value : gate->input_states) {
                uniqueInputs[value]++;
            }
            double sumUniqueInputs = std::accumulate(uniqueInputs.begin(), uniqueInputs.end(), 0, [](int sum, const std::pair<int, int>& p) { return sum + p.second; });
            double Hx = 0;
            for (auto& p : uniqueInputs) {
                double pValue = static_cast<double>(p.second) / sumUniqueInputs;
                Hx -= pValue * std::log2(pValue);
            }

            std::map<int, int> uniqueOutputs;
            for (int value : gate->output_states) {
                uniqueOutputs[value]++;
            }
            double sumUniqueOutputs = std::accumulate(uniqueOutputs.begin(), uniqueOutputs.end(), 0, [](int sum, const std::pair<int, int>& p) { return sum + p.second; });
            double Hy = 0;
            for (auto& p : uniqueOutputs) {
                double pValue = static_cast<double>(p.second) / sumUniqueOutputs;
                Hy -= pValue * std::log2(pValue);
            }

            entropy += (Hx - Hy);
        }
        this->parameters[ENTROPY] = entropy;
        
        
    }
};

#endif