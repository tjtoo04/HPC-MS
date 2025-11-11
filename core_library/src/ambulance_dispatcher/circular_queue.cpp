#include "core_library/circular_queue.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

// ANSI color codes
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

// Constructor
CircularQueue::CircularQueue() : rear(nullptr), size(0) {}

// Destructor
CircularQueue::~CircularQueue()
{
    if (isEmpty())
        return;

    Node *front = rear->next;
    Node *current = front;

    // Traverse and delete all nodes
    do
    {
        Node *temp = current;
        current = current->next;
        delete temp;
    } while (current != front);

    rear = nullptr;
    size = 0;
}

// Check queue empty
bool CircularQueue::isEmpty() const
{
    return rear == nullptr;
}

// Current size
int CircularQueue::getSize() const
{
    return size;
}

// Enqueue: Add an ambulance to the rear of the queue
void CircularQueue::enqueue(const Ambulance &ambulance)
{
    Node *newNode = new Node(ambulance);

    if (isEmpty())
    {
        rear = newNode;
        rear->next = rear;
    }
    else
    {
        newNode->next = rear->next;
        rear->next = newNode;
        rear = newNode;
    }

    size++;
}

// Dequeue: Remove the front ambulance from the queue
bool CircularQueue::dequeue(Ambulance &ambulance)
{
    if (isEmpty())
    {
        cout << RED << "\n[ERROR] No ambulances in the queue!" << RESET << endl;
        return false;
    }

    Node *front = rear->next;
    ambulance = front->data;

    // Only one node
    if (rear == front)
    {
        delete front;
        rear = nullptr;
    }
    else
    {
        // More than one node
        rear->next = front->next;
        delete front;
    }

    size--;
    return true;
}

// Shift rotation: Front ambulance -> back
bool CircularQueue::rotate()
{
    if (isEmpty())
    {
        return false;
    }

    if (size == 1)
    {
        return true;
    }

    // Update curr ambulance status
    Node *front = rear->next;
    front->data.status = "Standby";

    // Rotate
    rear = rear->next;

    // Update front = on duty
    Node *newFront = rear->next;
    newFront->data.status = "On Duty";

    return true;
}

// Display
void CircularQueue::display() const
{
    if (isEmpty())
    {
        cout << YELLOW << "\n⚠ INFO: " << RESET << "No ambulances currently registered in the system." << endl;
        return;
    }

    Node *front = rear->next;
    Node *current = front;
    int position = 1;

    // Traverse the circular queue and display
    do
    {
        if (position == 1)
        {
            cout << GREEN << "► [ON DUTY] " << RESET;
        }
        else
        {
            cout << "  [" << position << "]      ";
        }
        current->data.display();
        current = current->next;
        position++;
    } while (current != front);
}

// Get front ambulance without removing it
bool CircularQueue::getFront(Ambulance &ambulance) const
{
    if (isEmpty())
    {
        return false;
    }

    ambulance = rear->next->data;
    return true;
}

// Update front ambulance's data
bool CircularQueue::updateFront(const Ambulance &ambulance)
{
    if (isEmpty())
    {
        return false;
    }

    rear->next->data = ambulance;
    return true;
}

// Update rear ambulance's data
bool CircularQueue::updateRear(const Ambulance &ambulance)
{
    if (isEmpty())
    {
        return false;
    }
    
    rear->data = ambulance;
    return true;
}

// Get all ambulances for file saving
void CircularQueue::getAllAmbulances(Ambulance ambulances[], int &count) const
{
    if (isEmpty())
    {
        count = 0;
        return;
    }

    Node *front = rear->next;
    Node *current = front;
    count = 0;

    do
    {
        ambulances[count++] = current->data;
        current = current->next;
    } while (current != front);
}