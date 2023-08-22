#include <iostream>

using namespace std;

int main() {
    double sum;
    int n = 1;
    int k;

    cin >> k;

    while (sum <= k) {
        sum += 1.0 / n;
        n++;
    }

    cout << n - 1 << endl;

    return 0;
}
