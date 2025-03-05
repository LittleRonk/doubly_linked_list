#include <stdlib.h>
#include <doubly_linked_list.h>
#include <stdio.h>

DllList *dll_create(void * (*copy) (const void *data),
        void (*destroy) (void *data))
{
    if ((!copy && destroy) || (copy && !destroy))
        return NULL;

    DllList *list = calloc(1, sizeof(DllList));
    if (!list)
        return NULL;

    DllNode *top_limiter = calloc(1, sizeof(DllNode));
    if (!top_limiter)
        goto top_limiter_error;

    DllNode *end_limiter = calloc(1, sizeof(DllNode));
    if (!end_limiter)
        goto end_limiter_error;

    top_limiter->prev = NULL;
    top_limiter->next = end_limiter;
    top_limiter->data = NULL;

    end_limiter->prev = top_limiter;
    end_limiter->next = NULL;
    end_limiter->data = NULL;

    list->size = 0;
    list->copy = copy;
    list->destroy = destroy;
    list->head = top_limiter;
    list->tail = end_limiter;

    return list;

    end_limiter_error:
        free(top_limiter);
    top_limiter_error:
        free(list);
    return NULL;
}

void dll_clear(DllList *list)
{
    if (!list || (list->size == 0))
        return;

    DllNode *cur_node = list->head->next;
    DllNode *temp_node = NULL;

    /* We go throught the list from beginning to end, deleting nodes and,
     * if necessary, destroying data. */
    while(cur_node != list->tail)
    {
        temp_node = cur_node;
        cur_node = cur_node->next;

        if (list->destroy)
            list->destroy(temp_node->data);
        free(temp_node);
    }

    // The limiters point to each other.
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->size = 0;
}

void dll_destroy(DllList *list)
{
    if (!list)
        return;

    dll_clear(list);
    free(list->head);
    free(list->tail);
    free(list);
    list = NULL;
}

/*
 * @brief: Creates a new node and copies the data.
 * 
 * @param list: Pointer to the list to which the created node will be added.
 * @param data: Pointer to data.
 * @return: A pointer to the node, or NULL on failure.
 */
static DllNode *create_node_and_copy_data(const DllList *list, const void *data)
{
    DllNode *new_node = calloc(1, sizeof(DllNode));
    if (!new_node)
        return NULL;

    if (list->copy)
    {
        new_node->data = list->copy(data);
        if (!new_node->data)
        {
            free(new_node);
            return NULL;
        }
    }
    else
        new_node->data = (void *) data;

    return new_node;
}

bool dll_push_front(DllList *list, const void *data)
{
    if (!list)
        return false;

    DllNode *new_node = create_node_and_copy_data(list, data);
    if (!new_node)
        return false;

    new_node->next = list->head->next;
    new_node->prev = list->head;
    list->head->next->prev = new_node;
    list->head->next = new_node;
    
    ++list->size;
    return true;
}

bool dll_push_back(DllList *list, const void *data)
{
    if (!list)
        return false;

    DllNode *new_node = create_node_and_copy_data(list, data);
    if (!new_node)
        return false;
    
    new_node->prev = list->tail->prev;
    new_node->next = list->tail;
    list->tail->prev->next = new_node;
    list->tail->prev = new_node;

    ++list->size;
    return true;
}

/*
 * @brief: Search for a node by its index.
 *
 * @param list: A pointer to the list.
 * @param index: Index of the node being searched.
 * @return: Pointer to node, or NULL on failure.
 */
static DllNode *dll_get_node_at(const DllList *list, size_t index)
{
    if (!list || (list->size == 0) || (index >= list->size))
        return NULL;

    DllNode *node = NULL;

    /* We go throught the list from the beginning to the end of from the end
     * to the beginning, depending on the distance of the desing node from
     * the middle of the list */
    if (index <= (list->size / 2))
    {
        node = list->head->next;
        for (int i = 0; i < index; ++i)
            node = node->next;
    }
    else
    {
        node = list->tail->prev;
        for (int i = list->size - 1; i > index; --i)
            node = node->prev;
    }

    return node;
}

bool dll_insert_at(DllList *list, size_t index, const void *data)
{
    if (!list)
        return false;

    DllNode *new_node = create_node_and_copy_data(list, data);
    if (!new_node)
        return false;

    // Add an element to the beginning of the list.
    if (index == 0)
        dll_push_front(list, data);

    // Add an element to the end of the list.
    if (index >= list->size)
        dll_push_back(list, data);
    
    DllNode *before = dll_get_node_at(list, index); // The node before which a new node should be insert.
    DllNode *after = before->prev;                  // The node after which a new node must be inserted.

    // Switching the pointers
    new_node->prev = after;
    new_node->next = before;
    after->next = new_node;
    before->prev = new_node;

    ++list->size;
    return true;
}

bool dll_remove_at(DllList *list, size_t index)
{
    if (!list || (list->size == 0) || (list->size <= index))
        return false;

    DllNode *del_node = dll_get_node_at(list, index); // The node to be removed.
	DllNode *before = del_node->prev;                 // The node before the node being deleted
	DllNode *after = del_node->next;                  // The node after the node being deleted.

    // If deep coping is used delete the node data.
    if (list->destroy)
        list->destroy(del_node->data);

    // We free the memory occupied by the node and redirect the pointers.
    free(del_node);
    before->next = after;
    after->prev = before;

    --list->size;
    return true;
}

void * dll_pop_front(DllList *list)
{
    if (!list || (list->size == 0))
        return NULL;

    DllNode *temp = list->head->next;
    void *data = temp->data;

    list->head->next = temp->next;
    temp->next->prev = list->head;
    --list->size;

    free(temp);
    return data;
}

void * dll_pop_back(DllList *list)
{
    if(!list || (list->size == 0))
        return false;
    
	DllNode *temp = list->tail->prev;
    void *data = temp->data;

    list->tail->prev = temp->prev;
    temp->prev->next = list->tail;
    --list->size;

    free(temp);
    return data;
}

void * dll_get_at(const DllList *list, size_t index)
{
    if (!list || (list->size == 0) || (index >= list->size))
        return NULL;
    
	DllNode *node = dll_get_node_at(list, index);

    return node->data;
}

bool dll_is_empty(const DllList *list)
{
    if (!list)
        return true;
    return list->size == 0;
}

size_t dll_size(const DllList *list)
{
    if (!list)
        return 0;
    return list->size;
}

void dll_reverse(DllList *list)
{
    if (!list || (list->size < 2))
        return;

	DllNode *node_1 = list->head;
	DllNode *node_2 = list->tail;
    void *temp = NULL;

    int iter = (list->size / 2) + 1;
    for (int i = 0; i < iter; ++i)
    {
        temp = node_1->data;
        node_1->data = node_2->data;
        node_2->data = temp;
        node_1 = node_1->next;
        node_2 = node_2->prev;
    }
}

void dll_for_each(DllList *list, void (*func) (void *))
{
	DllNode *cur_node = list->head->next;
    while(cur_node->next != NULL)
    {
        func(cur_node->data);
        cur_node = cur_node->next;
    }
}

DllList * dll_copy(const DllList *list)
{
    if (!list)
        return NULL;

    DllList *new_list = dll_create(list->copy, list->destroy);
    if (!new_list)
        return NULL;

	DllNode *cur = list->head->next;
    while (cur->next != NULL)
    {
        if (!dll_push_back(new_list, cur->data))
        {
            dll_destroy(new_list);
            return NULL;
        }
        cur = cur->next;
    }

    return new_list;
}

/*
 * @brief: Swaps nodes.
 *
 * @param node_1: Pointer to the first node.
 * @param node_2: Pointer to the second node.
 */
void swape_node(DllNode *node_1, DllNode *node_2)
{
    node_1->next = node_2->next;
    node_2->next->prev = node_1;

    node_1->prev->next = node_2;
    node_2->prev = node_1->prev;
    node_1->prev = node_2;
    node_2->next = node_1;
}

/*
 * @brief: Extracts a node from a list.
 *
 * @param node: Pointer to the node to be extracted.
 */
static DllNode *extract_node(DllNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

/*
 * @brief: Inserts a node into a position before or after a reference node.
 *
 * @param reference_node: Pointer to the node relative to which the insertion occurs.
 * @param insertion_node: Pointer to the node to be inserted.
 * @param position: Defines the position to insert. Before the anchor node if 'position > 0',
 * after the anchor node if 'position < 0'.
 */
void insert_node(DllNode *reference_node, DllNode *insertion_node, int position)
{
    if (!reference_node || !insertion_node || position == 0)
        return;
    
    if (position > 0)
    {
        insertion_node->next = reference_node;
        insertion_node->prev = reference_node->prev;

        reference_node->prev->next = insertion_node;
        reference_node->prev = insertion_node;
    }
    else
    {
        insertion_node->prev = reference_node;
        insertion_node->next = reference_node->next;

        reference_node->next->prev = insertion_node;
        reference_node->next = insertion_node;
    }
}

void dll_bubble_sort(DllList *list, int (*cmp) (const void *, const void *))
{
	DllNode *min_max = NULL;
	DllNode *head_stop = list->head;
	DllNode *tail_stop = list->tail;
	DllNode *item_1 = NULL;
	DllNode *item_2 = NULL;
    bool is_sorted = false;

    while (!is_sorted)
    {
        is_sorted = true;
        item_1 = head_stop->next;
        item_2 = item_1->next;
        min_max = item_1;
        while (item_2 != tail_stop)
        {
            if (cmp(item_2->data, min_max->data) > 0)
                min_max = item_2;
            if (cmp(item_1->data, item_2->data) > 0)
            {
                is_sorted = false;
                swape_node(item_1, item_2);
            }
            item_1 = item_2;
            item_2 = item_2->next;
        }
        extract_node(min_max);
        insert_node(tail_stop, min_max, 1);
        tail_stop = min_max;

        is_sorted = true;
        item_2 = tail_stop->prev;
        item_1 = item_2->prev;
        min_max = item_2;
        while(item_1 != head_stop)
        {
            if (cmp(item_1->data, min_max->data) < 0)
                min_max = item_1;
            if (cmp(item_1->data, item_2->data) > 0)
            {
                is_sorted = false;
                swape_node(item_1, item_2);
            }
            item_2 = item_1;
            item_1 = item_1->prev;
        }
        extract_node(min_max);
        insert_node(head_stop, min_max, -1);
        head_stop = min_max;
    }
}

DllNode *merge_sort(DllNode *start, DllNode *end, size_t size, int (*cmp) (const void *, const void *))
{
    if (size == 1)
        return start;

    size_t left_part_size = size / 2;
    size_t right_part_size = size - left_part_size;

    // Find the initial node of the right side and last node
    // of the left side
    DllNode *left_part_end = NULL;
    DllNode *right_part_start = start;
    for (int i = 0; i < left_part_size; ++i)
        right_part_start = right_part_start->next;
    left_part_end = right_part_start->prev;

    // Recursivery sort the right and left parts
    DllNode *left_part = merge_sort(start, left_part_end, left_part_size, cmp);
    DllNode *right_part = merge_sort(right_part_start, end, right_part_size, cmp);

    // Pointers to the start and end nodes of the combined left and right parts
    DllNode *unity_start = NULL;
    DllNode *unity_end = NULL;

    // Combaine the left and the right parts
    for (int i = 0; i < size; ++i)
    {
        // If the length of the right or left part is zero,
        // then we add the remaining part without any additional checks
        if (left_part_size == 0)
        {
            unity_end->next = right_part;
            unity_end = right_part;
            right_part = right_part->next;
            continue;
        }
        if (right_part_size == 0)
        {
            unity_end->next = left_part;
            unity_end = left_part;
            left_part = left_part->next;
            continue;
        }

        // We using the comparison function to find the smallest element
        // and add it to the end of the united part.
        if (cmp(left_part->data, right_part->data) == -1)
        {
            if (i == 0)
                unity_start = left_part;
            else
                unity_end->next = left_part;
            unity_end = left_part;
            left_part = left_part->next;
            --left_part_size;
        }
        else
        {
            if (i == 0)
                unity_start = right_part;
            else
                unity_end->next = right_part;
            unity_end = right_part;
            right_part = right_part->next;
            --right_part_size;
        }
    }

    return unity_start;
}

void dll_sort(DllList *list, int (*cmp) (const void *, const void *))
{
    if (!list || list->size < 2)
        return;

    DllNode *sorted_list_start = merge_sort(list->head->next, list->tail->prev, list->size, cmp);
    sorted_list_start->prev = list->head;
    list->head->next = sorted_list_start;

    DllNode *sorted_list_end = sorted_list_start;
    DllNode *temp = NULL;
    int iter = list->size - 1;
    for (int i = 0; i < iter; ++i)
    {
        temp = sorted_list_end;
        sorted_list_end = sorted_list_end->next;
        sorted_list_end->prev = temp;
    }
    sorted_list_end->next = list->tail;
    list->tail->prev = sorted_list_end;
}
