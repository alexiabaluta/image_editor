// Copyright 2021 Alexia-Elena Baluta 311CA

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include "env.h"

// SELECT
void select_all(image_t *image, image_t *select);

int are_coordinates_valid(image_t *image, int x1, int y1, int x2, int y2);

int is_number(char *s);

void command_select(image_t *image, image_t *select);

// LOAD
int get_file_type(FILE *file);

void skip_comment(FILE *file);

void command_load(image_t *image, image_t *select);

void build_select(image_t *image, image_t *select, int x1, int y1, int x2,
				  int y2);

// ROTATE
void rotate90_square(int n, unsigned char **a, int num_of_colors);

void rotate90_all(image_t *image);

void command_rotate(image_t *image, image_t *select);

// CROP
void command_crop(image_t *image, image_t *select);

// GRAYSCALE
void command_grayscale(image_t *select);

// SEPIA
void command_sepia(image_t *select);

// SAVE
void command_save(image_t *image);

// EXIT
int command_exit(image_t *imag, image_t *select);

#endif
