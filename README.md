# Cartesian Genetic Programming for Combinational Digital Circuits Optimization

### Overview

This repository hosts the code for a project dedicated to optimizing combinational digital circuits through Cartesian Genetic Programming (CGP). The primary focus is on improving three key aspects: energy efficiency (adhering to Landauer's principle), size (measured by the number of gates), and depth (the longest path from input to output). This project leverages the principles of genetic programming to evolve digital circuits that are more efficient, compact, and shallower, aiming to push the boundaries of digital circuit design.

### Introduction to the Core Concepts

#### Combinational Digital Circuits

Combinational digital circuits are the foundation of digital systems, performing logical operations on inputs to produce outputs. Unlike sequential circuits, their outputs depend only on the current inputs, not on past inputs. These circuits are essential for tasks ranging from simple logic gates operations to complex arithmetic functions.

#### Cartesian Genetic Programming (CGP)

CGP is a form of genetic programming that evolves programs or digital circuits represented in a fixed-length genotype. It uses a two-dimensional grid of nodes to represent the genes, where each node corresponds to a function or operation in the circuit. CGP is notable for its effectiveness in evolving complex structures with relatively simple genetic mechanisms and has been successfully applied to a range of optimization problems, including digital circuit design.

#### Optimization Targets

- **Energy Efficiency**: Following Landauer's principle, which posits that there is a minimum possible amount of energy required to change one bit of information.
- **Size**: The size of a circuit is measured by the number of gates it contains. A smaller circuit uses fewer resources and can be more cost-effective.
- **Depth**: The depth of a circuit refers to the longest path from any input to any output. Reducing depth can lead to faster operation times since signals have shorter paths to traverse.

### Getting Started
#### Prerequisites

- C++ Compiler with OpenMP support
- OpenMP library for parallel computing

#### Compilation

This project requires OpenMP for parallel execution to optimize the computation process. The code is compiled with the following command to enable OpenMP and apply optimization flags:

```
g++ -fopenmp -O3 -o main ./main.cpp
```

- `-fopenmp`: Enables the OpenMP support in the compiler, allowing the code to run multiple processes in parallel.
- `-O3`: Applies level 3 optimizations during the compilation, enhancing the execution speed and efficiency of the generated program. 

#### Running the Program

After compilation, you can run the program with:
```
./main
```