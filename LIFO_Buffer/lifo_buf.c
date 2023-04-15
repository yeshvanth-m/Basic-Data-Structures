/* A basic implementation of LIFO buffer */
#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH           4

/* The data organized in structure */
typedef struct {
    int data_1;
    int data_2;
} data_t;

/* Static allocation of data is preffered */
data_t data[BUFFER_LENGTH];

/* LIFO buffer structure declaration */
typedef struct {
    int length;
    data_t *base;
    data_t *head;
} lifo_buf_t;

lifo_buf_t lifo_buf_ctrl;

/* LIFO buffer return code */
typedef enum {
    RC_LBUF_OK,
    RC_LBUF_ERR_FULL,
    RC_LBUF_ERR_EMPTY,
} lifo_rc_t;

/* Function to check if the LIFO buffer is empty */
lifo_rc_t lifo_is_bufEmpty (void)
{
    lifo_rc_t rc;

    if (lifo_buf_ctrl.head == lifo_buf_ctrl.base) {
        rc = RC_LBUF_ERR_EMPTY;
    }
    else {
        rc = RC_LBUF_OK;
    }
    return rc;
}

/* Function to check if the LIFO buffer is full */
lifo_rc_t lifo_is_bufFull (void)
{
    lifo_rc_t rc;

    if (lifo_buf_ctrl.head >= (lifo_buf_ctrl.base + lifo_buf_ctrl.length)) {
        rc = RC_LBUF_ERR_FULL;
    }
    else {
        rc = RC_LBUF_OK;
    }
    return rc;
}

/* Funtion to push an element into the LIFO buffer */
lifo_rc_t lifo_push (data_t *element)
{
    lifo_rc_t rc = lifo_is_bufFull();

    if (rc == RC_LBUF_OK) {
        (void) memset ((void *)lifo_buf_ctrl.head, 0u, sizeof(element));
        (void) memcpy ((void *)lifo_buf_ctrl.head, (void *)element, sizeof(element));
        lifo_buf_ctrl.head += 1;
    }
    return rc;
}

/* Function to pop and element from the LIFO buffer */
lifo_rc_t lifo_pop (data_t *element)
{
    lifo_rc_t rc = lifo_is_bufEmpty();

    if (rc == RC_LBUF_OK) {
        (void) memcpy ((void *)element, (void *)(lifo_buf_ctrl.head - 1), sizeof(element));
        lifo_buf_ctrl.head -= 1;
    }
    return rc;
}

/* Initialize the LIFO buffer */
void lifo_init (void)
{
    lifo_buf_ctrl.length = BUFFER_LENGTH;
    lifo_buf_ctrl.base = &data[0];
    lifo_buf_ctrl.head = lifo_buf_ctrl.base;
}

int main()
{
    printf("\nLIFO Buffer Implementation. Length of buffer: %d", BUFFER_LENGTH);
    char symbol;
    data_t element;
    int scan_element;
    lifo_rc_t rc;

    lifo_init();

    while (1)
    {
        printf("\nEnter 1 for push, 2 for pop, 3 for exit: ");
        scanf("%c", &symbol);

        if (symbol == '1') {
            printf("\nEnter the elements to be pushed: \nElement 1: ");
            scanf("%d", &element.data_1);
            printf("Element 2: ");
            scanf("%d", &element.data_2);
            rc = lifo_push(&element);

            if (rc == RC_LBUF_OK) {
                printf("\nElement pushed successfully.");
            }
            else {
                printf("\nError - buffer full. Pop an element and try again.");
            }
        }
        else if (symbol == '2') {
            rc = lifo_pop(&element);
            
            if (rc == RC_LBUF_OK) {
                printf("\nElement popped successfully.");
                printf("\nElement 1: %d, Element 2: %d", element.data_1, element.data_2);
            }
            else {
                printf("\nError - buffer empty. Push an element and try again.");
            }
        }
        else if (symbol == '3') {
            break;
        }
        else {
            printf("\nIncorrect choice, try again.");
        }
    }

    printf("\nExited program");
    return 0;
}