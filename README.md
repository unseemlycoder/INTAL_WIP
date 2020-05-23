## INTAL_WIP

A C library for the integers of arbitrary length (intal).

## INTAL
An intal is a nonnegative integer of arbitrary length, but it is sufficient for your implementation to support up to 1000 decimal digits. The integer is stored as a null-terminated string of ASCII characters. An intal is represented as a string of decimal digits ('0' thru '9') that are stored in the big-endian style. That is, the most significant digit is at the head of the string. An integer 25, for example, is stored in a string s as '2' at s[0], '5' at s[1], and null char at s[2].

## intal.h
Header file with instructions for implementing functions

## PESx201800xxx.c
Implementation of said functions

## intal_sampletest.c
Exhaustive set of sample test cases for the functions in intal.h
