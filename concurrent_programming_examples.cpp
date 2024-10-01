#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <exception>

// Example 1: Basic Thread Usage
// This example demonstrates how to create a thread and
// wait for it to finish using the join() method.
void printMessage() {
	// This function is called by the thread and prints
	// a message to the console.
	cout << "Hello, World!" << endl;
}

int main() {
	// Create a thread and pass the printMessage function as
	// its argument.
	std::thread t1(printMessage);

	// Wait for the thread to finish using the join() method.
	// This will block the main thread until the thread
	// has finished.
	t1.join();

	// The thread has finished, so the main thread can
	// continue executing.
	return 0;
}

// Example 2: Race Condition
// This example demonstrates a race condition, which
// occurs when two threads access shared resources and
// one of the threads modifies the resource without the
// other thread knowing about it.
class FileWriter {
	ofstream& fileStream;
public:
	// Constructor that takes a reference to an ofstream
	// object as its argument.
	FileWriter(ofstream& fs) : fileStream(fs) {}
	// Operator() is called by the thread and writes
	// data to the file.
	void operator()() {
		for (int i = 0; i > -100; --i) {
			// Write data to the file, but do not
			// synchronize with the other thread.
			fileStream << "From thread: " << i << endl;
		}
	}
};

int main() {
	// Create a file stream object and open the file.
	ofstream logFile("log.txt");

	// Create a thread and pass the FileWriter object
	// as its argument.
	FileWriter writer(logFile);
	std::thread t1(writer);

	// The main thread also writes data to the file,
	// but does not synchronize with the other thread.
	for (int i = 0; i < 100; ++i) {
		logFile << "From main: " << i << endl;
	}

	// The main thread has finished writing data,
	// so wait for the thread to finish using the
	// join() method.
	t1.join();

	// Close the file stream.
	logFile.close();

	return 0;
}

// Example 3: Handling Exceptions
// This example demonstrates how to handle exceptions
// when working with threads.
int main() {
	// Create a file stream object and open the file.
	ofstream logFile("log.txt");

	// Create a thread and pass the FileWriter object
	// as its argument.
	FileWriter writer(logFile);
	std::thread t1(writer);

	try {
		// The main thread writes data to the file,
		// but may throw an exception.
		for (int i = 0; i < 100; ++i) {
			logFile << "From main: " << i << endl;
			// Simulate an exception.
			if (i == 50) {
				throw std::runtime_error("Simulated exception");
			}
		}
	} catch (...) {
		// If an exception occurs, wait for the thread
		// to finish using the join() method and then
		// rethrow the exception.
		t1.join();
		throw;
	}

	// If no exception occurs, wait for the thread
	// to finish using the join() method.
	t1.join();

	// Close the file stream.
	logFile.close();

	return 0;
}

// Example 4: RAII for Thread Management
// This example demonstrates how to use RAII (Resource
// Acquisition Is Initialization) to manage threads.
class ThreadGuard {
	std::thread& t;
public:
	// Constructor that takes a reference to a thread
	// object as its argument.
	explicit ThreadGuard(std::thread& t_) : t(t_) {}
	// Destructor that waits for the thread to finish
	// using the join() method.
	~ThreadGuard() {
		if (t.joinable()) {
			t.join();
		}
	}
};

int main() {
	// Create a file stream object and open the file.
	ofstream logFile("log.txt");

	// Create a thread and pass the FileWriter object
	// as its argument.
	FileWriter writer(logFile);
	std::thread t1(writer);
	// Create a ThreadGuard object and pass the thread
	// object as its argument. The destructor of the
	// ThreadGuard object will wait for the thread to
	// finish using the join() method.
	ThreadGuard guard(t1);

	// The main thread writes data to the file.
	for (int i = 0; i < 100; ++i) {
		logFile << "From main: " << i << endl;
	}

	// The destructor of the ThreadGuard object will
	// wait for the thread to finish.
	logFile.close();

	return 0;
}

// Example 5: Passing Parameters to Threads
// This example demonstrates how to pass parameters
// to threads using std::ref.
void modifyMessage(string& msg) {
	// Modify the message.
	msg = "Beauty is only skin-deep";
	cout << "Thread says: " << msg << endl;
}

int main() {
	// Create a string object and initialize it.
	string message = "A friend in need is a friend indeed.";

	// Create a thread and pass the modifyMessage
	// function and the string object as its arguments.
	std::thread t1(modifyMessage, std::ref(message));

	// Wait for the thread to finish using the join()
	// method.
	t1.join();

	// Print the modified message.
	cout << "Main says: " << message << endl;

	return 0;
}

// Example 6: Passing Class Methods to Threads
// This example demonstrates how to pass class methods
// to threads.
class MessageModifier {
public:
	void modifyMessage(string* msg) {
		// Modify the message.
		*msg = "Beauty is only skin-deep";
		cout << "Thread says: " << *msg << endl;
	}
};

int main() {
	// Create a string object and initialize it.
	string message = "A friend in need is a friend indeed.";

	// Create an object of the MessageModifier class.
	MessageModifier modifier;

	// Create a thread and pass the modifyMessage
	// method of the MessageModifier class as its
	// argument.
	std::thread t1(&MessageModifier::modifyMessage, &modifier, &message);

	// Detach the thread from the main thread.
	t1.detach();

	// Print the modified message.
	cout << "Main says: " << message << endl;

	return 0;
}

// Example 7: Moving Parameters to Threads
// This example demonstrates how to move parameters
// to threads using std::move.
void printMessage(string msg) {
	// Print the message.
	cout << "Thread says: " << msg << endl;
}

int main() {
	// Create a string object and initialize it.
	string* messagePtr = new string("A friend in need is a friend indeed.");

	// Create a thread and pass the printMessage
	// function and the string object as its arguments.
	std::thread t1(printMessage, std::move(*messagePtr));

	// Wait for the thread to finish using the join()
	// method.
	t1.join();

	// Print the modified message.
	cout << "Main: " << *messagePtr << endl;

	// Delete the string object.
	delete messagePtr;

	return 0;
}

// Example 8: Moving Thread Objects
// This example demonstrates how to move thread objects
// using std::move.
void printMessage() {
	// Print a message.
	cout << "Thread says: Hello, World!" << endl;
}

int main() {
	// Create a thread and pass the printMessage
	// function as its argument.
	std::thread t1(printMessage);

	// Move the thread object to another thread.
	std::thread t2 = std::move(t1);

	// Wait for the thread to finish using the join()
	// method.
	t2.join();

	return 0;
}

// Example 9: RAII with Move Semantics
// This example demonstrates how to use RAII (Resource
// Acquisition Is Initialization) with move semantics
// to manage threads.
class ThreadGuard {
	std::thread t;
public:
	// Constructor that takes a thread object as its
	// argument using move semantics.
	explicit ThreadGuard(std::thread t_) : t(std::move(t_)) {}
	// Destructor that waits for the thread to finish
	// using the join() method.
	~ThreadGuard() {
		if (t.joinable()) {
			t.join();
		}
	}
};

int main() {
	// Create a thread and pass the printMessage
	// function as its argument.
	std::thread t1(printMessage);

	// Create a ThreadGuard object and pass the thread
	// object as its argument using move semantics.
	ThreadGuard guard(std::move(t1));

	return 0;
}

