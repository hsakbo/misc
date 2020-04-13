#include <iostream>

/* MIT 6.006 lecture 4 

for array index i starting from 1
parent = i / 2
left child = 2 * i
right child = 2 * i + 1


1st procedure: build a max heap.

max-heapify(Array, i):
it is to correct a single violation of the heap property
in a subtree's root. Assume that the tree rooted at left(i)
and right(i) are max heaps. This assumption works from 
bottom-up as leaves of a maxheap are trivially maxheaps
themselves. (worst case: O(log n))

Find max element Array[0]
Swap max with Array[n] so that the max element is at the end
Reduce n by 1. So the current heapsize is n-1
max-heapify(array(n-1), i)
repeat.

*/

void max_heapify(int *arr, int size, int i)
{
  int l_ind = 2 * i + 1;
  int r_ind = 2 * i + 2;
  int s_ind = i;

  if (l_ind < size && arr[l_ind] > arr[s_ind])
    s_ind = l_ind;
     
  if (r_ind < size && arr[r_ind] > arr[s_ind])
    s_ind = r_ind;
    
  if (s_ind != i)
    {
      arr[i] = arr[i] ^ arr[s_ind];
      arr[s_ind] = arr[i] ^ arr[s_ind];
      arr[i] = arr[i] ^ arr[s_ind];
      max_heapify(arr, size, s_ind);
    }
    
  return;
}

//this is O(n)
void build_heap(int *arr, int size)
{
  int n = size / 2 - 1;

  for (int i = n; i >= 0; i--)
    max_heapify(arr, size, i);
}

void heapsort(int *arr, int size)
{
  build_heap(arr, size);
  
  int last = size-1;
  for (int i = last; i > 0; i--)
    {
      arr[0] = arr[0] ^ arr[i];
      arr[i] = arr[0] ^ arr[i];
      arr[0] = arr[0] ^ arr[i];
      max_heapify(arr, --size, 0);
    }
}

using namespace std;
void print(int *, int);
int main()
{
  
  int arr[] = {10,9,8,7,6,5,4,3,2,1,0,-1};
  int arr2[] = {1,2,3,4,5,6,7};

  int size = sizeof(arr) / sizeof(int);
  heapsort(arr, size);
  print(arr, size);
  
  return 0;
}

void print(int *arr, int size)
{
  for (int i = 0; i < size; i++)
    cout << arr[i] << " ";
  cout << endl;
}
