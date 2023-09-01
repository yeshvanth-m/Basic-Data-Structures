# Basic Data Structures
This repository consists of basic data structures such as LIFO Buffer, FIFO Buffer, and Doubly Linked List implemented in C

## LIFO Buffer
### Design
![LIFO Buffer](/Images/LIFO.jpg)

- When the buffer is empty, both the head and base points to the first element in the buffer
- The head moves forward as new elements are added
- The buffer is full when the number of elements is equal to the size of the buffer
- The head moves backwards as elements are removed from the buffer

## Circular FIFO Buffer
### Design
![FIFO Buffer](/Images/FIFO.jpg)

- The implementation demonstrates a never-full circular FIFO buffer, which means the buffer is never full and elements are over-written when the buffer is full
- When the buffer is empty, the base, the head, and the tail point to the first element in the buffer
- The elements are added to the tail, so the tail moves forward as new elements are added
- The elements are removed from the head, so the head moves forward as elements are removed
- The count keeps track of the number of elements present in the buffer
- The tail overwrites into the head when the buffer is full and a new element is added to it

## Doubly Linked List
### Design
- This is a doubly linked list with remove from any position but add only at the tail
#### Initialize (Empty buffer)
![DLL Init](/Images/DLL_Init.jpg)

- The count is set to 0 and the next of each element is pointed to next element in the buffer, except for the end element
- Head and Tail point to the base (first element in buffer) and the End points to the last element in the buffer

#### Add an element
![DLL Add](/Images/DLL_Add.jpg)

- When the buffer is empty, the new element is added to the tail, and both head and tail point to the element added
- The tail moves forward as the next element is added to the buffer, and the prev pointer is updated accordingly
- The tail and point to the same element as the buffer becomes full

#### Remove an element
![DLL Remove 1](/Images/DLL_Remove_1.jpg)

- There are two types of removes - conventional remove and conditional remove. Conventionally, the element is removed from the Head. Conditionally the element can be removed upon matching an index
- If the element is removed from the tail, the tail moves backwards and the end element points to the lastly removed element
- If the element is removed from the middle, then head and tail remain the same, but the links are updated accordingly
- If the element is removed from the head, then the head moves forward

## How to use?
Using GCC: <br>
Compile: <br>
```gcc .\LIFO_Buffer\lifo_buf.c -o .\LIFO_Buffer\lifo_buf.exe``` <br>
Execute: <br>
```.\LIFO_Buffer\lifo_buf.exe```