# Edmonds-Karp
Implementation of Edmonds-Karp algorithm in C++ for the course of Analisys and Synthesis of Algorithms.
The program splits a matrix between primary and scenario entries, by calculating the minimum sum of 
    -primary values (regarding primary matrix entries),
    -background values (regarding secondary matrix entries) and 
    -all transition values between a primary and a background matrix entry.

Linux:
  Compile:
    g++ -Wall -ansi -pedantic al011.cpp
   
  Run:
    ./a.out < input_file > output_file
    
    (Default input and output are stdin and stdout)
    
Input:

  -two integers, m and n, representing the number of lines and columns in the matrix, followed by one empty line(\n).
  -Two matrixes, each m*n, separated by an empty line, representing the primary and background values (respectly) of each matrix entry.
  -One m*(n-1) matrix, representing the transition values from one matrix entry to the horizontally adjacent ones.
  -One (m-1)*n matrix, representing the transition values from one matrix entry to the vertically adjacent ones.

Output:

  One integer, representing the sum of:
    -primary values (regarding primary matrix entries),
    -background values (regarding secondary matrix entries) and 
    -all transition values between a primary and a background matrix entry.
  
  One m*n matrix with C's and P's, where:
    -C stands for scenario matrix entry and
    -P stands for primary matrix  program.

Input example:

5 5

8 7 9 9 7
6 2 2 8 7
9 1 2 1 8
2 1 3 1 7
1 3 2 1 9

2 1 2 3 2
1 9 9 1 3
1 7 7 9 3
8 7 9 7 2
7 9 8 9 1

8 9 7 6
1 9 0 8
1 8 9 2
8 7 9 1
9 8 7 2

8 2 1 7 9
7 8 7 1 8
2 8 7 7 8
9 7 8 9 8

Output:

57

C C C C C
C P P C C
C P P P C
P P P P C
P P P P C
