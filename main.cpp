#include "Circuit.h"

int main() {
    Circuit circuit;
    circuit.loadFromVerilog("benchmarks/04-adder.v");
    circuit.simulate();
    std::cout <<"Entropia:"<<circuit.entropy << std::endl;
    std::cout <<"Tamanho:"<<circuit.gates.size() << std::endl;
    std::cout <<"Depth:"<<circuit.depth << std::endl;

    return 0;
}
