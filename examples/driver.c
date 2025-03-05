#include <stdlib.h>
#include <stdio.h>
#include <doubly_linked_list.h>

void * copy_item(const void *item);
void free_item(void *item);
void print_dll(void *data);
int cmp(const void *item_1, const void *item_2);

int main(void)
{
    float x = 1.1;
    float y = 2.2;
    float z = 3.3;
    float d = 4.4;
    float f = 5.5;
    float r = 6.6;

    DllList *my_list = dll_create(copy_item, free_item);

    dll_push_back(my_list, &x);
    dll_push_back(my_list, &y);
    dll_push_back(my_list, &z);
    dll_push_back(my_list, &d);
    dll_push_back(my_list, &f);
    dll_push_back(my_list, &r);
    dll_push_back(my_list, &y);
    dll_push_back(my_list, &x);
    dll_push_back(my_list, &r);
    dll_insert_at(my_list, 2, &f);

    dll_for_each(my_list, print_dll);
    printf("\n****************\n");

    dll_remove_at(my_list, 1);
    printf("The element with index 1 has been removed.\n");
    printf("index 0: %.1f\n", *((float *) dll_get_at(my_list, 0)));
    printf("index 1: %.1f\n", *((float *) dll_get_at(my_list, 1)));
    printf("index 2: %.1f\n", *((float *) dll_get_at(my_list, 2)));
    printf("index 3: %.1f\n", *((float *) dll_get_at(my_list, 3)));
    printf("index 4: %.1f\n", *((float *) dll_get_at(my_list, 4)));
    printf("index 5: %.1f\n", *((float *) dll_get_at(my_list, 5)));
    printf("index 6: %.1f\n", *((float *) dll_get_at(my_list, 6)));
    printf("index 7: %.1f\n", *((float *) dll_get_at(my_list, 7)));

    printf("\n****************\n");
    float *ret_1 = dll_pop_front(my_list);
    float *ret_2 = dll_pop_back(my_list);
    printf("Dll pop front: %.1f\n", *ret_1);
    printf("Dll pop back: %.1f\n", *ret_2);
    printf("List size: %zu\n", dll_size(my_list));

    printf("\n****************\n");
    dll_for_each(my_list, print_dll);

    printf("\n****************\n");
    printf("List is empty: %s\n", dll_is_empty(my_list) ? "true" : "false");
    printf("List size: %zu\n", dll_size(my_list));

    printf("\n****************\n");
    dll_sort(my_list, cmp);
    printf("The list is sorted:\n");
    dll_for_each(my_list, print_dll);
    printf("index 1: %.1f\n", *((float *) dll_get_at(my_list, 1)));

    printf("\n****************\n");
    dll_reverse(my_list);
    dll_for_each(my_list, print_dll);

    dll_clear(my_list);
    printf("\n****************\n");
    dll_push_back(my_list, &x);
    dll_push_back(my_list, &r);
    dll_for_each(my_list, print_dll);

    dll_destroy(my_list);

    return 0;
}

void *copy_item(const void *item)
{
    float *new_item = (float *) calloc(1, sizeof(float));
    *new_item = *((float *) item);
    
    return new_item;
}

void free_item(void *item)
{
    free((float *) item);
}

void print_dll(void *data)
{
    printf("%.1f\n", *((float *) data));
}

int cmp(const void *item_1, const void *item_2)
{
    if (*((float *) item_1) == *((float *) item_2))
        return 0;
    else if (*((float *) item_1) > *((float *) item_2))
        return 1;
    return -1;
}
