#include "Circuit.h"
#include "SimpleCGP.h"
#include "SPEAmod.h"
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <VerilogFileName> [<generations> <parameter>]" << std::endl;
        return 1;
    }
    
    if (!std::filesystem::exists(argv[1])) {
        std::cerr << "File does not exist: " << argv[1] << std::endl;
        return 1;
    }
    
    std::string path = argv[1];
    size_t lastSlashPos = path.find_last_of('/');
    std::string bench_name = path.substr(lastSlashPos + 1);
    bench_name = bench_name.substr(0, bench_name.size() - 2); // Removing ".v"

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
            SimpleCGP ga(runGenerations,circuit,ENTROPY);
            ga.run(bench_name);
        }else if(parameter == "SIZE"){
            SimpleCGP ga(runGenerations,circuit,SIZE);
            ga.run(bench_name);
        }else if(parameter == "DEPTH"){
            SimpleCGP ga(runGenerations,circuit,DEPTH);
            ga.run(bench_name);
        }else{
            std::cerr << "Parameter does not exist\n";
        }
    }else{
        SimpleCGP ga(runGenerations,circuit,ENTROPY);
        ga.run(bench_name);
    }

    return 0;
}