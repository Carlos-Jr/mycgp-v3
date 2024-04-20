#include "Circuit.h"
#include <fstream>
#include <string>
#include <filesystem>

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
    circuit.loadFromVerilog(argv[1],2);
    circuit.update();

    std::cout << bench_name << ", "
            << circuit.parameters[SIZE] << ", "
            << circuit.parameters[ENTROPY] << ", "
            << circuit.parameters[DEPTH] << std::endl;
    

    return 0;
}