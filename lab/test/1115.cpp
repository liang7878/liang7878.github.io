#include <condition_variable>
#include <mutex>
#include <iostream>
#include <thread>

using namespace std;

mutex mut;
condition_variable cond1, cond2;
int num = 1;

void t1() {
    while(num <= 10) {
        unique_lock<mutex> locker(mut);
        cout << "t1: " << num << endl;
        num++;
        cond2.notify_one();
        cond1.wait(locker);
    }
}

void t2() {
    while (num <= 10) {
        unique_lock<mutex> locker(mut);
        cout << "t2: " << num << endl;

        num++;
        cond1.notify_one();
        cond2.wait(locker);
    }
}

int main() {
    thread thread1(t2);
    thread thread2(t1);
    thread1.join();
    thread2.join();

    return 0;
}