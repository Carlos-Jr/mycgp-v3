#ifndef SIMPLE_CGP_H
#define SIMPLE_CGP_H

#include "Circuit.h"
#include <random>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <filesystem>

class SimpleCGP {
public:
    unsigned int generations;
    Circuit best;
    CircuitParameters optimizeVar;

    enum MutationType {
        MUTATE_OUTPUT,
        MUTATE_FUNCTION,
        MUTATE_INPUT_0,
        MUTATE_INPUT_1,
        MUTATE_INVERT_INPUT_0,
        MUTATE_INVERT_INPUT_1
    };

    SimpleCGP(unsigned int generations, Circuit &best, CircuitParameters optimizeVar) :
    generations(generations), best(best), optimizeVar(optimizeVar) {}

    Circuit pointMutation(Circuit* parent) {
        Circuit child = *parent;
        std::random_device rd;
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

    bool isChildBetter(Circuit *oneChild){        //Primeiro, compara se têm a mesma tabela verdade, se não, é pior
        for(size_t out = 0; out < oneChild->output_nodes.size(); ++out) {
            for (size_t i = 0; i < oneChild->gates[oneChild->output_nodes[out]]->output_states.size(); ++i) {
                if (oneChild->gates[oneChild->output_nodes[out]]->output_states[i] != best.gates[best.output_nodes[out]]->output_states[i]) {
                    return false;
                }
            }
        }
        // É melhor ou igual na variável a ser melhorada?
        if(oneChild->parameters[optimizeVar]<= best.parameters[optimizeVar])
            return true;
        else
            return false;
    }
        

        void run(std::string name) {
            std::cout <<"generation,entropy,size,depth,change" << std::endl;
            for(unsigned int i = 0; i < generations; ++i) {
                std::cout << i <<","<< best.parameters[ENTROPY] <<","<<best.parameters[SIZE] <<","<<best.parameters[DEPTH];

                Circuit child = pointMutation(&best);

                if(isChildBetter(&child)) {
                    best = child;
                    std::cout<<",1" << std::endl;
                }else{
                    std::cout<<",0" << std::endl;
                }
            }
            // auto timestamp = std::to_string(std::time(nullptr));
            // best.saveVerilog(name + timestamp + ".v");
        }
};
#endif