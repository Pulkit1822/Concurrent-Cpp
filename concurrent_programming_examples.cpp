#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <exception>

using namespace std;

// Example 1: Basic Thread Usage
void printMessage() {
	cout << "Hello, World!" << endl;
}

int main() {
	std::thread t1(printMessage);
	t1.join(); // Main thread waits for t1 to finish
	// t1.detach(); // Main thread lets t1 run independently
	return 0;
}

// Example 2: Race Condition
class FileWriter {
	ofstream& fileStream;
public:
	FileWriter(ofstream& fs) : fileStream(fs) {}
	void operator()() {
		for (int i = 0; i > -100; --i) {
			fileStream << "From thread: " << i << endl;
		}
	}
};

int main() {
	cout << "Starting main thread" << endl;
	ofstream logFile("log.txt");

	FileWriter writer(logFile);
	std::thread t1(writer);

	for (int i = 0; i < 100; ++i) {
		logFile << "From main: " << i << endl;
	}

	t1.join();
	logFile.close();

	return 0;
}

// Example 3: Handling Exceptions
int main() {
	cout << "Starting main thread" << endl;
	ofstream logFile("log.txt");

	FileWriter writer(logFile);
	std::thread t1(writer);

	try {
		for (int i = 0; i < 100; ++i) {
			cout << "From main: " << i << endl; // Exception might occur here
		}
	} catch (...) {
		t1.join();
		throw;
	}

	t1.join();
	logFile.close();

	return 0;
}

// Example 4: RAII for Thread Management
class ThreadGuard {
	std::thread& t;
public:






	explicit ThreadGuard(std::thread& t_) : t(t_) {}
	~ThreadGuard() {
		if (t.joinable()) {
			t.join();
		}
	}
};

int main() {
	cout << "Starting main thread" << endl;
	ofstream logFile("log.txt");

	FileWriter writer(logFile);
	std::thread t1(writer);
	ThreadGuard guard(t1);

	for (int i = 0; i < 100; ++i) {
		cout << "From main: " << i << endl;
	}

	logFile.close();
	return 0;
}

// Example 5: Passing Parameters to Threads
void modifyMessage(string& msg) {
	msg = "Beauty is only skin-deep";
	cout << "Thread says: " << msg << endl;
}

int main() {
	string message = "A friend in need is a friend indeed.";
	std::thread t1(modifyMessage, std::ref(message));
	t1.join();

	cout << "Main says: " << message << endl;
	return 0;
}

// Example 6: Passing Class Methods to Threads
class MessageModifier {
public:
	void modifyMessage(string* msg) {
		*msg = "Beauty is only skin-deep";
		cout << "Thread says: " << *msg << endl;
	}
};

int main() {
	string message = "A friend in need is a friend indeed.";
	MessageModifier modifier;
	std::thread t1(&MessageModifier::modifyMessage, &modifier, &message);
	t1.detach();

	cout << "Main says: " << message << endl;
	return 0;
}

// Example 7: Moving Parameters to Threads
void printMessage(string msg) {
	cout << "Thread says: " << msg << endl;
}

int main() {
	string* messagePtr = new string("A friend in need is a friend indeed.");
	std::thread t1(printMessage, std::move(*messagePtr));
	t1.join();

	cout << "Main: " << *messagePtr << endl;
	delete messagePtr;
	return 0;
}

// Example 8: Moving Thread Objects
void printMessage() {
	cout << "Thread says: Hello, World!" << endl;
}

int main() {
	std::thread t1(printMessage);
	std::thread t2 = std::move(t1); // t2 becomes the owner of the thread
	t2.join();

	return 0;
}

// Example 9: RAII with Move Semantics
class ThreadGuard {
	std::thread t;
public:
	explicit ThreadGuard(std::thread t_) : t(std::move(t_)) {}
	~ThreadGuard() {
		if (t.joinable()) {
			t.join();
		}
	}
};

int main() {
	std::thread t1(printMessage);
	ThreadGuard guard(std::move(t1));

	return 0;
}
