#include <iostream>

using namespace std;

int main() {
    int mom = 0;
    int jin = 0;

    for (int i = 0; i < 12; i++) {
        int budget;
        cin >> budget;

        int remain = jin + 300 - budget;

        if (remain < 0)  {
            cout << "-" << (i+1) << endl;
            return 0;
        } else {
            mom = mom + remain - remain%100;
            jin = remain%100;
        }
    }

    cout << mom*1.2+jin << endl;

    return 0;
}