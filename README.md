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
```

### Running the Example

After building the project, you can run the example driver program:

```bash
./driver
```

### Example Code

Here is an example of how to use the doubly linked list:

```c
#include <doubly_linked_list.h>
#include <stdio.h>

void *copy_item(const void *item) {
    int *new_item = calloc(1, sizeof(int));
    *new_item = *((int *)item);
    return new_item;
}

void free_item(void *item) {
    free((int*)item);
}

void print_item(void *data, void *arg) {
    printf("%d ", *((int *)data));
}

int cmp(const void *item_1, const void *item_2) {
    return *((int *)item_1) - *((int *)item_2);
}

int main() {
    DllList *list = dll_create(copy_item, free_item);

    int x = 10, y = 20, z = 30;
    dll_push_back(list, &x);
    dll_push_back(list, &y);
    dll_push_back(list, &z);

    dll_for_each(list, print_item, NULL);  // Output: 10 20 30

    dll_sort(list, cmp);
    dll_for_each(list, print_item, NULL);  // Output: 10 20 30

    dll_destroy(list);
    return 0;
}
```

## API Documentation

### Structures

- **DllNode**: Represents a node in the doubly linked list.
- **DllList**: Represents doubly linked list itself.

### Functions

- **dll_create**: Creates a new doubly linked list.
- **dll_destroy**: Destroys the list and frees all allocated memory.
- **dll_clear**: Clears all elements from the list.
- **dll_push_front**: Inserts an element at the front of the list.
- **dll_push_back**: Inserts an element at the back of the list.
- **dll_insert_at**: Inserts an element at a specified index.
- **dll_pop_front**: Removes and returns the element from the front of the list.
- **dll_pop_back**: Removes and returns the element from the back of the list.
- **dll_get_at**: Retrieves the element at a specified index.
- **dll_remove_at**: Removes the element at a specified index.
- **dll_sort**: Sorts the list using merge sort.
- **dll_bubble_sort**: Sort the list using bubble sort.
- **dll_reverse**: Reverses the list.
- **dll_for_each**: Applies a function to each element in the list.
