#include <iostream>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <chrono>
#include <functional>

// Global deque and mutex for synchronization
std::deque<int> dataQueue;
std::mutex dataMutex;
std::condition_variable dataCondVar;

// Function to produce data
void producer() {
	int count = 10;
	while (count > 0) {
		{
			std::unique_lock<std::mutex> lock(dataMutex);
			dataQueue.push_front(count);
		}
		dataCondVar.notify_one();  // Notify one waiting thread
		std::this_thread::sleep_for(std::chrono::seconds(1));
		count--;
	}
}

// Function to consume data
void consumer() {
	int data = 0;
	while (data != 1) {
		std::unique_lock<std::mutex> lock(dataMutex);
		dataCondVar.wait(lock, []() { return !dataQueue.empty(); });  // Wait until notified and queue is not empty
		data = dataQueue.back();
		dataQueue.pop_back();
		std::cout << "Consumer received data: " << data << std::endl;
	}
}

int factorial(int n) {
	int result = 1;
	for (int i = n; i > 1; --i) {
		result *= i;
	}
	return result;
}

int main() {
	// Thread synchronization using condition variable
	std::thread producerThread(producer);
	std::thread consumerThread(consumer);
	producerThread.join();
	consumerThread.join();

	// Using std::future to get result from a thread
	std::future<int> futureResult = std::async(std::launch::async, factorial, 5);
	std::cout << "Factorial result: " << futureResult.get() << std::endl;

	// Using std::promise and std::future for asynchronous data provision
	std::promise<int> promise;
	std::future<int> future = promise.get_future();
	std::thread promiseThread([&promise]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		promise.set_value(6);
	});
	std::cout << "Promise result: " << future.get() << std::endl;
	promiseThread.join();

	// Using shared_future to share the result among multiple threads
	std::promise<int> sharedPromise;
	std::shared_future<int> sharedFuture = sharedPromise.get_future().share();
	std::thread sharedThread1([sharedFuture]() {
		std::cout << "Shared future result in thread 1: " << sharedFuture.get() << std::endl;
	});
	std::thread sharedThread2([sharedFuture]() {
		std::cout << "Shared future result in thread 2: " << sharedFuture.get() << std::endl;
	});
	sharedPromise.set_value(7);
	sharedThread1.join();
	sharedThread2.join();

	// Using std::packaged_task to package a function and its arguments
	std::packaged_task<int(int)> task(factorial);
	std::future<int> taskFuture = task.get_future();
	std::thread taskThread(std::move(task), 4);
	std::cout << "Packaged task result: " << taskFuture.get() << std::endl;
	taskThread.join();

	// Demonstrating time constraints with threads and synchronization primitives
	std::thread timeThread([]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	});
	timeThread.join();

	std::mutex timeMutex;
	std::unique_lock<std::mutex> timeLock(timeMutex);
	std::condition_variable timeCondVar;
	timeCondVar.wait_for(timeLock, std::chrono::milliseconds(200));

	std::promise<int> timePromise;
	std::future<int> timeFuture = timePromise.get_future();
	timeFuture.wait_for(std::chrono::milliseconds(300));

	return 0;
}
