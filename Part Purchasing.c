#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Part {
    int count; // The number of this part available
    int extra_part; // The additional part that comes when purchasing
    int price; // The price of purchasing the part
} Part;

typedef struct Component {
    int fixed; // Is the component known to the repair crew
    int known; // If the compenent has been queued before
    int part_req; // The part required to fix
    int num_comp_rev; // Number of components revealed when fixed
    int * comp_rev; // Array of revealed components when fixed
} Component;

typedef struct Node {
    struct Node * next;
    int data;
} Node;

typedef struct Queue {
    Node * tail;
} Queue;

// Linked List Prototypes
Node * createNode(int data);
Node * insertTail(Node * tail, int data);
Node * removeHead(Node * tail);

// Queue Prototypes
Queue * createQueue();
void deleteQueue(Queue * q);
void enqueue(Queue * q, int data);
void dequeue(Queue * q);
int front(Queue * q);
int isEmpty(Queue * q);

int main() {
    // Get number of parts
    int numParts;
    scanf("%d", &numParts);

    // Create an array for number of parts
    Part * parts = (Part *)malloc(numParts * sizeof(Part));

    // Handle input of parts
    for(int i = 0; i < numParts; i++) {
        scanf("%d %d", &parts[i].price, &parts[i].extra_part);
        parts[i].count = 0;
    }

    // Get number of components
    int numComponents;
    scanf("%d", &numComponents);

    // Create an array for number of components
    Component * components = (Component *)malloc(numComponents * sizeof(Component));

    // Handle input for components
    for(int i = 0; i < numComponents; i++) {
        scanf("%d %d", &components[i].part_req, &components[i].num_comp_rev);

        // Get number of components revealed
        components[i].comp_rev = (int *)malloc(numComponents * sizeof(int));

        // Store the components revealed
        for(int j = 0; j < components[i].num_comp_rev; j++)
            scanf("%d", &components[i].comp_rev[j]);

        // Set component to not fixed
        components[i].fixed = 0;

        // Set component to unknown
        components[i].known = 0;
    }

    // Create needed queue
    Queue * fixQueue = createQueue();

    // Track if ship is fully fixed
    int shipFixed = 0;

    // Track cost of repairs
    int totalCost = 0;

    // Check ship until fixed
    while(!shipFixed) {
        // Set ship to fixed, this will be changed
        // later if the ship isn't actually fixed
        shipFixed = 1;
        
        // Check if fixes are in queue
        if(!isEmpty(fixQueue)) {
            // Set ship to broken
            shipFixed = 0;
            
            // Keep working through queue until
            // all components are fixed
            while(!isEmpty(fixQueue)) {
                // Get component at front of queue
                int frontQueue = front(fixQueue);

                // Check if component is already fixed
                if(components[frontQueue].fixed) {
                    // Remove component from queue
                    dequeue(fixQueue);

                    // Continue checks
                    continue;
                }

                // Check if part needs to be purchased
                if(parts[components[frontQueue].part_req - 1].count == 0) {  // Part not found
                    // Purchase part
                    totalCost += parts[components[frontQueue].part_req - 1].price;
                    parts[components[frontQueue].part_req - 1].count++;

                    // Inventory extra part
                    parts[parts[components[frontQueue].part_req - 1].extra_part - 1].count++;

                    // Add component to back of the queue
                    enqueue(fixQueue, frontQueue);

                    // Remove component from front of the queue
                    dequeue(fixQueue);
                } else {    // Part is found
                    // Use that part
                    parts[components[frontQueue].part_req - 1].count--;

                    // Set component as fixed
                    components[frontQueue].fixed = 1;

                    // Queue uncovered repairs needed
                    for(int i = 0; i < components[frontQueue].num_comp_rev; i++) {
                        if(!components[components[frontQueue].comp_rev[i] - 1].known) {   
                            // Only queue unknown components
                            enqueue(fixQueue, components[frontQueue].comp_rev[i] - 1);

                            // Mark component as known
                            components[components[frontQueue].comp_rev[i] - 1].known = 1;
                        }
                    }

                    // Remove component from queue to be fixed
                    dequeue(fixQueue);
                }
            }
        } else {
            // Queue is empty, check components in order
            for(int i = 0; i < numComponents; i++) {
                // Check if part needs to be fixed
                if(!components[i].fixed) {
                    // Set ship to broken
                    shipFixed = 0;

                    // Queue component to be fixed
                    enqueue(fixQueue, i);

                    // Leave for loop
                    break;
                }
            }
        }
    }

    printf("%d\n", totalCost);

    // Free alocated memory
    deleteQueue(fixQueue);
    
    for(int i = 0; i < numComponents; i++)
        free(components[i].comp_rev);

    free(components);
    free(parts);

    return 0;
}

// Linked List Functions
// Function to create a Node
Node * createNode(int data) {
    // Allocation
    Node * res = (Node *) malloc(sizeof(Node));

    // Intialization
    res->data = data;
    res->next = res; // self, because circular
    
    // Return
    return res;
}

// Function to insert a new tail into a linked list
// Returns resulting tail
Node * insertTail(Node * tail, int data) {
    Node * newTail = createNode(data);

    // Empty list case
    if (tail == NULL) {
        return newTail;
    }

    // At least 1 node case
    // The resulting tail's next should point to old head
    newTail->next = tail->next;

    // The new tail should follow after the old tail
    tail->next = newTail;

    // Return the resulting tail
    return newTail;
}

Node * removeHead(Node * tail) {
    // Empty list case
    if (tail == NULL) {
        return NULL;
    }
    
    // Single node list case
    if (tail->next == tail) {
        free(tail);
        return NULL;
    }

    // More than 1 node case
    // Get the new head
    Node * newHead = tail->next->next;

    // Free the oldhead
    free(tail->next);

    // Link the tail to the head
    tail->next = newHead;

    // Return the tail
    return tail;
}

// Queue Prototypes
// Function to create a Queue
Queue * createQueue() {
    // Allocate
    Queue * res = (Queue *) malloc(sizeof(Queue));

    // Initilaize
    res->tail = NULL;

    // Return
    return res;
}

// Function to delete all the memory assoricated with queue
void deleteQueue(Queue * q) {
    // Delete all the values in the queue
    while (!isEmpty(q)) {
        dequeue(q);
    }
    
    // Remove the queue
    free(q);
}

// Function to add some data to the queue
void enqueue(Queue * q, int data) {
    // Use the linked list function to add a node
    q->tail = insertTail(q->tail, data);
}

// Function to remove the front of the queue
void dequeue(Queue * q) {
    // Use the linked list function to remove a node
    q->tail = removeHead(q->tail);
}

// Function to return the element at the front of the queue
int front(Queue * q) {
    // Ensure that there is a value
    assert(!isEmpty(q));

    // Return the value at the head
    return q->tail->next->data;
}

// Function to check that the queue is empty
int isEmpty(Queue * q) {
    return (q->tail == NULL);
}