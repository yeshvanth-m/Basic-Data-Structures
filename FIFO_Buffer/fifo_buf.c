/* A basic implementation of a never full FIFO Circular buffer */
#include <stdio.h>
#include <string.h>

/* Buffer size */
#define BUFFER_LENGTH           4

/* The data organized in structure */
typedef struct {
    int data_1;
    int data_2;
} data_t;

/* Static allocation of data is preferred */
data_t data[BUFFER_LENGTH];

/* FIFO buffer structure declaration */
typedef struct {
    int length;
    int count;
    data_t *base;
    data_t *head;
    data_t *tail;
} fifo_buf_t;

fifo_buf_t fifo_buf_ctrl;

/* FIFO buffer return code */
typedef enum {
    RC_FBUF_OK,
    RC_FBUF_ERR_EMPTY,
} fifo_rc_t;

void debug_fifo_pointer(void)
{
    printf("\nHead: %d, Tail: %d, Count: %d\n", fifo_buf_ctrl.head - fifo_buf_ctrl.base,
                        fifo_buf_ctrl.tail - fifo_buf_ctrl.base, fifo_buf_ctrl.count);
}

/* Function to move a pointer in circular buffer */
void fifo_increment_pointer (data_t** pointer)
{
    /* Increment the pointer */
    (*pointer)++;

    /*Check bounds, if the pointer has reached the end, then point it to the base */
    if (*pointer == (fifo_buf_ctrl.base + fifo_buf_ctrl.length)) {
        *pointer = fifo_buf_ctrl.base;
    }
}

/* Function to check if the FIFO buffer is empty */
fifo_rc_t fifo_is_bufEmpty (void)
{
    fifo_rc_t rc;

    /* If the count is 0, then the buffer is empty */
    if (fifo_buf_ctrl.count == 0) {
        rc = RC_FBUF_ERR_EMPTY;
    }
    else {
        rc = RC_FBUF_OK;
    }
    return rc;
}

/* Funtion to add an element into the FIFO buffer */
void fifo_add (data_t *element)
{
    /* If the count is full, then move the tail for overwriting */
    if (fifo_buf_ctrl.count == fifo_buf_ctrl.length) {
        fifo_increment_pointer(&fifo_buf_ctrl.tail);
    }

    /* Increment the count only if the buffer is not full */
    if (fifo_buf_ctrl.count < fifo_buf_ctrl.length) {
        fifo_buf_ctrl.count++;
    }

    /* Add the element to the head of the buffer */
    (void) memset ((void *)fifo_buf_ctrl.head, 0u, sizeof(element));
    (void) memcpy ((void *)fifo_buf_ctrl.head, (void *)element, sizeof(element));

    /* Increment the pointer of circular buffer  */
    fifo_increment_pointer(&fifo_buf_ctrl.head);
}

/* Function to remove an element from the FIFO buffer */
fifo_rc_t fifo_remove (data_t *element)
{
    /* Check if the buffer is empty */
    fifo_rc_t rc = fifo_is_bufEmpty();

    if (rc == RC_FBUF_OK) {
        /* Decrement the count on removing an element if the buffer is not empty */
        fifo_buf_ctrl.count--;
        (void) memcpy ((void *)element, (void *)fifo_buf_ctrl.tail, sizeof(element));

        /* Move the tail after removing an element from the tail */
        fifo_increment_pointer(&fifo_buf_ctrl.tail);
    }
    return rc;
}

/* Initialize the FIFO buffer */
void fifo_init (void)
{
    /* Set the buffer size */
    fifo_buf_ctrl.length = BUFFER_LENGTH;
    /* Set the count */
    fifo_buf_ctrl.count = 0;
    /* Make the base of the buffer point to the 0th element of the array */
    fifo_buf_ctrl.base = &data[0];
    /* Make the head point to the base as the buffer is empty */
    fifo_buf_ctrl.tail = fifo_buf_ctrl.head = fifo_buf_ctrl.base;
}

/* De-Initialize the FIFO buffer */
void fifo_deInit (void)
{
    /* Set the buffer size */
    fifo_buf_ctrl.length = 0;
    /* Set the count */
    fifo_buf_ctrl.count = 0;
    /* Make the base of the buffer point to the 0th element of the array */
    fifo_buf_ctrl.base = NULL;
    /* Make the head point to the base as the buffer is empty */
    fifo_buf_ctrl.tail = fifo_buf_ctrl.head = fifo_buf_ctrl.base;
}

int main()
{
    printf("\nCircular FIFO Buffer Implementation. Length of buffer: %d", BUFFER_LENGTH);
    char symbol;
    data_t element;
    fifo_rc_t rc;

    /* Initialize the FIFO circular buffer */
    fifo_init();

    printf("\nEnter 1 to add, 2 to remove, 3 to exit: ");

    /* 1 to add, 2 to remove, 3 to exit */
    while (1)
    {
        scanf("%c", &symbol);

        if (symbol == '1') {
            /* Add the element to the buffer as its an unlimited buffer */
            printf("\nEnter the data to be added: \nData 1: ");
            scanf("%d", &element.data_1);
            printf("Data 2: ");
            scanf("%d", &element.data_2);
            fifo_add(&element);

            printf("\nElement added successfully.\n");

            debug_fifo_pointer();
        }
        else if (symbol == '2') {
            /* Remove the element buffer if not empty */
            rc = fifo_remove(&element);
            
            if (rc == RC_FBUF_OK) {
                printf("\nElement removed successfully.");
                printf("\nData 1: %d, Data 2: %d\n", element.data_1, element.data_2);
            }
            else {
                printf("\nError - buffer empty. Add an element and try again.\n");
            }

            debug_fifo_pointer();
        }
        else if (symbol == '3') {
            break;
        }
    }

    /* De-initialize the buffer */
    fifo_deInit();
    printf("\nExited program");
    return 0;
}