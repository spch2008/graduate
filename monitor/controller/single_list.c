#include "single_list.h"

void init_list(list_node **head)
{
    list_node *node = (list_node*)malloc(sizeof(list_node));
    if (node == NULL) {
        printf("list node error\n");
        exit(0);
    }
    
    node->next = NULL;
    node->data = NULL;
    
    *head = node;
}

void destroy_list(list_node *head)
{
    if (head == NULL)
        return;
        
    while (head != NULL) {
        list_node *del = head;
        head = head->next;
        
        free(del);
    }
}

list_node * find_list(list_node *head, void *data)
{
    if (head == NULL)
        return NULL;
        
    list_node *curr = head;
    while (curr != NULL) {
        if (curr->data == data)
            break;
        curr = curr->next;
    }
    
    return curr;
}

void add_list(list_node *head, void *data)
{
    if (head == NULL)
        return;
        
    if (find_list(head, data) != NULL)
        return;
        
    list_node *node = (list_node*)malloc(sizeof(list_node));
    if (node == NULL) {
        printf("malloc node error\n");
        exit(0);
    }
    
    node->data = data;
    node->next = head->next;
    head->next = node;
}

void del_list(list_node *head, void *data)
{
    if (head == NULL || data == NULL)
        return;
        
    list_node *prev = head;
    list_node *curr = head->next;
    
    while (curr != NULL) {
        if (curr->data == data) {
            prev->next = curr->next;
            free(curr);
            break;
        }
        
        prev = curr;
        curr = curr->next;
    }
}
