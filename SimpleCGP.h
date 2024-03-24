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
    Circuit child;
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
    generations(generations), best(best), optimizeVar(optimizeVar) {srand(time(0));}

    void pointMutation() {
        child = this->best;
        MutationType mutationType = static_cast<MutationType>(rand() % 6);
                    
        if(mutationType==MUTATE_OUTPUT){
            unsigned int selectedOutput = rand() % child.output_nodes.size();
            unsigned int newOutput = rand() % child.gates.size();
            child.output_nodes[selectedOutput] = newOutput;

        }else{
            unsigned int gateToMutate = rand() % child.gates.size();
            if(mutationType==MUTATE_FUNCTION){
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_int_distribution<int> dist(0, 1);
                child.gates[gateToMutate]->logic_function = dist(mt) == 0 ? '&' : '|';

            }else if(mutationType==MUTATE_INPUT_0 || mutationType==MUTATE_INPUT_1){
                int newInput = rand() % (gateToMutate + child.n_inputs) - child.n_inputs;
                //Se mutationType==3 (MUTATE_INPUT_1) => mutar input 1 (3-2)
                child.gates[gateToMutate]->inputs[mutationType-2] = newInput;

            }else if(mutationType==MUTATE_INVERT_INPUT_0 || mutationType==MUTATE_INVERT_INPUT_1){
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_int_distribution<int> dist(0, 1);
                int randomBit = dist(mt);
                //Se mutationType==5 (MUTATE_INVERT_INPUT_1) => mutar inverter input 1 (4-2)
                child.gates[gateToMutate]->invert_inputs[mutationType-4] = randomBit;
            }
        }
        child.update();
    }

    bool isChildBetter(){
        //Primeiro, compara se têm a mesma tabela verdade, se não, é pior
        for(size_t out = 0; out < child.output_nodes.size(); ++out) {
            for (size_t i = 0; i < child.gates[child.output_nodes[out]]->output_states.size(); ++i) {
                if (child.gates[child.output_nodes[out]]->output_states[i] != best.gates[best.output_nodes[out]]->output_states[i]) {
                    return false;
                }
            }
        }
        // É melhor ou igual na variável a ser melhorada?
        if(child.parameters[optimizeVar]<= best.parameters[optimizeVar])
            return true;
        else
            return false;
    }

    void run() {
        for(unsigned int i = 0; i < generations; ++i) {
            std::cout <<"Geração:"<< i+1 <<" Entropia:"<<best.parameters[ENTROPY] <<" Tamanho:"<<best.parameters[SIZE] <<" Depth:"<<best.parameters[DEPTH] << std::endl;
            pointMutation();
            if(isChildBetter()) {
                best = child;
            }
        }
    }
    
};
#endif // GENETIC_ALGORITHM_H