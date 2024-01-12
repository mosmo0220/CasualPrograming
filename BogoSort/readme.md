# BogoSort Multithreaded Test

The `main` function in the given code is responsible for running the BogoSort algorithm on a range of input sizes and recording the time taken to sort each input. It uses multithreading to perform multiple tests in parallel and writes the results to an output file.

## Inputs

The `main` function takes user input for the maximum length of the list to be sorted using BogoSort. It also reads the number of hardware threads available on the system.

## Flow

1. The function opens an output file for writing the results.
2. It determines the number of hardware threads available and checks if it is zero.
3. It prompts the user to provide the maximum length of the list to be sorted.
4. It checks if the user input is valid and greater than 2.
5. It initializes a vector to store the total time taken for each input size.
6. It creates a mutex to synchronize access to the output file.
7. It iterates over a range of input sizes from 2 to the maximum length provided by the user.
8. For each input size, it creates multiple futures to run BogoSort tests in parallel.
9. Each future runs a lambda function that generates random numbers, writes the starting array to the output file, performs BogoSort on the array, writes the sorted array to the output file, and calculates the time taken.
10. After all the futures have completed, the average time taken for each input size is calculated and written to the output file.
11. The output file is closed.

## Outputs

The `main` function writes the results of the BogoSort tests to an output file. It includes the starting and sorted arrays for each test, as well as the average time taken to sort each input size.

## Usage example

```
Provide max length of list for BogoSort: 5
```

This code prompts the user to enter the maximum length of the list to be
sorted using BogoSort. If the user enters 5, the code will perform BogoSort
tests on lists of lengths 2, 3, 4 and 5. The results will be written to the
"results_output.txt" file.

## Time Complexity:

Worst Case: O(∞) (since this algorithm has no upper bound)
Average Case: O(n*n!)
Best Case: O(n)(when the array given is already sorted)

### Docs maded with help CodiumAI