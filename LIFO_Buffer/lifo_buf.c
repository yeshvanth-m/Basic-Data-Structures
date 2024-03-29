/* A basic implementation of LIFO buffer */
#include <stdio.h>
#include <string.h>

/* Buffer size */
#define BUFFER_LENGTH           4

/* Basic LIFO buffer implementation: 
    Empty state,  One element added,     Two more elements added, One element removed,  Buffer Full
     ______          ______                   ______                 ______              ______   
    |______|        |______|                 |______|               |______|            |_DATA_| <- HEAD
    |______|        |______|                 |_DATA_| <- HEAD       |______|            |_DATA_|
    |______|        |______|                 |_DATA_|               |_DATA_| <- HEAD    |_DATA_|
    |______|        |_DATA_| <- HEAD,BASE    |_DATA_| <- BASE       |_DATA_| <- BASE    |_DATA_| <- BASE

*/


/* The data organized in structure */
typedef struct 
{
    int data_1;
    int data_2;
} data_t;

/* Static allocation of data is preferred */
data_t data[BUFFER_LENGTH];

/* LIFO buffer structure declaration */
typedef struct 
{
    int length;
    data_t *base;
    data_t *head;
} lifo_buf_t;

lifo_buf_t lifo_buf_ctrl;

/* LIFO buffer return code */
typedef enum 
{
    RC_LBUF_OK,
    RC_LBUF_ERR_FULL,
    RC_LBUF_ERR_EMPTY,
} lifo_rc_t;

void debug_lifo_pointer(void)
{
    printf("\nHead: %d\n", lifo_buf_ctrl.head - lifo_buf_ctrl.base);
}

/* Function to check if the LIFO buffer is empty */
lifo_rc_t lifo_is_bufEmpty (void)
{
    lifo_rc_t rc = RC_LBUF_OK;

    /* If the head is pointing to base, then the buffer is empty */
    if (lifo_buf_ctrl.head == lifo_buf_ctrl.base) 
    {
        rc = RC_LBUF_ERR_EMPTY;
    }
    return rc;
}

/* Function to check if the LIFO buffer is full */
lifo_rc_t lifo_is_bufFull (void)
{
    lifo_rc_t rc = RC_LBUF_OK;

    /* If the head is greater than or equal to the base + length then buffer is full */
    if (lifo_buf_ctrl.head >= (lifo_buf_ctrl.base + lifo_buf_ctrl.length)) 
    {
        rc = RC_LBUF_ERR_FULL;
    }
    return rc;
}

/* Funtion to push an element into the LIFO buffer */
lifo_rc_t lifo_push (data_t *element)
{
    /* Check if the buffer is full */
    lifo_rc_t rc = lifo_is_bufFull();

    if (rc == RC_LBUF_OK) 
    {
        /* Buffer is not full, then copy the element into the buffer */
        (void) memset ((void *)lifo_buf_ctrl.head, 0u, sizeof(element));
        (void) memcpy ((void *)lifo_buf_ctrl.head, (void *)element, sizeof(element));
        /* Make the head point to the next element */
        lifo_buf_ctrl.head++;
    }
    return rc;
}

/* Function to pop and element from the LIFO buffer */
lifo_rc_t lifo_pop (data_t *element)
{
    /* Check if the buffer is empty */
    lifo_rc_t rc = lifo_is_bufEmpty();

    if (rc == RC_LBUF_OK) 
    {
        /* Buffer is not empty then remove the element from the buffer */
        (void) memcpy ((void *)element, (void *)(lifo_buf_ctrl.head - 1), sizeof(element));
        /* Make the head point to the previous element */
        lifo_buf_ctrl.head--;
    }
    return rc;
}

lifo_rc_t lifo_traverse (void)
{
    /* Check if the buffer is empty */
    lifo_rc_t rc = lifo_is_bufEmpty();

    if (rc == RC_LBUF_OK) 
    {
        /* To count the number of elements */
        int element_number = lifo_buf_ctrl.length;
        /* Get the head pointer */
        data_t *var = lifo_buf_ctrl.head; 
        do
        {
            /* Traverse through the LIFO buffer till hitting the base */
            var--;
            printf ("Element %d: Data A: %d, Data B: %d\n", element_number--, var->data_1, var->data_2);
        } while(var != lifo_buf_ctrl.base);
    }
    return rc;
}

/* Initialize the LIFO buffer */
void lifo_init (void)
{
    /* Set the buffer size */
    lifo_buf_ctrl.length = BUFFER_LENGTH;
    /* Make the base of the buffer point to the 0th element of the array */
    lifo_buf_ctrl.base = &data[0];
    /* Make the head point to the base as the buffer is empty */
    lifo_buf_ctrl.head = lifo_buf_ctrl.base;
}

/* De-Initialize the LIFO buffer */
void lifo_deInit (void)
{
    /* Set the buffer size */
    lifo_buf_ctrl.length = 0;
    /* Make the base of the buffer point to NULL */
    lifo_buf_ctrl.base = NULL;
    /* Make the head point to the NULL */
    lifo_buf_ctrl.head = NULL;
}

int main()
{
    printf("\nLIFO Buffer Implementation. Length of buffer: %d", BUFFER_LENGTH);
    char symbol;
    data_t element;
    lifo_rc_t rc;
    
    /* Initialize the LIFO buffer */
    lifo_init();

    /* 1 to push, 2 to pop, 3 to exit */
    while (symbol != '4')
    {
        printf("\nEnter 1 to push, 2 to pop, 3 to traverse, and 4 to exit: ");
        scanf(" %c", &symbol);

        switch (symbol)
        {
            case '1': 
            {
                /* Get the input and push if the buffer is not full */
                printf("\nEnter the element to be pushed: \nData A: ");
                scanf("%d", &element.data_1);
                printf("Data B: ");
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
                    printf("\nData A: %d, Data B: %d\n", element.data_1, element.data_2);
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