#include "core_library/emergency_department/priority_queue.hpp"
#include <iostream>
#include <iomanip>

// ANSI colors
#define C_RESET "\033[0m"
#define C_RED "\033[31m"
#define C_GREEN "\033[32m"
#define C_YELLOW "\033[33m"
#define C_CYAN "\033[36m"
#define C_BOLD "\033[1m"

// Constructor
PriorityQueue::PriorityQueue() : size(0) {}

// Check if empty
bool PriorityQueue::isEmpty() const
{
    return size == 0;
}

// Check if full
bool PriorityQueue::isFull() const
{
    return size >= MAX_SIZE;
}

// Get current size
int PriorityQueue::getSize() const
{
    return size;
}

// Swap two elements in heap
void PriorityQueue::swap(int i, int j)
{
    EmergencyCase temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

/*
 * HEAPIFY UP (Bubble Up)
 * Called after insertion to maintain min-heap property
 * 
 * Process:
 * 1. Compare new element with parent
 * 2. If new element has higher priority (lower value), swap with parent
 * 3. Repeat until heap property satisfied or reach root
 * 
 * Example:
 *        5              3  (after heapifyUp)
 *       / \            / \
 *      7   6    →     5   6
 *     /              /
 *    3              7
 */
void PriorityQueue::heapifyUp(int index)
{
    // If at root, stop
    if (index == 0)
        return;
    
    int parentIndex = parent(index);
    
    // If current node has higher priority than parent, swap
    // (remember: lower priority score = higher priority)
    if (heap[index] < heap[parentIndex])
    {
        swap(index, parentIndex);
        heapifyUp(parentIndex);  // Recursively check parent
    }
}

/*
 * HEAPIFY DOWN (Bubble Down)
 * Called after removal to maintain min-heap property
 * 
 * Process:
 * 1. Compare root with its children
 * 2. Find the child with highest priority (smallest value)
 * 3. If child has higher priority, swap with root
 * 4. Repeat for the subtree until heap property satisfied
 * 
 * Example:
 *        7              3  (after heapifyDown)
 *       / \            / \
 *      3   6    →     7   6
 *     /              /
 *    5              5
 */
void PriorityQueue::heapifyDown(int index)
{
    int left = leftChild(index);
    int right = rightChild(index);
    int highest = index;  // Index of highest priority (lowest value)
    
    // Check if left child has higher priority
    if (left < size && heap[left] < heap[highest])
        highest = left;
    
    // Check if right child has higher priority
    if (right < size && heap[right] < heap[highest])
        highest = right;
    
    // If highest priority is not current node, swap and continue
    if (highest != index)
    {
        swap(index, highest);
        heapifyDown(highest);  // Recursively fix subtree
    }
}

/*
 * ENQUEUE (Insert)
 * Add new emergency case with priority
 * 
 * Algorithm:
 * 1. Add element at end of array (maintain complete tree)
 * 2. Heapify up to restore heap property
 * 
 * Time Complexity: O(log n)
 */
bool PriorityQueue::enqueue(const EmergencyCase& emergencyCase)
{
    if (isFull())
    {
        std::cout << C_RED << "\n✗ ERROR: Queue is full! Cannot add more cases." 
                  << C_RESET << std::endl;
        return false;
    }
    
    // Insert at end
    heap[size] = emergencyCase;
    
    // Restore heap property
    heapifyUp(size);
    
    size++;
    return true;
}

/*
 * DEQUEUE (Remove highest priority)
 * Remove and return the root (highest priority case)
 * 
 * Algorithm:
 * 1. Save root element (highest priority)
 * 2. Replace root with last element
 * 3. Reduce size
 * 4. Heapify down to restore heap property
 * 
 * Time Complexity: O(log n)
 */
bool PriorityQueue::dequeue(EmergencyCase& emergencyCase)
{
    if (isEmpty())
    {
        std::cout << C_RED << "\n✗ ERROR: No cases in queue!" 
                  << C_RESET << std::endl;
        return false;
    }
    
    // Get root (highest priority)
    emergencyCase = heap[0];
    
    // Move last element to root
    heap[0] = heap[size - 1];
    size--;
    
    // Restore heap property if queue not empty
    if (size > 0)
        heapifyDown(0);
    
    return true;
}

/*
 * PEEK (View highest priority without removing)
 * Return root element without modifying heap
 * 
 * Time Complexity: O(1)
 */
bool PriorityQueue::peek(EmergencyCase& emergencyCase) const
{
    if (isEmpty())
    {
        std::cout << C_YELLOW << "\n⚠ INFO: No cases in queue." 
                  << C_RESET << std::endl;
        return false;
    }
    
    emergencyCase = heap[0];
    return true;
}

/*
 * DISPLAY ALL CASES
 * Show all cases in priority order
 * 
 * Method: Create temporary copy and repeatedly extract highest priority
 * This preserves original heap structure
 */
void PriorityQueue::display() const
{
    if (isEmpty())
    {
        std::cout << C_YELLOW << "\n⚠ No pending emergency cases." 
                  << C_RESET << std::endl;
        return;
    }
    
    std::cout << "\n" << C_CYAN << std::string(60, '=') << C_RESET << std::endl;
    std::cout << C_BOLD << C_CYAN << "   PENDING EMERGENCY CASES (Priority Order)" 
              << C_RESET << std::endl;
    std::cout << C_CYAN << std::string(60, '=') << C_RESET << std::endl;
    
    // Create temporary queue for display
    PriorityQueue tempQueue;
    for (int i = 0; i < size; i++)
    {
        tempQueue.enqueue(heap[i]);
    }
    
    int position = 1;
    EmergencyCase currentCase;
    
    while (!tempQueue.isEmpty())
    {
        tempQueue.dequeue(currentCase);
        
        std::cout << "\n" << C_BOLD << "Priority #" << position++ 
                  << C_RESET << std::endl;
        currentCase.display();
    }
    
    std::cout << "\n" << C_CYAN << std::string(60, '=') << C_RESET << std::endl;
    std::cout << C_BOLD << "Total Cases: " << size << C_RESET << std::endl;
    std::cout << C_CYAN << std::string(60, '=') << C_RESET << std::endl;
}

// Update waiting times for all cases
void PriorityQueue::updateWaitingTimes()
{
    for (int i = 0; i < size; i++)
    {
        heap[i].waitingMinutes = calculateWaitingTime(
            heap[i].arrivalTime, 
            heap[i].arrivalDate
        );
    }
    
    // After updating waiting times, priorities may change
    // Rebuild heap to ensure correct priority order
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        heapifyDown(i);
    }
}

// Get all cases as array
void PriorityQueue::getAllCases(EmergencyCase cases[], int& count) const
{
    count = size;
    for (int i = 0; i < size; i++)
    {
        cases[i] = heap[i];
    }
}

// Clear all cases
void PriorityQueue::clear()
{
    size = 0;
}

// Search for case by ID
bool PriorityQueue::findCase(const std::string& caseID, EmergencyCase& foundCase) const
{
    for (int i = 0; i < size; i++)
    {
        if (heap[i].caseID == caseID)
        {
            foundCase = heap[i];
            return true;
        }
    }
    return false;
}