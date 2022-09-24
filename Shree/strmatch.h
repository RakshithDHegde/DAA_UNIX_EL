#ifndef STRMATCH_H
#define STRMATCH_H
#include<iostream>
using namespace std;
# define NO_OF_CHARS 256

int harspool(string txt,string pat,long long int *count);
int boyer_moore( string txt, string pat,long long int *count);
void badCharHeuristic( string str, int size,int badchar[NO_OF_CHARS]);
int calc_hash(string txt, int n);
int rabin_karp(string text,string pat,long long int *count);

#endif