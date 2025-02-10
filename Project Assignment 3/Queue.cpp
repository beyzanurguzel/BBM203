#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    // TODO: Your code here
    front = -1;   // starting point of the queue
    rear = -1;   // end point of the queue
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // TODO: Your code here
    // Implement circular structure 
    // Add the province

    // Check if the queue is full
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        std::cerr << "Error: Queue overflow" << std::endl;
        return;
    }
    if (isEmpty()) {
        front = rear = 0;
    } else {
        rear = (rear + 1) % MAX_QUEUE_SIZE;
    }
    data[rear] = province;
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    // TODO: Your code here
    int province;
    if(isEmpty()) {
        std::cerr << "Queue is empty! " << std::endl;
        return -1; // Return an invalid value for error
    }
    if (front == rear) {
        province = data[front];
        front = rear = -1;
    } else {
        province = data[front];
        front = (front + 1) % MAX_QUEUE_SIZE;
    }
    return province;
}

// Returns the front province without removing it
int Queue::peek() const {
    // TODO: Your code here
    if (isEmpty()) {
        std::cerr << "Queue is empty! Nothing to peek." << std::endl;
        return -1; // Return an invalid value for error
    }
    return data[front];
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    // TODO: Your code here
    return (front == -1 && rear == -1);
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    // TODO: Your code here
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        std::cerr << "Error: Queue overflow" << std::endl;
        return;
    }

    if (front == -1) { // If the queue is initially empty
        front = 0;
        rear = 0;
        data[front] = province;
        return;
    }
    // Adjust the front to insert at the beginning of the queue
    front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    data[front] = province;
}

