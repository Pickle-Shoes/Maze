#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class HEAP 
{
    private:
        int* array;
        int capacity;
        int heapSize;

        int leftChild(int index);
        int rightChild(int index);
        int parent(int index);

        // These are for a Max Heap
        void heapifyUp(int index);
        void heapifyDown(int index); // complete this

        void heapifyDownMin(int index);// Helper function for a Min Heap - only if you do the Extra Credit

    public:
        HEAP(int size);
        ~HEAP();

        void buildH(int arr[], int n); // complete this
        void insertH(int data);
        void deleteMax();
        void deleteH(int data);
        int peek();

        void replace(int oldData, int newData); // complete this
        void heapSort(int arr[], int n); // complete this

        void printHeap(int index, int depth);

        void switchMinMax();  // Extra Credit

        void heapHelper(int timer);

};

#endif