#include "Circuit.h"
#include "SimpleCGP.h"
#include "SPEAmod.h"
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <VerilogFileName> [<generations> <parameter>]" << std::endl;
        return 1;
    }
    
    if (!std::filesystem::exists(argv[1])) {
        std::cerr << "File does not exist: " << argv[1] << std::endl;
        return 1;
    }

    Circuit circuit;
    circuit.loadFromVerilog(argv[1]);
    circuit.update();

    int runGenerations = 70000;
    //Argumento numero de gerações
    if(argc>2)
        runGenerations = std::stoi(argv[2]);
    
    // Argumento de parametro
    if(argc>3){
        std::string parameter = argv[3];
        std::transform(parameter.begin(), parameter.end(), parameter.begin(), ::toupper);

        if(parameter == "ENTROPY"){
            SPEAmod ga(runGenerations,circuit,ENTROPY);
            ga.run(4);
        }else if(parameter == "SIZE"){
            SPEAmod ga(runGenerations,circuit,SIZE);
            ga.run(4);
        }else if(parameter == "DEPTH"){
            SPEAmod ga(runGenerations,circuit,DEPTH);
            ga.run(4);
        }else{
            std::cerr << "Parameter does not exist\n";
        }
    }else{
        SPEAmod ga(runGenerations,circuit,ENTROPY);
        ga.run(4);
    }

    return 0;
}