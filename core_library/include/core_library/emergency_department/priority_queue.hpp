#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include "core_library/emergency_department/emergency_case.hpp"
#include <iostream>

/*
 * PRIORITY QUEUE - MIN-HEAP IMPLEMENTATION
 * 
 * Concept: Binary Min-Heap stored in array
 * - Root = highest priority (minimum value)
 * - Complete binary tree property
 * - Parent always has higher priority than children
 * 
 * Array representation:
 * - Parent(i) = (i-1)/2
 * - Left child(i) = 2*i + 1
 * - Right child(i) = 2*i + 2
 * 
 * Time Complexity:
 * - Insert: O(log n)
 * - Remove: O(log n)
 * - Peek: O(1)
 */

class PriorityQueue
{
private:
    static const int MAX_SIZE = 100;  // Maximum capacity
    EmergencyCase heap[MAX_SIZE];     // Array to store heap
    int size;                          // Current number of elements
    
    // Helper functions for heap operations
    
    // Get parent index
    int parent(int i) const { return (i - 1) / 2; }
    
    // Get left child index
    int leftChild(int i) const { return 2 * i + 1; }
    
    // Get right child index
    int rightChild(int i) const { return 2 * i + 2; }
    
    // Swap two elements
    void swap(int i, int j);
    
    // Move element up to maintain heap property (after insertion)
    void heapifyUp(int index);
    
    // Move element down to maintain heap property (after removal)
    void heapifyDown(int index);

public:
    // Constructor
    PriorityQueue();
    
    // Check if queue is empty
    bool isEmpty() const;
    
    // Check if queue is full
    bool isFull() const;
    
    // Get current size
    int getSize() const;
    
    // Insert emergency case (enqueue with priority)
    bool enqueue(const EmergencyCase& emergencyCase);
    
    // Remove highest priority case (dequeue)
    bool dequeue(EmergencyCase& emergencyCase);
    
    // View highest priority case without removing
    bool peek(EmergencyCase& emergencyCase) const;
    
    // Display all cases in priority order (without modifying heap)
    void display() const;
    
    // Update waiting times for all cases
    void updateWaitingTimes();
    
    // Get all cases as array (for file saving)
    void getAllCases(EmergencyCase cases[], int& count) const;
    
    // Clear all cases
    void clear();
    
    // Search for a case by ID
    bool findCase(const std::string& caseID, EmergencyCase& foundCase) const;
};

#endif