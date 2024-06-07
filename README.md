# Project Description

In modern computing, parallelism plays a crucial role in enhancing the 
performance of various algorithms and tasks. Processes and threads are
two fundamental units of parallel execution in operating systems.
Processes represent independent, isolated units of execution, each with its
own memory space, while threads are lighter-weight units that share the
same memory space within a process.


In this project, we implemented five well-known sorting algorithms: Bubble
Sort, Selection Sort, Insertion Sort, Merge Sort, and Quick Sort. Each
algorithm was implemented both as processes and as threads using
POSIX threading (pthread library). We used large datasets consisting of
random numbers to test the performance of these algorithms. The datasets were provided through files. Datasets included 1000, 10000, 10000,
100000, and 200000 random numbers generated between 1 to 100.


The implementation involved creating functions for each sorting algorithm,
as well as additional functions for thread creation (separate functions were
created for 2 threaded processes and 4 threaded processes). We carefully
designed the code to ensure that the sorting algorithms functioned correctly
in both process and thread environments.


After implementing the algorithms, we conducted experiments to measure
the execution time of each algorithm when implemented as processes and
threads. The execution time was measured using the clock() function from
the C++ standard library.

# For compiling and running

```

g++ ProcessesVSThreads -o exe -lpthread
./exe

```

# Conclusion

Through our project, we have observed distinct trends in the efficiency of threading across different sorting algorithms. Notably, threading does not enhance the efficiency of Merge Sort, hence, implementing it as a serial process is more effective. Conversely, we've found that the benefits of threading are more pronounced with larger datasets, since with smaller datasets, the overhead of managing multiple threads can outweigh the potential gains from parallel execution. This observation underscores the importance of considering dataset size when deciding whether to employ threading. Additionally, as the number of threads increases, particularly with larger datasets, we observe a decrease in execution time, indicating enhanced efficiency. Therefore, it is advisable to apply threading selectively, focusing primarily on larger datasets to maximize performance gains.
