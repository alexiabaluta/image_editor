// Copyright 2021 Alexia-Elena Baluta 311CA

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Aloca dinamic o matrice.
// Primeste ca parametru dimensiunile.
unsigned char **alloc_matrix(int n, int m)
{
	unsigned char **a = (unsigned char **)malloc(n * sizeof(unsigned char *));
	if (!a) {
		fprintf(stderr, "malloc() failed");
		return NULL;
	}

	for (int i = 0; i < n; i++) {
		a[i] = (unsigned char *)malloc(m * sizeof(unsigned char));
		if (!a) {
			fprintf(stderr, "malloc() failed");
			return NULL;
		}
	}

	return a;
}

// Citeste o matrice din fisier text.
void read_matrix(int n, int m, unsigned char **a, FILE *file)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			int x;
			fscanf(file, "%d", &x);
			a[i][j] = (unsigned char)x;
		}
}

// Citeste o matrice din fisier binar.
void read_matrix_binary(int n, int m, unsigned char **a, FILE *file)
{
	for (int i = 0; i < n; i++)
		fread(a[i], 1, m, file);
}

//	functie care primeste toate datele necesare pentru
//	eliberarea unei matrice alocata cu alloc_matrix
unsigned char **free_matrix(int n, unsigned char **a)
{
	if (a) {
		for (int i = 0; i < n; i++)
			if (a[i])
				free(a[i]);
		free(a);
	}
	return NULL;
}
