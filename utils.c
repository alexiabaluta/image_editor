#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "matrix_utils.h"
#include "env.h"
#include "image.h"
#include "commands.h"

// Interschimba valoarea a doua numere intregi.
void swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

// Interschimba valoarile a doua caractere.
void swap_char(unsigned char *a, unsigned char *b)
{
	unsigned char aux = *a;
	*a = *b;
	*b = aux;
}

// Returneaza valoarea minima dintre doua numere intregi.
int min_int(int a, int b)
{
	return a < b ? a : b;
}

// Calculeaza modulul unui numar intreg.
int abs(int a)
{
	return a > 0 ? a : (-1 * a);
}

// Copiaza datele unui bloc de memorie sursa, de dimensiune adresa date,
// peste un bloc de memorie destinatie.
void mem_cpy(void *dst, const void *src, int num_bytes)
{
	// Datele de intrare nu sunt valide.
	if (!dst || !src)
		return;

	char *dst_byte = (char *)dst;
	char *src_byte = (char *)src;

	// Se itereaza fiecare octet si se copiaza octetul din sursa in destinatie
	for (int i = 0; i < num_bytes; i++) {
		*dst_byte = *src_byte;
		dst_byte++;
		src_byte++;
	}
}

// Trece pe urmatoarea linie.
void skip_to_line(void)
{
	char rubbish;
	do {
		scanf("%c", &rubbish);
	} while (rubbish != '\n');
}
