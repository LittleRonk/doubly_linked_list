# Doubly Linked List Implementation in C

This project provides a generic implementation of a doubly linked list in C. The list supports various operations such as insertion, deletion, sorting, and traversal.

## Features

- **Dynamic Memory Management**: The list dynamically allocates memory for nodes and data.
- **Generic Data Handling**: The list can store any type of data by using `void*` pointers.
- **Deep Copy and Destruction**: The list supports custom copy and destroy functions for deep copying and freeing data.
- **Sorting**: The list can be sorted using either merge sort or bubble sort.
- **Reverse**: The list can be reversed in place.
- **Iteration**: The list supports applying a function to each element.

## Usage

### Building the Project

To build the project, use the following commands:

```bash
mkdir build
cd build
cmake ..
make
