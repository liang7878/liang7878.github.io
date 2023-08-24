#include<iostream>

using namespace std;

int main() {
    int n, x;

    // cin >> n >> x;

    n = 728; x = 7;

    int m = 1, ans = 0;

    while(m <= n) {
        // 首先求出 n 在当前位的数字
        int value = n / m % 10;

        // 接着求出 n 左侧的数字
        int left = n / (m * 10);

        // 接着求出 n 右侧的数字
        int right = n % m;

        // 这样就可以开始求结果了

        if (x == 0) {
            if (value == 0) {
                ans += (left-1)*m + right + 1;
            } else {
                ans += left * m;
            }

        } else {
            if (value > x) {
                ans += (left + 1) * m; // 当前位大于目标 x，左侧 + 1后乘以m
            }
            if (value == x) {
                ans += (left*m+right+1);
            }
            if (value < x) {
                ans += left * m;
            }
        }

        m *= 10;
    }

    cout << ans << endl;

    return 0;  
}