#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>

/*
 * @brief: Node structure for the doubly linked list.
 */
typedef struct DllNode {
    void *data;             ///< Pointer to the data stored in the node.
    struct DllNode *prev;   ///< Pointer to the previous node.
    struct DllNode *next;   ///< Pointer to the next node.
} DllNode;

/*
 * @brief: Doubly linked list structure.
 */
typedef struct {
    DllNode *head;          ///< Pointer to the first node (sentinel head).
    DllNode *tail;          ///< Pointer to the last node (sentinel tail).
    size_t size;            ///< Number of elements in the list.
    
    /*
     * @brief: Function to copy data.
     *
     * @param data: Pointer to the original data.
     * @return: Pointer to the copied data.
     * @note: The user must define this function to properly allocate and copy data.
     */
    void *(*copy)(const void *data);
    
    /*
     * @brief: Function to delete data.
     *
     * @param data: Pointer to the data to be deleted.
     * @note: The user must define this function to properly free allocated memory.
     */
    void (*destroy)(void *data);
} DllList;

/*
 * @brief: Creates a new doubly linked list.
 *
 * @param copy: Function pointer for copying data.
 * @param destroy: Function pointer for destroying data.
 * @return: Pointer to the created list, or NULL on failure.
 */
DllList *dll_create(void *(*copy)(const void *), void (*destroy)(void *));

/*
 * @brief: Destroys the doubly linked list and frees all allocated memory.
 *
 * @param list: Pointer to the doubly linked list.
 * @note: The user is responsible for ensuring no dangling pointers remain after destruction.
 */
void dll_destroy(DllList *list);

/*
 * @brief: Clears all elements from the list without destroying the list itself.
 *
 * @param list: Pointer to the doubly linked list.
 */
void dll_clear(DllList *list);

/*
 * @brief: Checks if the list is empty.
 *
 * @param list: Pointer to the doubly linked list.
 * @return true: if the list is empty, false otherwise.
 */
bool dll_is_empty(const DllList *list);

/*
 * @brief: Returns the number of elements in the list.
 *
 * @param list: Pointer to the doubly linked list.
 * @return: The number of elements in the list.
 */
size_t dll_size(const DllList *list);

/*
 * @brief: Inserts an element at the front of the list.
 *
 * @param: list Pointer to the doubly linked list.
 * @param: data Pointer to the data to insert.
 * @return: true on success, false on failure.
 */
bool dll_push_front(DllList *list, const void *data);

/*
 * @brief: Inserts an element at the back of the list.
 *
 * @param list: Pointer to the doubly linked list.
 * @param data: Pointer to the data to insert.
 * @return: true on success, false on failure.
 */
bool dll_push_back(DllList *list, const void *data);

/*
 * @brief: Inserts an element at a specified index.
 *
 * @param list: Pointer to the doubly linked list.
 * @param index: Zero-based index where the element should be inserted
 * (if the index is greater than the size of the list, the element will
 * be added to the end).
 * @param data: Pointer to the data to insert.
 * @return: 'true' on succes, 'false' on failure.
 */
bool dll_insert_at(DllList *list, size_t index, const void *data);

/*
 * @brief: Removes and returns the element from the front of the list.
 *
 * @param list: Pointer to the doubly linked list.
 * @return: Pointer to the removed data, or NULL if the list is empty.
 * @note: The user is responsible for freeing the returned data if necessary.
 */
void *dll_pop_front(DllList *list);

/*
 * @brief: Removes and returns the element from the back of the list.
 *
 * @param list: Pointer to the doubly linked list.
 * @return: Pointer to the removed data, or NULL if the list is empty.
 * @note: The user is responsible for freeing the returned data if necessary.
 */
void *dll_pop_back(DllList *list);

/*
 * @brief: Retrieves the element at a specified index.
 *
 * @param: list Pointer to the doubly linked list.
 * @param: index Zero-based index of the element.
 * @return: Pointer to the data at the given index, or NULL if the index is out of bounds.
 */
void *dll_get_at(const DllList *list, size_t index);

/*
 * @brief: Removes the element at a specified index.
 *
 * @param list: Pointer to the doubly linked list.
 * @param index: Zero-based index of the element to remove.
 * @return true: if the element was removed, false otherwise.
 */
bool dll_remove_at(DllList *list, size_t index);

/*
 * @brief: Removes a node at a specified index and returns its data.
 *
 * @param list: Pointer to the doubly linked list.
 * @param index: Zero-based index of the node to remove.
 * @return: Pointer to the data of the removed node, or NULL if the index is out of bounds.
 */
void *dll_extract_at(DllList *list, size_t index);

/*
 * @brief: Creates a deep copy of the list.
 * @param: list Pointer to the doubly linked list.
 * @return: Pointer to the new list, or NULL on failure.
 */
DllList *dll_copy(const DllList *list);

/*
 * @brief: Sorts the list using merge sort (efficient for large lists).
 *
 * The comparison function should follow the convention of `qsort`:
 * - Returns a negative value if a < b
 * - Returns 0 if a == b
 * - Returns a positive value if a > b
 *
 * @param list: Pointer to the doubly linked list.
 * @param cmp: Pointer to the comparison function.
 */
void dll_sort(DllList *list, int (*cmp) (const void *, const void *));

/*
 * @brief: Sorts the list using bubble sort (simpler but less efficient for large lists).
 *
 * The comparison function should follow the convention of `qsort`:
 * - Returns a negative value if a < b
 * - Returns 0 if a == b
 * - Returns a positive value if a > b
 *
 * @param list: Pointer to the doubly linked list.
 * @param cmp: Pointer to the comparison function.
 */
void dll_bubble_sort(DllList *list, int (*cmp) (const void *, const void *));

/*
 * @brief: Applies a function to each element in the list.
 *
 * @param list: Pointer to the doubly linked list.
 * @param func: Function to apply to each element.
 */
void dll_for_each(DllList *list, void (*func) (void *));

/*
 * @brief: Reverses the list.
 *
 * @param list: Pointer to the doubly linked list.
 */
void dll_reverse(DllList *list);

#endif // DOUBLY_LINKED_LIST_H
