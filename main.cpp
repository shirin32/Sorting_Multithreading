
#include <iostream>
#include <pthread.h>
#include <time.h>


#define MAX_SIZE 10                 // we have defined maximum size for our array as 10


#define NOS_THREAD 2 //number of threads

using namespace std;

// array of size MAX
int arr[MAX_SIZE];
int part = 0;

// merge function for merging two parts
void merge(int low, int mid, int high)
{
    int* left = new int[mid - low + 1];
    int* right = new int[high - mid];
    
    int n1 = mid - low + 1, n2 = high - mid, i, j;
    
    // storing values in left part
    for (i = 0; i < n1; i++)
        left[i] = arr[i + low];
    
    // storing values in right part
    for (i = 0; i < n2; i++)
        right[i] = arr[i + mid + 1];
    
    int k = low;
    i = j = 0;
    
    // merge left and right in ascending order
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }
    
    // insert remaining values from left
    while (i < n1) {
        arr[k++] = left[i++];
    }
    
    // insert remaining values from right
    while (j < n2) {
        arr[k++] = right[j++];
    }
}



void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);
    
    for (int j = low; j <= high- 1; j++)
    {
        
        if (arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quick_sort(int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
         at right place */
        int pi = partition(low, high);
        
        // Separately sort elements before
        // partition and after partition
        quick_sort(low, pi - 1);
        quick_sort(pi + 1, high);
    }
}

void*  quick_sort(void* arg){
   
    int thread_part = part++;  // which divide into two parts
    
    // Dividing main array into sub parts and calculating high and low
    int low = thread_part * (MAX_SIZE / 2);
    int high = ((thread_part + 1) * (MAX_SIZE / 2)) - 1;
    
    // evaluating pivot element
    if (low < high) {
        int pi = partition(low, high);
        
        // Separately sort elements before
        // partition and after partition
        quick_sort(low, pi - 1);
        quick_sort(pi + 1, high);
        //merge(low, mid, high);
    }
    return 0;
}


int main()
{
    // generating random values in array
    for (int i = 0; i < MAX_SIZE; i++)
        arr[i] = rand() % 50;
    
    cout << "Unsorted Array: ";
    
    for (int i = 0; i < MAX_SIZE; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
    
    clock_t t1, t2; // t1 and t2 for calculating time for Quick sort
    
    t1 = clock();
    pthread_t threads[NOS_THREAD];
    
    
    for (int i = 0; i < NOS_THREAD; i++)
    {
        pthread_create(&threads[i], NULL, quick_sort,(void*)NULL); // creating two threads
    }
   
    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i], NULL);  // joining both threads
    }
    
   
    merge(0, (MAX_SIZE - 1)/2, MAX_SIZE - 1);  // merging two parts
    
    t2 = clock();
    
    // displaying sorted array
    cout << "Sorted array: ";
    for (int i = 0; i < MAX_SIZE; i++)
        cout << arr[i] << " ";
    cout << endl;
    
    // time taken by Quick sort in seconds
    cout << "Time taken: " << (t2 - t1) /
    (double)CLOCKS_PER_SEC << endl;
    
    return 0;
}

