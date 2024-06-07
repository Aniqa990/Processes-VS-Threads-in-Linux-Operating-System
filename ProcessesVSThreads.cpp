#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

#define THREADS_2 2
#define THREADS_4 4

using namespace std;


struct ThreadArgs {
    int* arr;
    int low;
    int high;
};

void insertionSortProcess(int* arr, int ARRAY_SIZE)
{
    int i, key, j;
    for (i = 1; i < ARRAY_SIZE; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}


void* insertionSort_for_Threads(void* args) {
    ThreadArgs* tArgs = (ThreadArgs*)args;
    
    int start = tArgs->low;
    int end = tArgs->high;
    int* arr = tArgs->arr;
    

    for (int i = start + 1; i <= end; ++i) {
        int key = arr[i];
        int j = i - 1;

        while (j >= start && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }

    pthread_exit(NULL);
}

void selectionSortProcess(int* arr, int ARRAY_SIZE) 
{ 
    int i, j, min_idx; 
  
    for (i = 0; i < ARRAY_SIZE - 1; i++) { 
 
        min_idx = i; 
        for (j = i + 1; j < ARRAY_SIZE; j++) { 
            if (arr[j] < arr[min_idx]) 
                min_idx = j; 
        } 
        
        if (min_idx != i) 
            swap(arr[min_idx], arr[i]); 
    } 
} 

void* selectionSort_for_Threads(void* args) {
    ThreadArgs* tArgs = (ThreadArgs*)args;
    
    int start = tArgs->low;
    int end = tArgs->high;
    int* arr = tArgs->arr;
    
    int i, j, min_idx;

    for (i = start; i < end - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < end; j++) {
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }

        if (min_idx != i)
            swap(arr[min_idx], arr[i]);
    }
    
    pthread_exit(NULL);
}


void bubbleSortProcess(int *arr, int ARRAY_SIZE)
{
    int i, j;
    bool swapped;
    for (i = 0; i < ARRAY_SIZE - 1; i++) {
        swapped = false;
        for (j = 0; j < ARRAY_SIZE - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false)
            break;
    }
}

void* bubbleSort_for_Threads(void* args) {
    ThreadArgs* tArgs = (ThreadArgs*)args;
    
    int start = tArgs->low;
    int end = tArgs->high;
    int* arr = tArgs->arr;
    
    bool swapped;

    for (int i = start; i < end - 1; i++) {
        swapped = false;
        for (int j = start; j < end - i -1 + start; j++) { //adding start for correct indexing
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        
        if (swapped == false)
            break;
    }

    pthread_exit(NULL);
}


void merge(int low, int mid, int high, int* arr) {
    int* L = new int[mid - low + 1];
    int* R = new int[high - mid];

    int size_1 = mid - low + 1;
    int size_2 = high - mid;

    for (int i = 0; i < size_1; i++)
        L[i] = arr[i + low];

    for (int i = 0; i < size_2; i++)
        R[i] = arr[i + mid + 1];

    int k = low;
    int i = 0, j = 0;

    while (i < size_1 && j < size_2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < size_1)
        arr[k++] = L[i++];

    while (j < size_2)
        arr[k++] = R[j++];
        
    delete [] L;
    delete [] R;
}

void mergeSort(int low, int high, int* arr) {
    int mid = low + (high - low) / 2;
    if (low < high) {
        mergeSort(low, mid, arr);
        mergeSort(mid + 1, high, arr);
        merge(low, mid, high, arr);
    }
}

int partition(int low, int high, int* arr) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(int low, int high, int* arr) {
    if (low < high) {
        int pi = partition(low, high, arr);

        quickSort(low, pi - 1, arr);
        quickSort(pi + 1, high, arr);
    }
}

void* quickSort_for_Threads(void* arg) {
    ThreadArgs* tArgs = (ThreadArgs*)arg;
    int low = tArgs->low;
    int high = tArgs->high;
    int* arr = tArgs->arr;
    

    if (low < high) {
        int pi = partition(low, high, arr);

        quickSort(low, pi - 1, arr);
        quickSort(pi + 1, high, arr);
    }

    pthread_exit(NULL);

}

void* mergeSort_for_Threads(void* arg) {
    ThreadArgs* tArgs = (ThreadArgs*)arg;
    int low = tArgs->low;
    int high = tArgs->high;
    int* arr = tArgs->arr;
    

    int mid = low + (high - low) / 2;
    if (low < high) {
        mergeSort(low, mid, arr);
        mergeSort(mid + 1, high, arr);
        merge(low, mid, high, arr);
    }

    pthread_exit(NULL);
}


void reset_array(int* og_arr, int* arr, int ARRAY_SIZE){
    for(int i=0; i<ARRAY_SIZE; i++){
    	arr[i] = og_arr[i];
    }
}
	

// Main function
int main() {
    
    vector <int> arr;
    vector <int> og_arr;
    int ARRAY_SIZE;
    
    string filename;
    
    cout<<"Enter the name of file you want elements from: ";
    cin>>filename;
    
    ifstream file(filename);

    if (!file)
    {
        cerr << "Failed to open file" << endl;
        return -1;
    }

    int element;
    if (file.is_open()) {
        while (file >> element) {
            arr.push_back(element);
        }
    }

    file.close();
    
    ARRAY_SIZE = arr.size();
    
    //saving original array read from file in og_arr
    
    for(int i=0; i<arr.size(); i++){
    	og_arr.push_back(arr[i]);
    }
    
    
    clock_t startTime, endTime;
    
    
    
    
    //PROCESSES
    
    //bubble sort
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;
    
    startTime = clock();

    bubbleSortProcess(arr.data(), ARRAY_SIZE);
    
    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl<<endl;
    
    cout << "Time taken for bubble sort process: " << (double)(endTime - startTime) / (double)CLOCKS_PER_SEC << " seconds" << endl<<endl;
    
    
    //insertion sort
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;
    
    startTime = clock();

    insertionSortProcess(arr.data(), ARRAY_SIZE);
    
    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl<<endl;
    
    cout << "Time taken for insertion sort process: " << (double)(endTime - startTime) / (double)CLOCKS_PER_SEC << " seconds" << endl<<endl;
    
    
    //selection sort
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;
    
    startTime = clock();

    selectionSortProcess(arr.data(), ARRAY_SIZE);
    
    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl<<endl;
    
    cout << "Time taken for selection sort process: " << (double)(endTime - startTime) / (double)CLOCKS_PER_SEC << " seconds" << endl<<endl;
    
    //merge sort
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;
    
    startTime = clock();

    mergeSort(0, ARRAY_SIZE-1, arr.data());
    
    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl<<endl;
    
    cout << "Time taken for merge sort process: " << (double)(endTime - startTime) / (double)CLOCKS_PER_SEC << " seconds" << endl<<endl;
    
    //quick sort
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;
    
    startTime = clock();

    quickSort(0, ARRAY_SIZE-1, arr.data());
    
    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl<<endl;
    
    cout << "Time taken for quick sort process: " << (double)(endTime - startTime) / (double)CLOCKS_PER_SEC << " seconds" << endl<<endl;
    
    
    
    
    
    
    
    // 2 THREADED PROCESSES
    
    pthread_t threads[THREADS_2];
    ThreadArgs threadArgs[THREADS_2];
    int segment_size = ARRAY_SIZE / THREADS_2;
    




    //bubble sort with 2 threads
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;
    
    startTime = clock();

    for (int i = 0; i < THREADS_2; i++) {
        threadArgs[i].arr = arr.data();
        threadArgs[i].low = i * segment_size;
        threadArgs[i].high = (i + 1) * segment_size;
        pthread_create(&threads[i], NULL, bubbleSort_for_Threads, (void*)&threadArgs[i]);
    }

    for (int i = 0; i < THREADS_2; ++i) {
        pthread_join(threads[i], NULL);
    }

    merge(0, (ARRAY_SIZE - 1) / 2, ARRAY_SIZE - 1, arr.data());
    
    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl << endl;
    
    cout << "Time taken for bubble sort with 2 threads: " << (double)(endTime - startTime) / (double)CLOCKS_PER_SEC << " seconds" << endl<<endl;






    //insertion sort with 2 threads
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    cout<<endl<<endl;
    
    startTime = clock();

    for (int i = 0; i < THREADS_2; i++) {
        threadArgs[i].arr = arr.data();
        threadArgs[i].low = i * segment_size;
        threadArgs[i].high = (i + 1) * segment_size - 1;
        pthread_create(&threads[i], NULL, insertionSort_for_Threads, (void*)&threadArgs[i]);
    }

    for (int i = 0; i < THREADS_2; ++i) {
        pthread_join(threads[i], NULL);
    }

    merge(0, (ARRAY_SIZE - 1) / 2, ARRAY_SIZE - 1, arr.data());
    
    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl <<endl;
    
    cout << "Time taken for insertion sort with 2 threads: " << (double)(endTime - startTime) / (double)CLOCKS_PER_SEC << " seconds" << endl << endl;
    




    //selection sort with 2 threads
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;
   
    
    startTime = clock();
    
    for (int i = 0; i < THREADS_2; i++) {
        threadArgs[i].arr = arr.data();
        threadArgs[i].low = i * segment_size;
        threadArgs[i].high = (i + 1) * segment_size;
        pthread_create(&threads[i], NULL, selectionSort_for_Threads, (void*)&threadArgs[i]);
    }
    
    for (int i = 0; i < THREADS_2; i++) {
        pthread_join(threads[i], NULL);
    }


    merge(0, (ARRAY_SIZE - 1) / 2, ARRAY_SIZE - 1, arr.data());

    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; i++)
        cout << arr[i] << " ";
    cout << endl;

    cout << "Time taken for selection sort with 2 threads: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl << endl;
    




    //merge sort with 2 threads
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    
    cout << endl << endl;
    
    startTime = clock();
    
    for (int i = 0; i < THREADS_2; i++) {
        threadArgs[i].arr = arr.data();
        threadArgs[i].low = i * segment_size;
        threadArgs[i].high = (i + 1) * segment_size - 1;
        pthread_create(&threads[i], NULL, mergeSort_for_Threads, (void*)&threadArgs[i]);
    }
    
    for (int i = 0; i < THREADS_2; i++)
        pthread_join(threads[i], NULL);

    merge(0, (ARRAY_SIZE - 1) / 2, ARRAY_SIZE - 1, arr.data());

    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; i++)
        cout << arr[i] << " ";
    cout << endl;

    cout << "Time taken for merge sort with 2 threads: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl << endl;




    
    //quick sort with 2 threads
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;
    
    startTime = clock();

    for (int i = 0; i < THREADS_2; i++) {
        threadArgs[i].arr = arr.data();
        threadArgs[i].low = i * segment_size;
        threadArgs[i].high = (i + 1) * segment_size - 1;
        pthread_create(&threads[i], NULL, quickSort_for_Threads, (void*)&threadArgs[i]);
    }

    for (int i = 0; i < THREADS_2; i++)
        pthread_join(threads[i], NULL);


    merge(0, (ARRAY_SIZE - 1) / 2, ARRAY_SIZE - 1, arr.data());

    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; i++)
        cout << arr[i] << " ";
    cout << endl;

    cout << "Time taken for quick sort with 2 threads: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl<<endl;
    
    
    
    
    //PROCESSES WITH 4 THREADS




    
    pthread_t threads_4[THREADS_4];
    ThreadArgs args_4[THREADS_4];
    segment_size = ARRAY_SIZE / THREADS_4;
    



    //bubble sort with 4 threads
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
    	cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;
    
    startTime = clock();

    for (int i = 0; i < THREADS_4; i++) {
    	args_4[i].arr = arr.data();
    	args_4[i].low = i * segment_size;
    	args_4[i].high = (i + 1) * segment_size;
    	pthread_create(&threads_4[i], NULL, bubbleSort_for_Threads, (void*)&args_4[i]);
    }

    for (int i = 0; i < THREADS_4; ++i) {
    	pthread_join(threads_4[i], NULL);
    }

    //mids calculated with start+(end-start)/2
    merge(0, (ARRAY_SIZE / 2 - 1) / 2, ARRAY_SIZE / 2 - 1, arr.data());
    merge(ARRAY_SIZE / 2, ARRAY_SIZE / 2 + (ARRAY_SIZE - 1 - ARRAY_SIZE / 2) / 2, ARRAY_SIZE - 1, arr.data());
    merge(0, (ARRAY_SIZE - 1) / 2, ARRAY_SIZE - 1, arr.data());
    
    endTime = clock();

    cout << "Sorted array: ";
    
    for (int i = 0; i < ARRAY_SIZE; ++i) {
    	cout << arr[i] << " ";
    }
    cout << endl<<endl;
    
    cout << "Time taken for bubble sort with 4 threads: " << (double)(endTime - startTime) / (double)CLOCKS_PER_SEC << " seconds" << endl<<endl;




    
    //insertion sort with 4 threads
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
    	cout << arr[i] << " ";
    }
    cout<<endl<<endl;
    
    startTime = clock();

    for (int i = 0; i < THREADS_4; i++) {
    	args_4[i].arr = arr.data();
    	args_4[i].low = i * segment_size;
    	args_4[i].high = (i + 1) * segment_size-1;
    	pthread_create(&threads_4[i], NULL, insertionSort_for_Threads, (void*)&args_4[i]);
    }

    for (int i = 0; i < THREADS_4; ++i) {
    	pthread_join(threads_4[i], NULL);
    }

    merge(0, (ARRAY_SIZE / 2 - 1) / 2, ARRAY_SIZE / 2 - 1, arr.data());
    merge(ARRAY_SIZE / 2, ARRAY_SIZE / 2 + (ARRAY_SIZE - 1 - ARRAY_SIZE / 2) / 2, ARRAY_SIZE - 1, arr.data());
    merge(0, (ARRAY_SIZE - 1) / 2, ARRAY_SIZE - 1, arr.data());
    
    
    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
    	cout << arr[i] << " ";
    }
    cout << endl <<endl;
    
    cout << "Time taken for insertion sort with 4 threads: " << (double)(endTime - startTime) / (double)CLOCKS_PER_SEC << " seconds" << endl<<endl;
    
    
    


    //selection sort with 4 threads
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
    	cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;

    
    startTime = clock();
    
    for (int i = 0; i < THREADS_4; i++) {
    	args_4[i].arr = arr.data();
    	args_4[i].low = i * segment_size;
    	args_4[i].high = (i + 1) * segment_size;
    	pthread_create(&threads_4[i], NULL, selectionSort_for_Threads, (void*)&args_4[i]);
    }
    
    for (int i = 0; i < THREADS_4; i++) {
    	pthread_join(threads_4[i], NULL);
    }


    merge(0, (ARRAY_SIZE / 2 - 1) / 2, ARRAY_SIZE / 2 - 1, arr.data());
    merge(ARRAY_SIZE / 2, ARRAY_SIZE / 2 + (ARRAY_SIZE - 1 - ARRAY_SIZE / 2) / 2, ARRAY_SIZE - 1, arr.data());
    merge(0, (ARRAY_SIZE - 1) / 2, ARRAY_SIZE - 1, arr.data());
    

    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; i++)
        cout << arr[i] << " ";
    cout << endl << endl;

    cout << "Time taken for selection sort with 4 threads: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl<<endl;
    
    
    
    
    
    //merge sort with 4 threads
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
    	cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;
    
    startTime = clock();
    
    for (int i = 0; i < THREADS_4; i++) {
    	args_4[i].arr = arr.data();
    	args_4[i].low = i * segment_size;
    	args_4[i].high = (i + 1) * segment_size - 1;
    	pthread_create(&threads_4[i], NULL, mergeSort_for_Threads, (void*)&args_4[i]);
    }

    for (int i = 0; i < THREADS_4; i++)
    	pthread_join(threads_4[i], NULL);
 

    merge(0, (ARRAY_SIZE / 2 - 1) / 2, ARRAY_SIZE / 2 - 1, arr.data());
    merge(ARRAY_SIZE / 2, ARRAY_SIZE / 2 + (ARRAY_SIZE - 1 - ARRAY_SIZE / 2) / 2, ARRAY_SIZE - 1, arr.data());
    merge(0, (ARRAY_SIZE - 1) / 2, ARRAY_SIZE - 1, arr.data());
    

    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; i++)
    	cout << arr[i] << " ";
    cout << endl << endl;

    cout << "Time taken for merge sort with 4 threads: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl<<endl;
    
    
    
    
    
    //quick sort with 4 threads
    
    reset_array(og_arr.data(), arr.data(), ARRAY_SIZE);
    
    cout << "Unsorted array: ";
    
    for (int i = 0; i < ARRAY_SIZE; ++i) {
    	cout << arr[i] << " ";
    }
    
    cout<<endl<<endl;
    
    startTime = clock();
    
    
    for (int i = 0; i < THREADS_4; i++) {
    	args_4[i].arr = arr.data();
    	args_4[i].low = i * segment_size;
    	args_4[i].high = (i + 1) * segment_size - 1;
        
        pthread_create(&threads_4[i], NULL, quickSort_for_Threads, (void*)&args_4[i]);
    }
    
    for (int i = 0; i < THREADS_4; i++)
    	pthread_join(threads_4[i], NULL);


    merge(0, (ARRAY_SIZE / 2 - 1) / 2, ARRAY_SIZE / 2 - 1, arr.data());
    merge(ARRAY_SIZE / 2, ARRAY_SIZE / 2 + (ARRAY_SIZE - 1 - ARRAY_SIZE / 2) / 2, ARRAY_SIZE - 1, arr.data());
    merge(0, (ARRAY_SIZE - 1) / 2, ARRAY_SIZE - 1, arr.data());
    
    
    endTime = clock();

    cout << "Sorted array: ";
    for (int i = 0; i < ARRAY_SIZE; i++)
    	cout << arr[i] << " ";
    cout << endl << endl;

	cout << "Time taken for quick sort with 4 threads: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl<<endl;

    

    return 0;
}

