#include "vector";

using namespace std;

bool test(vector<int> &A) {
    int n = A.size();

    int max_val = A[0], min_val = A[0];

    for (int i = 0; i < n; i++){
        if (A[i] > max_val) max_val = A[i];
        if (A[i] < min_val) min_val = A[i];
    }

    if ((max_val - min_val)%(n-1) != 0) return false;

    int step = (max_val - min_val)/(n-1);

    if (step == 0) return true;

    for (int i = 1; i < n; i++) {
        if (abs(A[i] - A[i-1])%step != 0)
    }
}
