// Copyright 2021 Alexia-Elena Baluta 311CA

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "matrix_utils.h"
#include "env.h"
#include "commands.h"

// Returneaza o imagine cu valori nule.
image_t init_image(void)
{
	image_t image = {NULL, 0, 0, 0, 0, 0, 0, 0};
	return image;
}

// Elibereaza memoria si reinitializeaza cu valori nule o imagine.
void destroy_image(image_t *image)
{
	image->img_grid = free_matrix(image->height, image->img_grid);
	*image = init_image();
}

// Elibereaza memoria si reinitializeaza cu valori nule o selectie.
void destroy_select(image_t *select)
{
	free(select->img_grid);
	*select = init_image();
}

// Construieste o imagine.
void build_image(image_t *image, int height, int width, int type)
{
	destroy_image(image);

	image->type = type;

	// Verifica daca o imagine este sub format RGB sau GRAYSCALE.
	image->is_rgb = FALSE;
	if (image->type == P3 || image->type == P6)
		image->is_rgb = TRUE;

	// In functie de tip, un pixel e reprezentat printr-o valoare sau trei.
	image->num_of_colors = image->is_rgb ? 3 : 1;

	// In height si width se salveaza numarul de valori.
	// Pentru latime este necesara convertirea din numarul de pixeli.
	image->width = width * image->num_of_colors;
	image->height = height;

	image->origin_x = 0;
	image->origin_y = 0;

	image->img_grid = alloc_matrix(image->height, image->width);
}

// Construieste o selectie.
void build_select(image_t *image, image_t *select, int x1, int y1, int x2,
				  int y2)
{
	destroy_select(select);

	// Copiaza atributele pe care o selectie le mosteneste de la imagine.
	select->type = image->type;
	select->is_rgb = image->is_rgb;
	select->num_of_colors = image->num_of_colors;

	// Calculeaza atributele care depind de coordonate.
	// Converteste din numarul de pixeli in numarul de valori.
	select->width = abs(x1 - x2) * select->num_of_colors;
	select->height = abs(y1 - y2);
	select->origin_x = min_int(x1, x2) * select->num_of_colors;
	select->origin_y = min_int(y1, y2);

	// Face o copiere shallow a matricei imaginii.
	// (Este nevoie ca transformarile facute pe selectie sa se reflecte asupra
	// imaginii)
	select->img_grid = (unsigned char **)malloc(select->height
												* sizeof(unsigned char *));
	if (!select->img_grid) {
		select->img_grid = NULL;
		return;
	}
	for (int i = 0; i < select->height; i++)
		select->img_grid[i] = image->img_grid[select->origin_y + i]
							  + select->origin_x;
}
