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
#include <random>

enum CircuitParameters {
  SIZE,
  DEPTH,
  ENTROPY
};

class Gate {
public:
    char logic_function;
    std::vector<int> inputs;
    std::vector<bool> invert_inputs;
    bool active;
    std::vector<int> input_states;
    std::vector<int> output_states;
    double entropy;
    

    Gate(char logic_function, std::vector<int> inputs, std::vector<bool> invert_inputs)
        : logic_function(logic_function), inputs(inputs), invert_inputs(invert_inputs), active(false), entropy(0) {}

    Gate(const Gate& other)
        : logic_function(other.logic_function), inputs(other.inputs), invert_inputs(other.invert_inputs), active(other.active), input_states(other.input_states), output_states(other.output_states), entropy(other.entropy) {}
        
    std::string to_string() {
        std::stringstream ss;
        ss << "{\n";
        ss << "  \"Function\": \"" << logic_function << "\",\n";
        ss << "  \"Is_Active\": \"" << (active?"Yes":"No") << "\",\n";
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
        ss << "  \"input_states\": [";
        for (size_t i = 0; i < input_states.size(); ++i) {
            ss << input_states[i];
            if (i < input_states.size() - 1) ss << ", ";
        }
        ss << "],\n";
        ss << "  \"output_states\": [";
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
    double parameters[3];
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
    
    void randomizeGate(int node){
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(0, 1);
        char logic_functions[2] = {'&', '|'};
        char newlogic_function = logic_functions[dist(mt)]; 

        int newInput0 = rand() % (node + n_inputs) - n_inputs;
        bool invert_input0 = static_cast<bool>(dist(mt));

        int newInput1 = rand() % (node + n_inputs) - n_inputs;
        bool invert_input1 = static_cast<bool>(dist(mt));

        gates[node]->active = false;

        gates[node]->logic_function = newlogic_function;

        gates[node]->inputs.clear();
        gates[node]->invert_inputs.clear();

        gates[node]->inputs.push_back(newInput0);
        gates[node]->inputs.push_back(newInput1);
        gates[node]->invert_inputs.push_back(invert_input0);
        gates[node]->invert_inputs.push_back(invert_input1);
    }

    void expandCircuit(float value){
        unsigned int newGates = static_cast<int>(gates.size() * value) - gates.size();
        std::cout << "Acrescentar " << newGates << " portas no circuito\n";
        for(int i=0;i<newGates;++i){
            gates.push_back(new Gate('&', std::vector<int>(), std::vector<bool>()));
            randomizeGate(gates.size()-1);
        }
    }

    void loadFromVerilog(const std::string& filepath) {
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
        
        nodesList.insert(nodesList.end(), outputs.begin(), outputs.end());

        gates.resize(nodesList.size(), nullptr);
        
        for (size_t i = 0; i < inputs.size(); i++)
        {
            nodeMapping[inputs[i]] = -i-1;
        }

        output_nodes.resize(n_outputs,0);

        int nodeCount = 0;
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

                char logic_function = '-';
                if (equationParts[1].find('&') != std::string::npos) {
                    logic_function = '&';
                }else if (equationParts[1].find('|') != std::string::npos) {
                    logic_function = '|';
                }

                std::vector<int> gateInputs;
                std::vector<bool> invertGateInputs;
                std::stringstream ssInputs(equationParts[1]);
                std::string token;
                while (std::getline(ssInputs, token, logic_function)) {
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
                std::cout << "Inserindo gate " << nodeCount << std::endl;
                std::cout << "Nó " << outputNode << std::endl;
                
                gates[nodeCount] = thisGate;
                nodeMapping[outputNode] = nodeCount;

                if (outputNode.find("po") != std::string::npos) {
                    int outputNodeNumber = std::stoi(outputNode.substr(2));
                    output_nodes[outputNodeNumber]= nodeCount;
                }

                ++nodeCount;
                
            }
        }
    }

    void saveStatesJSON(const std::string& filepath){
        std::ofstream logFile(filepath);
        logFile << "[\n";
        for (size_t g = 0; g < gates.size(); ++g) {
            logFile << "  {\n";
            logFile << "    \"gate\": \"" << gates[g]->logic_function << "\",\n";
            logFile << "    \"input_states\": [";
            for (size_t i = 0; i < gates[g]->input_states.size(); ++i) {
                logFile << gates[g]->input_states[i];
                if (i < gates[g]->input_states.size() - 1) logFile << ", ";
            }
            logFile << "],\n";
            logFile << "    \"output_states\": [";
            for (size_t i = 0; i < gates[g]->output_states.size(); ++i) {
                logFile << gates[g]->output_states[i];
                if (i < gates[g]->output_states.size() - 1) logFile << ", ";
            }
            logFile << "]\n";
            if (g < gates.size() - 1) logFile << "  },\n";
            else logFile << "  }\n";
        }
        logFile << "]\n";
        logFile.close();
    }

    void saveVerilog(const std::string& filepath){
        std::ofstream file(filepath);

        file<<"module LogicModule ( \n   ";

        for (int i = 0; i < n_inputs; ++i) {
            if (i > 0) file << ",";
            file << " pi" << i;
        }
        file << ",\n   ";

        for (int i = 0; i < n_outputs; ++i) {
            if (i > 0) file << ",";
            file << " po" << i;
        }
        
        file << " );\n";
        
        file << "  input ";
        for (int i = 0; i < n_inputs; ++i) {
            if (i > 0) file << ",";
            file << " pi" << i;
        }
        file << ";\n";

        file << "  output";
        for (int i = 0; i < n_outputs; ++i) {
            if (i > 0) file << ",";
            file << " po" << i;
        }
        file << ";\n";

        file << "  wire ";
        for(size_t i = 0; i < gates.size(); ++i) {
            auto it = std::find(output_nodes.begin(), output_nodes.end(), i);
            if(it == output_nodes.end()) {
                if(gates[i]->active){
                    if (i > 0) file << ",";
                    file << "n" << i;
                }
            } 
        }
        file << ";\n";


        for (size_t i = 0; i < gates.size(); i++) {
            if(gates[i]->active){
                file << "  assign ";

                auto it = std::find(output_nodes.begin(), output_nodes.end(), i);

                if (it != output_nodes.end()) {
                    file << "po" << std::distance(output_nodes.begin(), it);
                } else {
                    file << "n" << i;
                }
                file << " = ";

                file << (gates[i]->invert_inputs[0] ? "~" : "");

                if (gates[i]->inputs[0] >= 0) {
                    file << "n" << gates[i]->inputs[0];
                } else {
                    file << "pi" << (-gates[i]->inputs[0]-1);
                }

                if(gates[i]->inputs.size()>1){
                    file << " "<< gates[i]->logic_function<<" ";
                    file << (gates[i]->invert_inputs[1] ? "~" : "");

                    if (gates[i]->inputs[1] >= 0) {
                        file << "n" << gates[i]->inputs[1];
                    } else {
                        file << "pi" << (-gates[i]->inputs[1]-1);
                    }
                }

                file << ";\n";
                }
        }
        file<<"\nendmodule";
        file.close();
    }
    void saveGatesJSON(const std::string& filepath){
        std::ofstream outFile(filepath);
        outFile << "[\n";
        for (size_t i = 0; i < gates.size(); ++i) {
            if(gates[i]->active){
                outFile << "  {\n";
                outFile << "    \"id\": \"" << i << "\",\n";
                outFile << "    \"logic_function\": \"" << gates[i]->logic_function << "\",\n";
                outFile << "    \"inputs\": [";
                for (size_t j = 0; j < gates[i]->inputs.size(); ++j) {
                outFile << gates[i]->inputs[j];
                if (j < gates[i]->inputs.size() - 1) outFile << ", ";
                }
                outFile << "],\n";
                outFile << "    \"invert_inputs\": [";
                for (size_t j = 0; j < gates[i]->invert_inputs.size(); ++j) {
                outFile << (gates[i]->invert_inputs[j] ? "true" : "false");
                if (j < gates[i]->invert_inputs.size() - 1) outFile << ", ";
                }
                outFile << "]\n";
                if (i < gates.size() - 1) outFile << "  },\n";
                else outFile << "  }\n";
            }
        }
        outFile << "]";
        outFile.close();
    }
    void update() {
        this->parameters[ENTROPY] = 0;
        this->parameters[DEPTH] = 0;
        double entropy = 0;
        int n_combinations = std::pow(2, n_inputs);
        
        for (auto& gate : gates) {
            gate->input_states.clear();
            gate->output_states.clear();
            gate->input_states.resize(n_combinations, 9);
            gate->output_states.resize(n_combinations, 9);
            gate->active = false;
        }

        unsigned int activeSize = 0;
        std::vector<int> depth(gates.size(), 0);
        std::function<int(int)> dfs = [&](int node) {
            // Check for out-of-bounds access
            if (node < 0 || node >= gates.size()) {
                std::cout << "Out-of-bounds access: node = " << node << std::endl;
                return 0;
            }
            if (gates[node] == nullptr) {
                std::cout << "Null gate at node: " << node << std::endl;
                return 0; // No gate connected
            }
            if (depth[node] > 0) return depth[node];
            
            gates[node]->active = true;
            if(gates[node]->logic_function != '-')
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
            if (gates[outputNode]->logic_function !='-') { // Skip if it's an wire gate
                maxPath = std::max(maxPath, dfs(outputNode));
            }else{ //Mark Wire as active
                gates[outputNode]->active = true;
            }
        }

        this->parameters[DEPTH] = maxPath;
        this->parameters[SIZE] = activeSize;

        #pragma omp parallel for
        for (int combination_input = 0; combination_input < n_combinations; ++combination_input) {
            for (auto& gate : gates) {
                if(gate->active){
                    if (gate->logic_function == '-') {
                        bool value = (combination_input >> (-gate->inputs[0] - 1)) & 1;
                        gate->input_states[combination_input] = value;
                        gate->output_states[combination_input] = value;
                    }else{
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

                        if (gate->logic_function == '&') {
                            gate->output_states[combination_input] = std::all_of(gateInputStates.begin(), gateInputStates.end(), [](bool b) { return b; }) ? 1 : 0;
                        } else if (gate->logic_function == '|') {
                            gate->output_states[combination_input] = std::any_of(gateInputStates.begin(), gateInputStates.end(), [](bool b) { return b; }) ? 1 : 0;
                        }else {
                            throw std::runtime_error("Logic gate is not AND or OR");
                        }
                    }
                }
            }
        }
        #pragma omp parallel for reduction(+:entropy)
        for (auto& gate : gates) {
            if(gate->active){
                std::map<int, int> uniqueInputs;
                for (int value : gate->input_states) {
                    uniqueInputs[value]++;
                }
                std::map<int, int> uniqueOutputs;
                for (int value : gate->output_states) {
                    uniqueOutputs[value]++;
                }

                double Hx = 0;
                for (auto& p : uniqueInputs) {
                    double pValue = static_cast<double>(p.second) / static_cast<double>(n_combinations);
                    Hx -= pValue * std::log2(pValue);
                }
                double Hy = 0;
                for (auto& p : uniqueOutputs) {
                    double pValue = static_cast<double>(p.second) / static_cast<double>(n_combinations);
                    Hy -= pValue * std::log2(pValue);
                }

                entropy += (Hx-Hy);
            }
        }
        this->parameters[ENTROPY] = entropy;
        
        this->saveGatesJSON("debug.json");
    }
};

#endif