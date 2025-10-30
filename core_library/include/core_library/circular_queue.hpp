#ifndef CIRCULARQUEUE_HPP
#define CIRCULARQUEUE_HPP

#include "ambulance.hpp"
#include <iostream>
using namespace std;

struct Node
{
    Ambulance data;
    Node *next;

    Node(Ambulance amb) : data(amb), next(nullptr) {}
};

class CircularQueue
{
private:
    Node *rear;
    int size;

public:
    CircularQueue();

    ~CircularQueue();

    bool isEmpty() const;

    int getSize() const;

    void enqueue(const Ambulance &ambulance);

    bool dequeue(Ambulance &ambulance);

    bool rotate();

    void display() const;

    bool getFront(Ambulance &ambulance) const;

    bool updateFront(const Ambulance &ambulance);

    void getAllAmbulances(Ambulance ambulances[], int &count) const;

    bool findLeastDutyAmbulance(Ambulance &ambulance) const;
};

#endif