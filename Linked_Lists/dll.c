/* A basic implementation of a statically allocated Doubly Linked List (DLL) buffer 
    An element can be added only at the tail, but can be removed from anywhere.
*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Buffer size */
#define BUFFER_LENGTH           10


/* The data organized in structure */
typedef struct 
{
    int idx;
    int data;
    data_t *next;
    data_t *prev;
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

    if (rc == RC_DLLBUF_OK)
    {
        /* Element will be added to tail always */
        if (dll_buf_ctrl.alloc_count == 0)
        {
            /* Queue is empty */
            memset ((void *)dll_buf_ctrl.tail, 0u, sizeof(element));
            memcpy ((void *)dll_buf_ctrl.tail, (void *)element, sizeof(element));
        }
        else
        {
            /* There are elements present in queue */
            dll_buf_ctrl.tail->prev = dll_buf_ctrl.tail;
            dll_buf_ctrl.tail = dll_buf_ctrl.tail->next;

            memset ((void *)dll_buf_ctrl.tail, 0u, sizeof(element));
            memcpy ((void *)dll_buf_ctrl.tail, (void *)element, sizeof(element));

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
        if (conv_remove)
        {
            /* Remove the element from the head in case of conventional remove */
            memcpy ((void *)element, (void *)dll_buf_ctrl.head, sizeof(element));
            if (dll_buf_ctrl.alloc_count > 1)
            {
                /* Store the head that is removed for later operations */
                data_t *cur_element = dll_buf_ctrl.head;
                /* Move the current head */
                dll_buf_ctrl.head = dll_buf_ctrl.head->next;
                /* De-link the previous pointer as the element is removed */
                dll_buf_ctrl.head->prev = NULL;
                /* The current element is the end of queue so de-link the next */
                cur_element->next = NULL;
                /* Assign next of end to the current element */
                dll_buf_ctrl.end->next = cur_element;
                /* Move the end to the last removed element */
                dll_buf_ctrl.end = cur_element;
            }
        }
        else
        {
            data_t *traverse_var = dll_buf_ctrl.head;

            if (dll_buf_ctrl.alloc_count == 1)
            {
                if (traverse_var->idx == idx)
                {
                    memcpy ((void *)element, (void *)traverse_var, sizeof(element));
                }
                else
                {
                    rc = RC_DLLBUF_NOT_FOUND;
                }
            }
            else
            {
                while (traverse_var != dll_buf_ctrl.tail)
                {
                    if (traverse_var->idx == idx)
                    {
                        /* Remove the element from the middle in case of conditional remove */
                        memcpy ((void *)element, (void *)traverse_var, sizeof(element));
                        if ((traverse_var != dll_buf_ctrl.head) && (traverse_var != dll_buf_ctrl.tail))
                        {
                            traverse_var->prev = traverse_var->next = NULL;
                            dll_buf_ctrl.end->next = traverse_var;
                            dll_buf_ctrl.end = traverse_var;
                        }
                    }
                    else
                    {
                        traverse_var = traverse_var->next;
                    }
                }
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
        
    }
    return rc;
}

/* Initialize the LIFO buffer */
void lifo_init (void)
{
    /* Set the buffer size */
    dll_buf_ctrl.length = BUFFER_LENGTH;
    /* Initialize the allocated count */
    dll_buf_ctrl.alloc_count = 0;
    /* Make the base of the buffer point to the 0th element of the array */
    dll_buf_ctrl.base = &data[0];
    /* Make the head and tail point to the base as the buffer is empty */
    dll_buf_ctrl.tail = dll_buf_ctrl.head = dll_buf_ctrl.base;

    for (int i = 0; i < (dll_buf_ctrl.length - 1); i++)
    {
        /* Link all the nodes in the queue */
        data[i].next = &data[i + 1];
    }
    /* Initialize the end of the queue */
    dll_buf_ctrl.end = &data[dll_buf_ctrl.length - 1];
    dll_buf_ctrl.end->next = NULL;
}

/* De-Initialize the LIFO buffer */
void lifo_deInit (void)
{
    /* Set the buffer size */
    dll_buf_ctrl.length = 0;
    /* Make the base of the buffer point to NULL */
    dll_buf_ctrl.base = NULL;
    /* Make the head and tail point to the NULL */
    dll_buf_ctrl.tail = dll_buf_ctrl.head = dll_buf_ctrl.base;
}

int main()
{
    printf("\nLIFO Buffer Implementation. Length of buffer: %d", BUFFER_LENGTH);
    char symbol;
    data_t element;
    lifo_rc_t rc;
    
    /* Initialize the LIFO buffer */
    lifo_init();

    printf("\nEnter 1 to push, 2 to pop, 3 to traverse, and 4 to exit: ");

    /* 1 to push, 2 to pop, 3 to exit */
    while (symbol != '4')
    {
        scanf("%c", &symbol);

        switch (symbol)
        {
            case '1': 
            {
                /* Get the input and push if the buffer is not full */
                printf("\nEnter the element to be pushed: \nData 1: ");
                scanf("%d", &element.data_1);
                printf("Data 2: ");
                scanf("%d", &element.data_2);
                rc = lifo_push(&element);
                debug_lifo_pointer();

                if (rc == RC_LBUF_OK) 
                {
                    printf("\nData pushed successfully.\n");
                }
                else 
                {
                    printf("\nError - buffer full. Pop an element and try again.\n");
                }
            }
            break;
            case '2':
            {
                /* Pop if the buffer is not empty  */
                rc = lifo_pop(&element);
                debug_lifo_pointer();

                if (rc == RC_LBUF_OK) 
                {
                    printf("\nElement popped successfully.");
                    printf("\nData 1: %d, Data 2: %d\n", element.data_1, element.data_2);
                }
                else 
                {
                    printf("\nError - buffer empty. Push an element and try again.\n");
                }
            }
            break;
            case '3':
            {
                /* Traverse the LIFO buffer */
                rc = lifo_traverse();
                
                if (rc != RC_LBUF_OK)
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
    lifo_deInit();
    printf("\nExited program");
    return 0;
}