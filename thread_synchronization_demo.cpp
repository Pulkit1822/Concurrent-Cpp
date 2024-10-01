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
	// Producer will run and produce data 10 times
	int count = 10;
	while (count > 0) {
		// Lock the mutex to protect the shared dataQueue
		{
			std::unique_lock<std::mutex> lock(dataMutex);
			// Push the data to the front of the queue
			dataQueue.push_front(count);
		}
		// Notify one waiting thread that data is available
		dataCondVar.notify_one();
		// Pause for 1 second before producing the next data
		std::this_thread::sleep_for(std::chrono::seconds(1));
		count--;
	}
}

// Function to consume data
void consumer() {
	int data = 0;
	while (data != 1) {
		// Lock the mutex to protect the shared dataQueue
		std::unique_lock<std::mutex> lock(dataMutex);
		// Wait until the dataQueue is not empty and the
		// condition variable is signaled
		dataCondVar.wait(lock, []() { return !dataQueue.empty(); });
		// Get the data from the back of the queue and remove it
		data = dataQueue.back();
		dataQueue.pop_back();
		// Print the data
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
	// Create a producer thread and a consumer thread
	std::thread producerThread(producer);
	std::thread consumerThread(consumer);
	// Wait for both threads to finish
	producerThread.join();
	consumerThread.join();

	// Using std::future to get result from a thread
	// Create a future that will hold the result of the factorial function
	std::future<int> futureResult = std::async(std::launch::async, factorial, 5);
	// Get the result from the future
	std::cout << "Factorial result: " << futureResult.get() << std::endl;

	// Using std::promise and std::future for asynchronous data provision
	// Create a promise to hold the result of the factorial function
	std::promise<int> promise;
	// Create a future that will hold the result of the promise
	std::future<int> future = promise.get_future();
	// Create a thread that will set the value of the promise
	std::thread promiseThread([&promise]() {
		// Pause for 100 milliseconds before setting the value of the promise
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		// Set the value of the promise
		promise.set_value(6);
	});
	// Get the result from the future
	std::cout << "Promise result: " << future.get() << std::endl;
	// Wait for the thread to finish
	promiseThread.join();

	// Using shared_future to share the result among multiple threads
	// Create a promise to hold the result of the factorial function
	std::promise<int> sharedPromise;
	// Create a shared future that will hold the result of the promise
	std::shared_future<int> sharedFuture = sharedPromise.get_future().share();
	// Create two threads that will get the result from the shared future
	std::thread sharedThread1([sharedFuture]() {
		// Get the result from the shared future
		std::cout << "Shared future result in thread 1: " << sharedFuture.get() << std::endl;
	});
	std::thread sharedThread2([sharedFuture]() {
		// Get the result from the shared future
		std::cout << "Shared future result in thread 2: " << sharedFuture.get() << std::endl;
	});
	// Set the value of the promise
	sharedPromise.set_value(7);
	// Wait for the threads to finish
	sharedThread1.join();
	sharedThread2.join();

	// Using std::packaged_task to package a function and its arguments
	// Create a packaged task that will hold the result of the factorial function
	std::packaged_task<int(int)> task(factorial);
	// Create a future that will hold the result of the packaged task
	std::future<int> taskFuture = task.get_future();
	// Create a thread that will execute the packaged task
	std::thread taskThread(std::move(task), 4);
	// Get the result from the future
	std::cout << "Packaged task result: " << taskFuture.get() << std::endl;
	// Wait for the thread to finish
	taskThread.join();

	// Demonstrating time constraints with threads and synchronization primitives
	// Create a thread that will sleep for 500 milliseconds
	std::thread timeThread([]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	});
	// Wait for the thread to finish
	timeThread.join();

	// Create a mutex to protect the shared data
	std::mutex timeMutex;
	// Create a unique lock to lock the mutex
	std::unique_lock<std::mutex> timeLock(timeMutex);
	// Create a condition variable to wait for
	std::condition_variable timeCondVar;
	// Wait for 200 milliseconds or until the condition variable is signaled
	timeCondVar.wait_for(timeLock, std::chrono::milliseconds(200));

	// Create a promise to hold the result of the factorial function
	std::promise<int> timePromise;
	// Create a future that will hold the result of the promise
	std::future<int> timeFuture = timePromise.get_future();
	// Wait for 300 milliseconds or until the future is ready
	timeFuture.wait_for(std::chrono::milliseconds(300));

	return 0;
}

