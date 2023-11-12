#include "vector";
#include "iostream";

using namespace std;

void quick_sort(vector<int> &arr, int start, int end) {
    if (start >= end) return;

    int pivot = arr[start], i = start + 1, j = end;

    while(i <= j) {
        if (arr[i] > pivot && arr[j] < pivot) {
            swap(arr[i], arr[j]);
            i++;j--;
        }

        if (arr[i] < pivot) i++;
        if (arr[j] > pivot) j--;
    }

    swap(arr[start], arr[j]);

    quick_sort(arr, start, j-1);
    quick_sort(arr, j+1, end);
}

int main() {
    vector<int> test = {1,5,4,3,2};

    quick_sort(test, 0, (int)test.size() - 1);

    for (int i = 0; i < test.size(); i++) {
        cout << test[i] << endl;
    }
}