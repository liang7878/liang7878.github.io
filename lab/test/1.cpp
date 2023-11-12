#include "vector";
#include "iostream";

using namespace std;

bool checkNum(int num) {
    // 一个整数n,判断n是否为丑数。如果是,返回true;否则,返回false。 丑数就是只包含质因数2、3或5的正整数。
    if (num < 2) return false;
    if (num == 2 || num == 3 || num == 5 || num == 4) return true;

    vector<bool> res(num + 1, false);

    res[1] = true;
    res[2] = true; res[3] = true; res[4] = true; res[5] = true;

    for (int i = 6; i <= num; i++) {
        if (i%2 == 0 && res[i/2]) res[i] = true;
        else if (i%3 == 0 && res[i/3]) res[i] = true;
        else if (i%5 == 0 && res[i/5]) res[i] = true;
        else res[i] = false;
    }

    return res[num];
}

int main() {
    cout << checkNum(1) << endl;
    cout << checkNum(2) << endl;
    cout << checkNum(9) << endl;
    cout << checkNum(14) << endl;
    cout << checkNum(16) << endl;
    cout << checkNum(100) << endl;
}