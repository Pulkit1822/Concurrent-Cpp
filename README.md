<p align="center">
  <a href="https://github.com/Pulkit1822/Concurrent-Cpp">
  <img src="https://github.com/Pulkit1822/Concurrent-Cpp/blob/main/content/logo.jpg" height="128">
  </a>
  <h1 align="center">C++ Concurrency</h1>
</p>


## Overview

This repository contains various examples and demonstrations of concurrent programming in C++. The examples cover a wide range of topics, including basic thread usage, thread synchronization, handling race conditions, and using various synchronization primitives like mutexes, condition variables, futures, promises, and more.

## Files and Topics

### 1. `CONCURRENT_PROGRAMMING_EXAMPLES.CPP`

This file contains multiple examples demonstrating different aspects of concurrent programming in C++. Each example is designed to illustrate a specific concept or technique.

- **Example 1: Basic Thread Usage**
  - Demonstrates how to create and join a thread.
  - ```cpp
    std::thread t1(printMessage);
    t1.join();
    ```

- **Example 2: Race Condition**
  - Shows how race conditions can occur when multiple threads access shared resources without proper synchronization.
  - ```cpp
    class FileWriter {
        ofstream& fileStream;
        // ...
    };
    ```

- **Example 3: Handling Exceptions**
  - Illustrates how to handle exceptions in a multithreaded environment.
  - ```cpp
    try {
        // ...
    } catch (...) {
        t1.join();
        throw;
    }
    ```

- **Example 4: RAII for Thread Management**
  - Uses RAII (Resource Acquisition Is Initialization) to manage thread lifetimes.
  - ```cpp
    class ThreadGuard {
        std::thread& t;
        // ...
    };
    ```

- **Example 5: Passing Parameters to Threads**
  - Demonstrates how to pass parameters to threads.
  - ```cpp
    std::thread t1(modifyMessage, std::ref(message));
    ```

- **Example 6: Passing Class Methods to Threads**
  - Shows how to pass class member functions to threads.
  - ```cpp
    std::thread t1(&MessageModifier::modifyMessage, &modifier, &message);
    ```

- **Example 7: Moving Parameters to Threads**
  - Illustrates how to move parameters to threads to avoid copying.
  - ```cpp
    std::thread t1(printMessage, std::move(*messagePtr));
    ```

- **Example 8: Moving Thread Objects**
  - Demonstrates how to transfer ownership of a thread using `std::move`.
  - ```cpp
    std::thread t2 = std::move(t1);
    ```

- **Example 9: RAII with Move Semantics**
  - Combines RAII and move semantics for thread management.
  - ```cpp
    class ThreadGuard {
        std::thread t;
        // ...
    };
    ```

### 2. `THREAD_SYNCHRONIZATION_DEMO.CPP`

This file provides examples of thread synchronization using various synchronization primitives.

- **Producer-Consumer Problem**
  - Uses a `std::deque`, `std::mutex`, and `std::condition_variable` to synchronize producer and consumer threads.
  - ```cpp
    std::deque<int> dataQueue;
    std::mutex dataMutex;
    std::condition_variable dataCondVar;
    ```

- **Using `std::future` and `std::promise`**
  - Demonstrates how to use futures and promises for asynchronous data provision.
  - ```cpp
    std::future<int> futureResult = std::async(std::launch::async, factorial, 5);
    ```

- **Using `std::shared_future`**
  - Shows how to share the result of a future among multiple threads.
  - ```cpp
    std::shared_future<int> sharedFuture = sharedPromise.get_future().share();
    ```

- **Using `std::packaged_task`**
  - Packages a function and its arguments for asynchronous execution.
  - ```cpp
    std::packaged_task<int(int)> task(factorial);
    ```

### 3. `THREAD_SYNCHRONIZATION_EXAMPLE.CPP`

This file contains examples of thread synchronization and avoiding common pitfalls like deadlocks.

- **Basic Thread Synchronization**
  - Uses a global mutex to synchronize access to shared data.
  - ```cpp
    std::mutex global_mutex;
    ```

- **Avoiding Global Variables**
  - Bundles a mutex with the resource it protects to avoid global variables.
  - ```cpp
    class Logger {
        std::mutex file_mutex;
        std::ofstream log_file;
        // ...
    };
    ```

- **Avoiding Deadlocks**
  - Uses `std::lock` to avoid deadlocks when locking multiple mutexes.
  - ```cpp
    std::lock(mutex1, mutex2);
    ```

- **Deferred Locking with `std::unique_lock`**
  - Demonstrates deferred locking and transferring ownership of a lock.
  - ```cpp
    std::unique_lock<std::mutex> lock(file_mutex, std::defer_lock);
    ```

- **Thread-Safe Initialization with `std::call_once`**
  - Uses `std::call_once` to ensure a piece of code is executed only once.
  - ```cpp
    std::call_once(init_flag, &OnceLogger::initialize, this);
    ```

## Conclusion

These examples provide a comprehensive overview of concurrent programming in C++. By studying and experimenting with these examples, you can gain a deeper understanding of how to write efficient and safe multithreaded code in C++.

## Feedback

If you have any feedback, suggestions, or questions regarding this repository, please feel free to open an issue or contact me below on any of the platforms you prefer 😊
<br/>
<p align="center">
  <a href="https://pulkitmathur.tech/"><img src="https://github.com/Pulkit1822/Pulkit1822/blob/main/animated-icons/pic.jpeg" alt="portfolio" width="32"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://www.linkedin.com/in/pulkitkmathur/"><img src="https://github.com/TheDudeThatCode/TheDudeThatCode/blob/master/Assets/Linkedin.svg" alt="Linkedin Logo" width="32"></a>&nbsp;&nbsp;&nbsp;
  <a href="mailto:pulkitmathur.me@gmail.com"><img src="https://github.com/TheDudeThatCode/TheDudeThatCode/blob/master/Assets/Gmail.svg" alt="Gmail logo" height="32"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://www.instagram.com/pulkitkumarmathur/"><img src="https://github.com/TheDudeThatCode/TheDudeThatCode/blob/master/Assets/Instagram.svg" alt="Instagram Logo" width="32"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://in.pinterest.com/pulkitkumarmathur/"><img src="https://upload.wikimedia.org/wikipedia/commons/0/08/Pinterest-logo.png?20160129083321" alt="Pinterest Logo" width="32"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://twitter.com/pulkitkmathur"><img src="https://upload.wikimedia.org/wikipedia/commons/5/57/X_logo_2023_%28white%29.png" alt="Twitter Logo" width="32"></a>&nbsp;&nbsp;&nbsp;
</p>



Happy learning and coding!

---

If you find this repository useful, don't forget to star it! ⭐️

### Written by [Pulkit](https://github.com/Pulkit1822)