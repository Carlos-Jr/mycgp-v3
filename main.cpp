#include "Circuit.h"

#include "Circuit.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <VerilogFileName>" << std::endl;
        return 1;
    }
    Circuit circuit;
    circuit.loadFromVerilog(argv[1]);
    circuit.simulate();
    std::cout <<"Entropia:"<<circuit.entropy << std::endl;
    std::cout <<"Tamanho:"<<circuit.gates.size() << std::endl;
    std::cout <<"Depth:"<<circuit.depth << std::endl;

    return 0;
}