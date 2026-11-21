# Project 4: Memory Management

## Build and run

Run `make`, then `./matrix`. The program prints the average time in seconds
per full matrix pass for each of the four operations, each measured over ten
passes. `make clean` removes the executable. A C++11 compiler and a POSIX
implementation of `clock_gettime` are required. For older Linux systems that
require librt, build with `make LDLIBS=-lrt`.

The program uses a global 20,480 x 4,096 character matrix (80 MiB), aligned
to a 4,096-byte boundary. Volatile matrix accesses prevent the compiler from
removing the individual reads and writes, including with optimization enabled.
The first write also includes first-touch memory costs. Different operating
systems may use pages larger than 4,096 bytes; record the target system's page
size and other relevant conditions when interpreting benchmark results.

## Remaining group work

- Add the group members' names and IDs to the author line in `matrix.cpp`.
- Run the benchmark on the intended test system, record its compiler, options,
  operating system and page size, and make a bar chart from the four results.
- Prepare `reportExperiment.pdf` with the chart, experiment, observations,
  performance implications, conclusions, and any implementation/testing issues.
- Prepare `reportPF.pdf` with the page-fault calculation and illustrations.
- Upload the required individual files to Canvas as instructed by the course.
