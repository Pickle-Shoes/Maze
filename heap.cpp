#include "heap.h"

HEAP::HEAP(int size) {
    capacity = size;
    heapSize = 0;
    array = new int[capacity];
}

HEAP::~HEAP() {
    delete[] array;
}

int HEAP::leftChild(int index) {
    return 2*index + 1;
}
int HEAP::rightChild(int index) {
    return 2*index + 2;
}
int HEAP::parent(int index) {
    return (index - 1) / 2;
}

void HEAP::insertH(int data) {
    if (heapSize == capacity) {
        cout << "Heap overflow - can't insert"<< endl;
        return;
    }

    array[heapSize] = data;
    heapifyUp(heapSize);
    heapSize++;
}

void HEAP::heapifyUp(int index) {
    while(index != 0 && array[parent(index)] < array[index]) {
        swap(array[parent(index)], array[index]);
        index = parent(index);
    }
}

int HEAP::peek() {
    if (heapSize <= 0) {
        cout << "Heap is empty" << endl;
        return -1;
    }

    return array[0];
}

void HEAP::deleteMax() {
    if (heapSize <= 0) {
        cout << "Heap is empty - nothing to delete" << endl;
        return;
    }

    if (heapSize == 1) {
        heapSize--;
        return;
    }

    array[0] = array[heapSize - 1];
    heapSize--;

    heapifyDown(0);
}

void HEAP::deleteH(int data) {
    int index = -1;

    // search for data - linear search :( O(n)
    for (int i = 0; i < heapSize; i++) {
        if (array[i] == data) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Element not found - can't delete"  << endl;
        return;
    }

    array[index] = array[heapSize - 1];
    heapSize--;

    heapifyDown(index);
}


void HEAP::heapifyDown(int index) //check for largest value compare both children and swap with the larger one to create a max heap
{

    int largest = index;
    int left = leftChild(index);
    int right = rightChild(index);

    if(right < heapSize && array[right] > array[largest]){  
        largest  = right;
    }
    if(left < heapSize && array[left] > array[largest]){
        largest  = left;    
    }

    if(largest != index){
        swap(array[index], array[largest]);
        heapifyDown(largest); //recusively call to create the max heap.
    }
}

 
void HEAP::buildH(int arr[], int n) 
{
    for(int i = 0; i < n; i++){
        array[i] = arr[i];
    }
    for(int i = n/2 -1; i >= 0; i--){
        heapifyDown(i);
    } 
    heapSize = n;
}

void HEAP::replace(int oldData, int newData)
{
    deleteH(oldData);
    insertH(newData);
}

void HEAP::heapSort(int arr[], int n)
{
      buildH(arr, n); // build a heap

    for(int i = n - 1; i >= 1; i--){ //go through the heap and swap the root with the last node and then heapify down
        swap(array[0], array[i]);
        heapSize--;
        heapifyDown(0);
    }
    
    for(int i = 0; i < n; i++){ //copy array to original 
        arr[i] = array[i];
    }
    heapSize = n; //reset heap size
}

void HEAP::printHeap(int index, int depth) {
    if (index >= heapSize)
        return;

    printHeap(rightChild(index), depth + 1);

    for (int i = 0; i < depth; i++) {
        cout << "  "; // indentation based on depth
    }

    cout << array[index] << endl;

    printHeap(leftChild(index), depth + 1);
}

void HEAP::switchMinMax(){
    if(array[0] > array[1]  && array[0] > array[2]){ //check for max heap
        for(int i = 0; i < heapSize; i++){
            heapifyDownMin(i);
        }
        for(int i = 0; i < heapSize; i++){
            heapifyDownMin(i);
        }
        for(int i = 0; i < heapSize; i++){
            heapifyDownMin(i);
        }
    }
    else if(array[0] < array[1] && array[0] < array[2]){ //check for min heap
        for(int i = 0; i < heapSize; i++){
            heapifyDown(i);
        }
        for(int i = 0; i < heapSize; i++){
            heapifyDown(i);
        }
        
    }
    return;
}

void HEAP::heapifyDownMin(int index){ //make the heap a min heap
    int smallest = index;
    int left = leftChild(index);
    int right = rightChild(index);

     if(left < heapSize && array[left] < array[smallest]){
        smallest = left;    
    }
    if(right < heapSize && array[right] < array[smallest]){  
        smallest = right;
    }

    if(smallest != index){
        swap(array[index], array[smallest]);
        heapifyDownMin(smallest);
    }
}
