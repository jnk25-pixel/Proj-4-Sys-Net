/**
 * @file matrix.cpp
 * @brief Measure row-major and column-major access to a large character matrix.
 *
 * Each row occupies 4096 bytes, and the matrix begins at a 4096-byte boundary.
 * Volatile accesses ensure that optimized builds still perform every matrix
 * read and write. On a system with 4096-byte pages, each row occupies one page.
 *
 * @author Layla Wilson
 * @author Jazmyne Kenny
 * @bug No known bugs.
 */

#define _POSIX_C_SOURCE 200809L

#include <cstddef>
#include <cstdio>
#include <ctime>

/** @brief Number of rows in the matrix. */
constexpr std::size_t ROWS = 20480;
/** @brief Number of characters in each row. */
constexpr std::size_t COLUMNS = 4096;
/** @brief Number of complete passes in each timing measurement. */
constexpr int REPETITIONS = 10;
/** @brief Number of nanoseconds in one second. */
constexpr double NANOSECONDS_PER_SECOND = 1000000000.0;

/**
 * @brief Global matrix used by all four experiments.
 *
 * Alignment ensures that a row does not cross a 4096-byte page boundary.
 * The actual operating-system page size may differ from 4096 bytes.
 */
alignas(4096) volatile char matrix[ROWS][COLUMNS];

/**
 * @brief Write every element of the matrix in row-major order.
 * @param value Character to write to each element.
 * @return Nothing.
 */
void write_row(char value)
{
    for (std::size_t row = 0; row < ROWS; ++row) {
        for (std::size_t column = 0; column < COLUMNS; ++column) {
            matrix[row][column] = value;
        }
    }
}

/**
 * @brief Write every element of the matrix in column-major order.
 * @param value Character to write to each element.
 * @return Nothing.
 */
void write_column(char value)
{
    for (std::size_t column = 0; column < COLUMNS; ++column) {
        for (std::size_t row = 0; row < ROWS; ++row) {
            matrix[row][column] = value;
        }
    }
}

/**
 * @brief Read every matrix element in row-major order.
 * @param value Unused; retained for a common experiment function signature.
 * @return Nothing.
 */
void read_row(char value)
{
    (void)value;
    for (std::size_t row = 0; row < ROWS; ++row) {
        for (std::size_t column = 0; column < COLUMNS; ++column) {
            (void)matrix[row][column];
        }
    }
}

/**
 * @brief Read every matrix element in column-major order.
 * @param value Unused; retained for a common experiment function signature.
 * @return Nothing.
 */
void read_column(char value)
{
    (void)value;
    for (std::size_t column = 0; column < COLUMNS; ++column) {
        for (std::size_t row = 0; row < ROWS; ++row) {
            (void)matrix[row][column];
        }
    }
}

/**
 * @brief Measure ten complete passes of one matrix operation.
 *
 * CLOCK_MONOTONIC is unaffected by changes to the wall clock. The two write
 * operations alternate the written character between passes. The timing
 * includes the loop and function-call overhead, but excludes output.
 *
 * @param operation Matrix operation to repeat.
 * @param average_seconds Receives the mean elapsed time per complete pass.
 * @return True on success; false if either clock_gettime call fails.
 */
bool measure(void (*operation)(char), double &average_seconds)
{
    timespec start{};
    timespec end{};
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        std::perror("clock_gettime");
        return false;
    }

    for (int repetition = 0; repetition < REPETITIONS; ++repetition) {
        operation(repetition % 2 == 0 ? 'A' : 'B');
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        std::perror("clock_gettime");
        return false;
    }

    const double elapsed = static_cast<double>(end.tv_sec - start.tv_sec)
        + static_cast<double>(end.tv_nsec - start.tv_nsec)
            / NANOSECONDS_PER_SECOND;
    average_seconds = elapsed / REPETITIONS;
    return true;
}

/**
 * @brief Run the four experiments and print the average seconds per pass.
 * @return Zero on success; nonzero if the clock cannot be read.
 */
int main()
{
    double average_seconds = 0.0;

    if (!measure(write_row, average_seconds)) {
        return 1;
    }
    std::printf("Write Row: %.9f seconds\n", average_seconds);

    if (!measure(write_column, average_seconds)) {
        return 1;
    }
    std::printf("Write Col: %.9f seconds\n", average_seconds);

    if (!measure(read_row, average_seconds)) {
        return 1;
    }
    std::printf("Read Row:  %.9f seconds\n", average_seconds);

    if (!measure(read_column, average_seconds)) {
        return 1;
    }
    std::printf("Read Col:  %.9f seconds\n", average_seconds);
    return 0;
}
