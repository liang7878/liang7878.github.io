#include <iostream>

using namespace std;

int main() {
    int ans;
    int n;

    cin >> n;

    for (int i = 0; i < 3; i++) {
        int price, count;

        cin >> count >> price;

        int total_price = price, total_count = count;
        while (total_count < n) {
            total_price <<= 1;
            total_count <<= 1;
        }

        while (total_count > n) {
            total_price -= price;
            total_count -= count;
        }

        while (total_count < n) {
            total_price += price;
            total_count += count;
        }

        if (total_price < ans || ans == 0) {
            ans = total_price;
        }
    }

    cout << ans << endl;

    return 0;
}
