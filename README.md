# cs-1550-Project-3-vm-simulator

[link to project prompt](https://people.cs.pitt.edu/~jmisurda/teaching/cs1550/2167/cs1550-2167-project3.htm)

Due: Sunday, July 17, 2016, by 11:59pm

The following text is a summary of the information from the prompt.

Implement a page table for a 32-bit address space. All pages will be 4KB in size. The number of frames will be a parameter to the execution of your program. Project to implement four page replacement algorithms:

  1. Opt – Simulate what the optimal page replacement algorithm would choose if it had perfect knowledge 
  2. Clock – Use the better implementation of the second-chance algorithm 
  3. Aging – Implement the aging algorithm that approximates LRU with an 8-bit counter
  4. LRU – Do exact LRU.

Program may be written in C/C++, Java, Perl, or Python as long as it runs on thoth.cs.pitt.edu.




Program must run with the following command:
`./vmsim –n <numframes> -a <opt|clock|aging|lru> [-r <refresh>] <tracefile>`

The program will then run through the memory references of the file and display the action taken for each address:
 * hit
 * page fault – no eviction
 * page fault – evict clean
 * page fault – evict dirty 

When the trace is over, print out summary statistics in the following format:
```
Algorithm: Clock
Number of frames:       8
Total memory accesses:  1000000
Total page faults:      181856
Total writes to disk:   29401
```

Provided are two sample memory traces available at `/u/OSLab/original/swim.trace.gz` `/u/OSLab/original/gcc.trace.gz`. This project will grade with two additional ones. 

In the resulting trace file is a sequence of lines, each having a memory address in hexadecimal followed by a R or W character to indicate if that access was a read or a write. For example, gcc.trace trace starts like this: 
  ```
  0041f7a0 R
  13f5e2c0 R
  ```

If you are writing in C, you may parse each line with the following code:

```C
unsigned int address;
char mode;

fscanf(file, "%x %c", &addr, &mode);
```

## Write Up

For Aging, you have a refresh parameter to set. Try to find a good refresh period that works well. You do not need to find the absolute minimum, just approximately how long to wait. Plot your results in a graph and discuss why your choice of refresh seemed to be the best.

For each of your four algorithms (with Aging using the proper refresh you determined), describe in a document the resulting page fault statistics for 8, 16, 32, and 64 frames. Use this information to determine which algorithm you think might be most appropriate for use in an actual operating system. Use OPT as the baseline for your comparisons.


## Requirements and Submission
You need to submit:
*	Your well-commented program’s source
*	A document (.DOC or .PDF) detailing the results of your simulation with a graph plotting the number of page faults versus the number of frames and your conclusions on which algorithm would be best to use in a real OS
* Make a tar.gz file as in the first assignment, named bhw7-project3.tar.gz
  *	DO NOT submit the trace files.
* Copy it to ~jrmst106/submit/1550 by the deadline for credit.


