#include "iostream";
#include "vector";

using namespace std;


int getProfit(vector<int>& prices) {
    int res = 0, n = prices.size();
    for (int i = 0; i < n - 1; ++i) {
        if (prices[i] < prices[i + 1]) {
            res += prices[i + 1] - prices[i];
        }
    }
    return res;
}

int main() {
    vector<int> test;
 
    test.push_back(1);
    test.push_back(4);
    test.push_back(5);
    test.push_back(3);

    cout << getProfit(test) << endl;
}