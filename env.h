// Copyright 2021 Alexia-Elena Baluta 311CA

#ifndef ENV_H
#define ENV_H

typedef enum {
	FALSE = 0,
	TRUE = 1
} bool;

typedef enum {
	P1 = 1,
	P2 = 2,
	P3 = 3,
	P4 = 4,
	P5 = 5,
	P6 = 6
} magic_word;

// tip de date care memoreaza o imagine
typedef struct {
	unsigned char **img_grid; // matrice de pixeli
	int width, height; // latimea memoreaza numarul de valori separate
	int origin_x, origin_y; // pozitia relativa a selectiei fata de imagine
	int type; // tipul fisierului, interpretat cu enum-ul magic_word
	int is_rgb;
	int num_of_colors; // numarul de culori ale unui pixel
} image_t;

#endif
