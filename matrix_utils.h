// Copyright 2021 Alexia-Elena Baluta 311CA

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

unsigned char **alloc_matrix(int n, int m);

void read_matrix(int n, int m, unsigned char **a, FILE *file);

void read_matrix_binary(int n, int m, unsigned char **a, FILE *file);

void print_matrix(int n, int m, unsigned char **a, FILE *file);

void print_matrix_binary(int n, int m, unsigned char **a, FILE *file);

unsigned char **free_matrix(int n, unsigned char **a);

#endif
