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

// Function to print shared data
void print_shared_data(const std::string& thread_id, int value) {
	std::lock_guard<std::mutex> lock(global_mutex);
	std::cout << "From " << thread_id << ": " << value << std::endl;
}

// Functor class for thread execution
class Task {
public:
	void operator()() {
		for (int i = -1; i >= -100; --i) {
			print_shared_data("Thread 1", i);
		}
	}
};

int main() {
	Task task;
	std::thread thread1(task);

	for (int i = 0; i < 100; ++i) {
		print_shared_data("Main Thread", i);
	}

	thread1.join();
	return 0;
}

/*
 * Avoiding global variables and bundling mutex with the resource it protects
 */
class Logger {
	std::mutex file_mutex;
	std::ofstream log_file;

public:
	Logger() {
		log_file.open("log.txt");
	}

	// Function to print to log file with thread safety
	void log(const std::string& thread_id, int value) {
		std::lock_guard<std::mutex> lock(file_mutex);
		log_file << "From " << thread_id << ": " << value << std::endl;
	}

	// Prevent leaking the file stream to the outside world
	void process_file(void (*func)(std::ofstream&)) {
		func(log_file);
	}
};

// Functor class for thread execution with Logger
class LoggerTask {
	Logger& logger;

public:
	LoggerTask(Logger& log) : logger(log) {}

	void operator()() {
		for (int i = -1; i >= -100; --i) {
			logger.log("Thread 1", i);
		}
	}
};

int main() {
	Logger logger;
	LoggerTask logger_task(logger);
	std::thread thread1(logger_task);

	for (int i = 0; i < 100; ++i) {
		logger.log("Main Thread", i);
	}

	thread1.join();
	return 0;
}

/*
 * Example of potential deadlock and its solution using std::lock
 */
class SafeLogger {
	std::mutex mutex1;
	std::mutex mutex2;
	std::ofstream log_file;

public:
	SafeLogger() {
		log_file.open("safe_log.txt");
	}

	// Function to log data with proper mutex locking to avoid deadlock
	void log_data(const std::string& thread_id, int value) {
		std::lock(mutex1, mutex2);
		std::lock_guard<std::mutex> lock1(mutex1, std::adopt_lock);
		std::lock_guard<std::mutex> lock2(mutex2, std::adopt_lock);
		log_file << "From " << thread_id << ": " << value << std::endl;
	}
};

/*
 * Example of using unique_lock for deferred locking and transferring ownership
 */
class DeferredLogger {
	std::mutex file_mutex;
	std::ofstream log_file;

public:
	DeferredLogger() {
		log_file.open("deferred_log.txt");
	}

	// Function to log data with deferred locking
	void log_data(const std::string& thread_id, int value) {
		std::unique_lock<std::mutex> lock(file_mutex, std::defer_lock);
		lock.lock();
		log_file << "From " << thread_id << ": " << value << std::endl;
	}

	// Function to transfer mutex ownership
	std::unique_lock<std::mutex> transfer_lock() {
		return std::unique_lock<std::mutex>(file_mutex);
	}
};

int main() {
	DeferredLogger deferred_logger;
	std::unique_lock<std::mutex> lock = deferred_logger.transfer_lock();

	// Perform operations while holding the lock
	lock.unlock();  // Unlock before destruction

	// Re-lock if needed
	lock.lock();

	return 0;
}

/*
 * Example of using std::call_once for thread-safe initialization
 */
class OnceLogger {
	std::mutex file_mutex;
	std::ofstream log_file;
	std::once_flag init_flag;

	// Initialization function
	void initialize() {
		log_file.open("once_log.txt");
	}

public:
	// Function to log data with thread-safe initialization
	void log_data(const std::string& thread_id, int value) {
		std::call_once(init_flag, &OnceLogger::initialize, this);
		log_file << "From " << thread_id << ": " << value << std::endl;
	}
};

int main() {
	OnceLogger once_logger;

	std::thread thread1([&once_logger]() {
		for (int i = -1; i >= -100; --i) {
			once_logger.log_data("Thread 1", i);
		}
	});

	for (int i = 0; i < 100; ++i) {
		once_logger.log_data("Main Thread", i);
	}

	thread1.join();
	return 0;
}
