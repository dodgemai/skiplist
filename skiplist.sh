#!/bin/bash
# The above tells the shell that this is a bash file
# removes skiplist executable file if I accidentally uploaded it
rm -f skiplist
# compiles skiplist executable
gcc --std=c11 -g -o /autograder/submission/skiplist /autograder/submission/readin.c
# runs executable 
/autograder/submission/skiplist