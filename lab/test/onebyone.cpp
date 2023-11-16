#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>

using namespace std;

mutex mut;  // 定义互斥量
condition_variable cond1, cond2;  // 定义两个条件变量，不同线程等待不同的条件变量
int num = 1;

void thread1() {
	while (num <= 10) {
		unique_lock<mutex> locker(mut); // 使用 unique_lock 锁管理类，自动上锁和解锁
		cout << "thread1: " << num << endl;
		num++;
		cond2.notify_one(); // 线程 1 打印完成，通知线程 2 打印
		cond1.wait(locker); // 等待线程 2 打印完成再继续打印
	}
}

void thread2() {
	while (num <= 10) {
		unique_lock<mutex> locker(mut);
		cout << "thread2: " << num << endl;
		num++;
		cond1.notify_one();
		cond2.wait(locker);
	}
}

int main() {
	thread t1(thread1);  // 定义两个线程
	thread t2(thread2);
	t1.join();  // 回收线程资源
	t2.join();
	
	system("pause");
	return 0;
}