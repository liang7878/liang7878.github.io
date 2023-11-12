#include "iostream";
#include "vector";

using namespace std;

int partition(vector<int>& A, int start, int end) {
    int i = start, j = end, pivot = A[end];
    while (i <= j) {
        while (A[i] < pivot) ++i;
        while (A[j] > pivot) --j;
        if (i <= j) {
            swap(A[i], A[j]);
            ++i; --j;
        }
    }
    swap(A[i], A[end]);
    return i;
}

void quick_sort(vector<int> &A, int start, int end) {
    if (start >= end) return;
    int pos = partition(A, start, end);
    quick_sort(A, start, pos - 1);
    quick_sort(A, pos, end);
}

bool quick_sort(vector<int>& A) {
    int n = A.size();

    if (n <= 2) return true;

    quick_sort(A, 0, A.size() - 1);

    int step = A[1] - A[0];

    for (int i = 2; i < A.size(); ++i) {
        if (A[i] - A[i-1] != step) return false;
    }

    return true;
}


int main() {
    vector<int> test;

    test.push_back(5);
    test.push_back(1);
    test.push_back(3);

    cout << quick_sort(test) << endl;
}