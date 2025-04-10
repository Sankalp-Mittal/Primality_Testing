# Primality_Testing

This is repository containing all the codes and plots made for experimenting and rediscovering the Miller Rabin Algorithm for primality testing.
<br>This was done as a course project for __CS648 (Randomized Algorithms)__ course at the __Indian Institute of Technology Kanpur__

## Contributors
- Sankalp Mittal
- Debraj Karmakar
- Naman Kumar Jaiswal

## Instructions

All the `.cpp` codes in this repository are written using the `gmp.h` C++ library and require a linux environment to run. 
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
