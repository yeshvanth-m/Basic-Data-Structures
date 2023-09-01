/* A basic implementation of a statically allocated Doubly Linked List (DLL) buffer 
    An element can be added only at the tail, but can be removed from anywhere.
*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Buffer size */
#define BUFFER_LENGTH           10


/* The data organized in structure */
typedef struct data_t
{
    int idx;
    int data;
    struct data_t *next;
    struct data_t *prev;
} data_t;

/* Static allocation of data is preferred */
data_t data[BUFFER_LENGTH];

/* LIFO buffer structure declaration */
typedef struct 
{
    int length;
    int alloc_count;
    data_t *base;
    data_t *end;
    data_t *head;
    data_t *tail;
} dll_buf_t;

dll_buf_t dll_buf_ctrl;

/* DLL buffer return code */
typedef enum 
{
    RC_DLLBUF_OK,
    RC_DLLBUF_ERR_FULL,
    RC_DLLBUF_ERR_EMPTY,
    RC_DLLBUF_NOT_FOUND,
} dll_rc_t;

void debug_dll_pointer(void)
{
    
}

/* Function to check if the DLL buffer is empty */
dll_rc_t dll_is_bufEmpty (void)
{
    dll_rc_t rc = RC_DLLBUF_OK;

    /* If the allocated count is 0, then the buffer is empty */
    if (dll_buf_ctrl.alloc_count == 0)
    {
        rc = RC_DLLBUF_ERR_EMPTY;
    }
    return rc;
}

/* Function to check if the DLL buffer is full */
dll_rc_t dll_is_bufFull (void)
{
    dll_rc_t rc = RC_DLLBUF_OK;

    /* If the allocated count is 0, then the buffer is empty */
    if (dll_buf_ctrl.alloc_count == dll_buf_ctrl.length)
    {
        rc = RC_DLLBUF_ERR_FULL;
    }
    return rc;
}

/* Funtion to add an element into the DLL buffer */
dll_rc_t dll_add (data_t *element)
{
    /* Check if the buffer is full */
    dll_rc_t rc = dll_is_bufFull();
    /* Get the tail */
    data_t *tail = dll_buf_ctrl.tail;

    if (rc == RC_DLLBUF_OK)
    {
        /* Element will be added to tail always */
        if (dll_buf_ctrl.alloc_count == 0)
        {
            /* Queue is empty */
            memset ((void *)tail, 0u, sizeof(element));
            memcpy ((void *)tail, (void *)element, sizeof(element));
        }
        else
        {
            /* There are elements present in queue */
            /* Add the new element at the end of tail */
            tail = tail->next;
            /* Link the prev of newly added element to the old tail */
            tail->prev = dll_buf_ctrl.tail;
            /* Copy the element */
            memset ((void *)tail, 0u, sizeof(element));
            memcpy ((void *)tail, (void *)element, sizeof(element));

            /* Update the new tail */
            dll_buf_ctrl.tail = tail;
        }
        dll_buf_ctrl.alloc_count++;
    }
    return rc;
}

/* Function to pop and element from the LIFO buffer */
dll_rc_t dll_remove (data_t *element, bool conv_remove, int idx)
{
    /* Check if the buffer is empty */
    dll_rc_t rc = dll_is_bufEmpty();

    if (rc == RC_DLLBUF_OK) 
    {
        /* Store the head that is removed for later operations */
        data_t *head = dll_buf_ctrl.head, *tail = dll_buf_ctrl.tail, *end = dll_buf_ctrl.end;

        if (conv_remove)
        {
            data_t *remove_element = dll_buf_ctrl.head;
            /* Remove the element from the head in case of conventional remove */
            memcpy ((void *)element, (void *)remove_element, sizeof(element));
            if (dll_buf_ctrl.alloc_count > 1)
            {
                /* Move the current head */
                head = head->next;
                /* De-link the previous pointer as the element is removed */
                head->prev = NULL;
                /* The removed element is the end of queue so de-link the next */
                remove_element->next = NULL;
                /* Assign next of old end to the current element */
                end->next = remove_element;
                /* Update the end to the last removed element */
                dll_buf_ctrl.end = remove_element;
                /* Update the new head */
                dll_buf_ctrl.head = head;
            }
            dll_buf_ctrl.alloc_count--;
        }
        else
        {
            rc = RC_DLLBUF_NOT_FOUND;
            /* Always start from the head */
            data_t *traverse_var = head;

            if (dll_buf_ctrl.alloc_count == 1)
            {
                /* Index matched */
                if (traverse_var->idx == idx)
                {
                    /* Remove the only element present in the queue if index matches */
                    memcpy ((void *)element, (void *)traverse_var, sizeof(element));
                    /* Decrement the count */
                    dll_buf_ctrl.alloc_count--;
                    rc = RC_DLLBUF_OK;
                }
            }
            else
            {
                do
                {
                    /* Index of the search element matched */
                    if (traverse_var->idx == idx)
                    {
                        /* Remove the element from the middle in case of conditional remove */
                        memcpy ((void *)element, (void *)traverse_var, sizeof(element));
                        /* The element matched is the head */
                        if (traverse_var == head)
                        {
                            /* Move the head to the next pointing element */
                            head = head->next;
                            /* De-link the previous of the head */
                            head->prev = NULL;
                            /* Update the new head */
                            dll_buf_ctrl.head = head;
                        }
                        else if (traverse_var == tail)
                        {
                            /* The element matched is tail */
                            /* Move the tail backward by one */
                            tail = tail->prev;
                            /* Assign the next of tail to the next elemenet in queue */
                            tail->next = traverse_var->next;
                            /* Update the new tail */
                            dll_buf_ctrl.tail = tail;
                        }
                        else
                        {
                            traverse_var->prev->next = traverse_var->next;
                            traverse_var->next->prev = traverse_var->prev;
                        }
                        /* De-link the prev and next removed element */
                        traverse_var->prev = traverse_var->next = NULL;
                        /* Link the previous end to the newly removed element */
                        end->next = traverse_var;
                        /* Assign the end as the newly removed element */
                        dll_buf_ctrl.end = traverse_var;
                        /* Decrement the count */
                        dll_buf_ctrl.alloc_count--;

                        rc = RC_DLLBUF_OK;
                        /* Break the loop in case of element match */
                        break;
                    }
                    else
                    {
                        /* Move to the next element if index not matched */
                        traverse_var = traverse_var->next;
                    }
                    /* Move till the tail of the queue */
                } while (traverse_var != tail->next);
            }
        }
    }
    return rc;
}

dll_rc_t dll_traverse (void)
{
    /* Check if the buffer is empty */
    dll_rc_t rc = dll_is_bufEmpty();

    if (rc == RC_DLLBUF_OK) 
    {
        /* Always start from the head */
        data_t *traverse_var = dll_buf_ctrl.head, *tail = dll_buf_ctrl.tail;
        do
        {
            /* Print the index and data of elements in the queue */
            printf ("[Idx: %d, Data: %d] -> ", traverse_var->idx, traverse_var->data);
            traverse_var = traverse_var->next;
        } while (traverse_var != tail->next);
        printf ("End\n");
    }
    return rc;
}

/* Initialize the LIFO buffer */
void dll_init (void)
{
    /* Set the buffer size */
    dll_buf_ctrl.length = BUFFER_LENGTH;
    /* Initialize the allocated count */
    dll_buf_ctrl.alloc_count = 0;
    /* Make the base of the buffer point to the 0th element of the array */
    dll_buf_ctrl.base = &data[0];
    /* Make the head and tail point to the base as the buffer is empty */
    dll_buf_ctrl.tail = dll_buf_ctrl.head = dll_buf_ctrl.base;
    /* Do not use the prev and next of the head tail and base */
    dll_buf_ctrl.tail->prev = dll_buf_ctrl.head->prev = dll_buf_ctrl.base->prev = NULL;
    dll_buf_ctrl.tail->next = dll_buf_ctrl.head->next = dll_buf_ctrl.base->next = NULL;

    for (int i = 0; i < (dll_buf_ctrl.length - 1); i++)
    {
        /* Link all the nodes in the queue with next */
        data[i].next = &data[i + 1];
    }
    /* Initialize the end of the queue as last element in the DLL */
    dll_buf_ctrl.end = &data[dll_buf_ctrl.length - 1];
    /* Do not use the prev and next of the end */
    dll_buf_ctrl.end->prev = dll_buf_ctrl.end->next = NULL;

    /* There is no element after end' */
    data[dll_buf_ctrl.length - 1].next = NULL;
}

/* De-Initialize the LIFO buffer */
void dll_deInit (void)
{
    /* Set the buffer size */
    dll_buf_ctrl.length = 0;
    /* Make the base of the buffer point to NULL */
    dll_buf_ctrl.base = NULL;
    /* Make the head and tail point to the NULL */
    dll_buf_ctrl.tail = dll_buf_ctrl.head = dll_buf_ctrl.base;
}

void debug_pointers (void)
{
    for (int i = 0; i < 4; i++)
        printf ("%d   %d   %d\n", data[i].prev - &data[0], &data[i] - &data[0], data[i].next - &data[0]);
}

dll_rc_t dll_run_test (void)
{
    
}

int main()
{
    printf("\nDoubly Linked List Buffer Implementation. Length of buffer: %d", BUFFER_LENGTH);
    char symbol;
    data_t element;
    dll_rc_t rc;
    
    /* Initialize the LIFO buffer */
    dll_init();

    /* 1 to push, 2 to pop, 3 to exit */
    while (symbol != '4')
    {
        printf("\nEnter 1 to add, 2 to remove, 3 to traverse, and 4 to exit: ");
        scanf(" %c", &symbol);

        switch (symbol)
        {
            case '1': 
            {
                /* Get the input and push if the buffer is not full */
                printf("\nEnter the index of element to be added: \nIdx: ");
                scanf("%d", &element.idx);
                printf("Data: ");
                scanf("%d", &element.data);
                rc = dll_add(&element);

                if (rc == RC_DLLBUF_OK) 
                {
                    printf("\nData added successfully.\n");
                }
                else 
                {
                    printf("\nError - buffer full. Remove an element and try again.\n");
                }
            }
            break;
            case '2':
            {
                char conv_remove;
                /* Remove an element if the buffer is not empty  */
                printf ("\nEnter Y for conventional remove, N for conditional remove: ");
                scanf (" %c", &conv_remove);

                if (conv_remove == 'Y')
                {
                    rc = dll_remove (&element, true, 0);
                }
                else
                {
                    int idx;
                    printf ("\nEnter the index to search: ");
                    scanf (" %d", &idx);
                    rc = dll_remove (&element, false, idx);
                }

                if (rc == RC_DLLBUF_OK) 
                {
                    printf("\nElement removed successfully.");
                    printf("\nIdx: %d, Data: %d\n", element.idx, element.data);
                }
                else if (rc == RC_DLLBUF_NOT_FOUND)
                {
                    printf ("\nElement not found. \n");
                }
                else 
                {
                    printf ("\nError - buffer empty. add an element and try again.\n");
                }
            }
            break;
            case '3':
            {
                /* Traverse the LIFO buffer */
                rc = dll_traverse();
                
                if (rc != RC_DLLBUF_OK)
                {
                    /* In case of buffer empty */
                    printf("\nBuffer empty. Push an element and try again.\n");
                }
            }
            break;
            default:
            {
                /* Exit on 4 */
                break;
            }
        }
    }

    /* De-init the pointers */
    dll_deInit();
    printf("\nExited program");
    return 0;
}