#ifndef SPEAMOD_H
#define SPEAMOD_H

#include "Circuit.h"
#include <random>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <filesystem>

class SPEAmod {
public:
    std::random_device rd;

    unsigned int generations;
    Circuit original;
    std::vector<std::pair<Circuit, unsigned int>> archive;// The archive saves a pair of a circuit and its errors (unsigned int)
    CircuitParameters objective;

    enum MutationType {
        MUTATE_OUTPUT,
        MUTATE_FUNCTION,
        MUTATE_INPUT_0,
        MUTATE_INPUT_1,
        MUTATE_INVERT_INPUT_0,
        MUTATE_INVERT_INPUT_1
    };

    SPEAmod(unsigned int generations, Circuit &original, CircuitParameters objective) :
    generations(generations), original(original), objective(objective) {}

    Circuit pointMutation(Circuit* parent) {
        Circuit child = *parent;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> distMutationType(0, 5);
        MutationType mutationType = static_cast<MutationType>(distMutationType(mt));

        if(mutationType==MUTATE_OUTPUT){
            std::uniform_int_distribution<unsigned int> distOutput(0, child.output_nodes.size() - 1);
            unsigned int selectedOutput = distOutput(mt);
            std::uniform_int_distribution<unsigned int> distNewOutput(0, child.gates.size() - 1);
            unsigned int newOutput = distNewOutput(mt);
            child.output_nodes[selectedOutput] = newOutput;

        }else{
            std::uniform_int_distribution<unsigned int> distGateToMutate(0, child.gates.size() - 1);
            unsigned int gateToMutate = distGateToMutate(mt);
            if(mutationType==MUTATE_FUNCTION){
                std::uniform_int_distribution<int> distFunction(0, 1);
                child.gates[gateToMutate]->logic_function = distFunction(mt) == 0 ? '&' : '|';
                //Se o tamanho for menor que 2, gerar uma outra input aleatória
                if(child.gates[gateToMutate]->inputs.size()<2){
                    std::uniform_int_distribution<int> distNewInput(-static_cast<int>(child.n_inputs), gateToMutate - 1);
                    int newInput = distNewInput(mt);
                    child.gates[gateToMutate]->inputs.push_back(newInput);
                    std::uniform_int_distribution<int> distBit(0, 1);
                    int randomBit = distBit(mt);
                    child.gates[gateToMutate]->invert_inputs.push_back(randomBit);
                }
            }else if(mutationType==MUTATE_INPUT_0 || mutationType==MUTATE_INPUT_1){
                std::uniform_int_distribution<int> distNewInput(-static_cast<int>(child.n_inputs), gateToMutate - 1);
                int newInput = distNewInput(mt);
                //Se mutationType==3 (MUTATE_INPUT_1) => mutar input 1 (3-2)
                child.gates[gateToMutate]->inputs[mutationType-2] = newInput;

            }else if(mutationType==MUTATE_INVERT_INPUT_0 || mutationType==MUTATE_INVERT_INPUT_1){
                std::uniform_int_distribution<int> distBit(0, 1);
                int randomBit = distBit(mt);
                //Se mutationType==5 (MUTATE_INVERT_INPUT_1) => mutar inverter input 1 (4-2)
                if(mutationType==MUTATE_INVERT_INPUT_1 && child.gates[gateToMutate]->inputs.size()>1)
                    child.gates[gateToMutate]->invert_inputs[mutationType-4] = randomBit;
            }
            child.update();
        }
        return child;
    }

    unsigned int getErrors(Circuit *oneCircuit){
        unsigned int errors = 0;
        for(size_t out = 0; out < oneCircuit->output_nodes.size(); ++out) {
            for (size_t i = 0; i < oneCircuit->gates[oneCircuit->output_nodes[out]]->output_states.size(); ++i) {
                if (oneCircuit->gates[oneCircuit->output_nodes[out]]->output_states[i] != original.gates[original.output_nodes[out]]->output_states[i]) {
                    ++errors;
                }
            }
        }
        return errors;
    }
    
    int dominates(Circuit *A,unsigned int errorsA,Circuit *B,unsigned int errorsB){
        bool Adominates = false;
        bool Bdominates = false;
            
        if((A->parameters[objective] < B->parameters[objective]) || (errorsA < errorsB)){ 
            Adominates = true;
        }
        if((A->parameters[objective] > B->parameters[objective]) || (errorsA > errorsB)){ 
            Bdominates = true;
        }
        
        if(Adominates && !Bdominates){
            return 1;
        }else if(!Adominates && Bdominates){
            return -1;
        }
        
        return 0;
    }

    void run(unsigned int lambda) {
        archive.push_back(std::make_pair(original,0));
        std::cout <<"generation,entropy,size,depth,errors" << std::endl;
        for(unsigned int i = 0; i < generations; ++i) {
            std::uniform_int_distribution<size_t> dist(0, archive.size() - 1);
            int randomParent = dist(rd);
            std::vector<std::pair<Circuit, unsigned int>> children;
            for (int i = 0; i < lambda; ++i) {
                Circuit child = pointMutation(&archive[randomParent].first);
                unsigned int childErrors = getErrors(&child);
                children.push_back(std::make_pair(child,childErrors));
            }
            for (auto &child : children) {
                bool dominated = false;
                for (int i =0;i<archive.size();++i) {
                    int childDominatesArchive = dominates(&child.first, child.second, &archive[i].first, archive[i].second);
                    if (childDominatesArchive<0) {
                        dominated = true;
                    }else if (childDominatesArchive>0){
                        archive.erase(archive.begin() + i);
                    }else{
                        bool allEqual = true;
                        for (int param = 0; param < 3; ++param) {
                            if (child.first.parameters[param] != archive[i].first.parameters[param]) {
                                allEqual = false;
                                break;
                            }
                        }
                        if (allEqual) {
                            archive.erase(archive.begin() + i);
                        }
                    }
                }
                if (!dominated) {
                    archive.push_back(child);
                }
            }

            for (const auto &element : archive) {
                std::cout << i << "," << element.first.parameters[ENTROPY] << "," << element.first.parameters[SIZE] << "," << element.first.parameters[DEPTH] << "," << element.second << std::endl;
            }
            
        }
    }
    
};
#endif