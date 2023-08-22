#include <iostream>

using namespace std;

int main() {
    int res = 0, max = 0;

    for (int i = 0; i < 7; i++) {
        int a, b;

        cin >> a >> b;

        if (a + b > 8 && a + b > max) {
            max = a + b;
            res = i + 1;
        }
    }

    cout << res << endl;

    return 0;
}
