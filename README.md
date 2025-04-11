# Primality_Testing

This is repository containing all the codes and plots made for experimenting and rediscovering the Miller Rabin Algorithm for primality testing.
<br>This was done as a course project for __CS648 (Randomized Algorithms)__ course at the __Indian Institute of Technology Kanpur__

## Contributors
- Sankalp Mittal
- Debraj Karmakar
- Naman Kumar Jaiswal

## Instructions

All the `.cpp` codes in this repository are written using the `gmp.h` and `gmpxx.h` C++ library and require a linux environment to run. This is done to handle large numbers and to test how does the code perform as the input size increases 
<br>To install the library in __Ubuntu/Debian__ follow these steps

```
sudo apt update
sudo apt install libgmp-dev
```
To install in __Fedora__

```
sudo dnf install gmp-devel
```

To compile the `.cpp` file using this run the following command

```
g++ -o $(file_name) $(file_name).cpp  -lgmp
```
and run it as
```
./$(file_name)
```

## Organization

The repository has been organized on the following basis

- __Applications__: This contains a jupyter notebook containing the implementation of primality testing and its usage in some applications
- __Basic Analysis__: This contains some jupyter notebooks analysing Fermat's Theorem
- __data__: This contains the data generated from various experiments
- __plots__: This contains the various plots generated as a result of the experiments
- __PlottingCodes__: This contains the python scripts used to plot the graphs from the data
- __PrimalityTestingCodes__: This contaings the codes for primality testing and the various experiments done

## Decleration
The [following](https://github.com/Ssophoclis/AKS-algorithm/tree/master) github repository was used to implement the __AKS Primality__ test
