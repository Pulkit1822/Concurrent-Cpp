/*
 *  Concurrent C++ Programming Example
 */

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>

// Global mutex for thread synchronization
std::mutex global_mutex;

// Function to print shared data, with thread safety using a lock_guard
void print_shared_data(const std::string& thread_id, int value) {
	// Acquire the lock only for the duration of the function
	std::lock_guard<std::mutex> lock(global_mutex);
	// Print the data
	std::cout << "From " << thread_id << ": " << value << std::endl;
}

// Functor class for thread execution
class Task {
public:
	// Operator() is called by the thread and prints shared data
	void operator()() {
		for (int i = -1; i >= -100; --i) {
			// Call the print_shared_data function with the thread id and value
			print_shared_data("Thread 1", i);
		}
	}
};

int main() {
	// Create an instance of the Task functor class
	Task task;
	// Create a thread and pass the functor object to it
	std::thread thread1(task);

	// Main thread loop
	for (int i = 0; i < 100; ++i) {
		// Call the print_shared_data function with the main thread id and value
		print_shared_data("Main Thread", i);
	}

	// Wait for the thread to finish
	thread1.join();
	return 0;
}

/*
 * Avoiding global variables and bundling mutex with the resource it protects
 */
class Logger {
	// Mutex to protect the log file
	std::mutex file_mutex;
	// Log file stream
	std::ofstream log_file;

public:
	// Constructor that opens the log file
	Logger() {
		log_file.open("log.txt");
	}

	// Function to print to log file with thread safety
	void log(const std::string& thread_id, int value) {
		// Acquire the lock only for the duration of the function
		std::lock_guard<std::mutex> lock(file_mutex);
		// Print the data
		log_file << "From " << thread_id << ": " << value << std::endl;
	}

	// Prevent leaking the file stream to the outside world
	void process_file(void (*func)(std::ofstream&)) {
		// Call the function with the log file stream
		func(log_file);
	}
};

// Functor class for thread execution with Logger
class LoggerTask {
	// Reference to the Logger object
	Logger& logger;

public:
	// Constructor that takes a reference to a Logger object
	LoggerTask(Logger& log) : logger(log) {}

	// Operator() is called by the thread and prints to the log file
	void operator()() {
		for (int i = -1; i >= -100; --i) {
			// Call the log function with the thread id and value
			logger.log("Thread 1", i);
		}
	}
};

int main() {
	// Create an instance of the Logger class
	Logger logger;
	// Create an instance of the LoggerTask functor class
	LoggerTask logger_task(logger);
	// Create a thread and pass the functor object to it
	std::thread thread1(logger_task);

	// Main thread loop
	for (int i = 0; i < 100; ++i) {
		// Call the log function with the main thread id and value
		logger.log("Main Thread", i);
	}

	// Wait for the thread to finish
	thread1.join();
	return 0;
}

/*
 * Example of potential deadlock and its solution using std::lock
 */
class SafeLogger {
	// Mutex 1
	std::mutex mutex1;
	// Mutex 2
	std::mutex mutex2;
	// Log file stream
	std::ofstream log_file;

public:
	// Constructor that opens the log file
	SafeLogger() {
		log_file.open("safe_log.txt");
	}

	// Function to log data with proper mutex locking to avoid deadlock
	void log_data(const std::string& thread_id, int value) {
		// Lock both mutexes at the same time to avoid deadlock
		std::lock(mutex1, mutex2);
		// Acquire the lock only for the duration of the function
		std::lock_guard<std::mutex> lock1(mutex1, std::adopt_lock);
		std::lock_guard<std::mutex> lock2(mutex2, std::adopt_lock);
		// Print the data
		log_file << "From " << thread_id << ": " << value << std::endl;
	}
};

/*
 * Example of using unique_lock for deferred locking and transferring ownership
 */
class DeferredLogger {
	// Mutex to protect the log file
	std::mutex file_mutex;
	// Log file stream
	std::ofstream log_file;

public:
	// Constructor that opens the log file
	DeferredLogger() {
		log_file.open("deferred_log.txt");
	}

	// Function to log data with deferred locking
	void log_data(const std::string& thread_id, int value) {
		// Create a unique_lock that will defer locking until lock() is called
		std::unique_lock<std::mutex> lock(file_mutex, std::defer_lock);
		// Lock the mutex
		lock.lock();
		// Print the data
		log_file << "From " << thread_id << ": " << value << std::endl;
	}

	// Function to transfer mutex ownership
	std::unique_lock<std::mutex> transfer_lock() {
		// Return a unique_lock that owns the mutex
		return std::unique_lock<std::mutex>(file_mutex);
	}
};

int main() {
	DeferredLogger deferred_logger;
	// Transfer mutex ownership
	std::unique_lock<std::mutex> lock = deferred_logger.transfer_lock();

	// Perform operations while holding the lock
	// ...

	// Unlock before destruction
	lock.unlock();

	// Re-lock if needed
	// lock.lock();

	return 0;
}

/*
 * Example of using std::call_once for thread-safe initialization
 */
class OnceLogger {
	// Mutex to protect the log file
	std::mutex file_mutex;
	// Log file stream
	std::ofstream log_file;
	// Flag to indicate if initialization has occurred
	std::once_flag init_flag;

	// Initialization function
	void initialize() {
		log_file.open("once_log.txt");
	}

public:
	// Function to log data with thread-safe initialization
	void log_data(const std::string& thread_id, int value) {
		// Call the initialization function only once
		std::call_once(init_flag, &OnceLogger::initialize, this);
		// Print the data
		log_file << "From " << thread_id << ": " << value << std::endl;
	}
};

int main() {
	OnceLogger once_logger;

	// Create a thread and pass the functor object to it
	std::thread thread1([&once_logger]() {
		for (int i = -1; i >= -100; --i) {
			once_logger.log_data("Thread 1", i);
		}
	});

	// Main thread loop
	for (int i = 0; i < 100; ++i) {
		once_logger.log_data("Main Thread", i);
	}

	// Wait for the thread to finish
	thread1.join();
	return 0;
}

