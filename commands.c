// Copyright 2020 Alexia-Elena Baluta 311CA

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "matrix_utils.h"
#include "env.h"
#include "image.h"

#define MAX_OPT 256
#define MAGIC_LEN 2
#define EPS 0.5
#define FMAX 256
#define MAX_LINE 256
// valoare data din enuntul problemei
#define MAX_VALUE 255

// Trimite parametrii corespunzatori selectiei intregii imagini.
void select_all(image_t *image, image_t *select)
{
	int x1 = 0, y1 = 0;
	int x2 = image->width / image->num_of_colors, y2 = image->height;

	build_select(image, select, x1, y1, x2, y2);
}

// Verifica daca coordonatele de selectie se afla inauntrul imaginii.
int are_coordinates_valid(image_t *image, int x1, int y1, int x2, int y2)
{
	if ((x1 - x2) * (y1 - y2) == 0)
		return FALSE;
	if (x1 < 0 || x1 > image->width / image->num_of_colors)
		return FALSE;
	if (x2 < 0 || x2 > image->width / image->num_of_colors)
		return FALSE;
	if (y1 < 0 || y1 > image->height)
		return FALSE;
	if (y2 < 0 || y2 > image->height)
		return FALSE;
	return TRUE;
}

// Verifica daca un sir de caractere poate fi parsat ca numar.
// (contine doar cifre, cu exceptia primului caracter care poate fi '-')
int is_number(char *s)
{
	if ((s[0] < '0' || s[0] > '9') && s[0] != '-')
		return FALSE;

	for (int i = 1; i < (int)strlen(s); i++)
		if (s[i] < '0' || s[i] > '9')
			return FALSE;
	return TRUE;
}

// Selecteaza o anumita portiune din imagine
// Salveaza coordonatele selectiei anterioare, parseaza optiunile comenzii
// si trimite parametrii functiei care construieste selectia ceruta.
void command_select(image_t *image, image_t *select)
{
	int x1, y1, x2, y2;
	int x01 = select->origin_x, y01 = select->origin_y,
		x02 = select->width, y02 = select->height;

	char command[MAX_OPT];
	fgets(command, MAX_OPT, stdin);
	command[strlen(command) - 1] = '\n';

	char delim[] = "\n ";
	char *token = strtok(command, delim);

	if (strcmp(token, "ALL") == 0) { // SELECT ALL
		if (!image->img_grid) {
			printf("No image loaded\n");
			return;
		}

		select_all(image, select);
		if (!select->img_grid) {
			build_select(image, select, x01, y01, x02, y02);
			printf("No image loaded\n");
			return;
		}

		printf("Selected ALL\n");
	} else { // SELECT <x1> <y1> <x2> <y2>
		int opts[4];

		for (int i = 0; i < 4; i++) {
			// Verifica daca toti parametrii ceruti exista si sunt numere.
			if (token && is_number(token)) {
				opts[i] = atoi(token);
			} else {
				printf("Invalid command\n");
				return;
			}
			token = strtok(NULL, delim);
		}
		x1 = opts[0];
		y1 = opts[1];
		x2 = opts[2];
		y2 = opts[3];

		if (!image->img_grid) {
			printf("No image loaded\n");
			return;
		}

		if (x1 > x2)
			swap(&x1, &x2);
		if (y1 > y2)
			swap(&y1, &y2);

		if (are_coordinates_valid(image, x1, y1, x2, y2)) {
			build_select(image, select, x1, y1, x2, y2);
			// Daca a esuat comanda, se readuce selectia la stadiul anterior.
			if (!select->img_grid) {
				build_select(image, select, x01, y01, x02, y02);
				printf("No image loaded\n");
					return;
			}

			printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
		} else {
			printf("Invalid set of coordinates\n");
		}
	}
}

// Citeste magicword-ul specific formatului si il returneaza ca intreg.
int get_file_type(FILE *file)
{
	char type[MAGIC_LEN + 1];
	fscanf(file, "%s", type);

	return (int)type[1] - '0';
}

// Citeste un caracter, daca acesta indica un comentariu, atunci se sare linia,
// altfel, se readuce cursorul la pozitia precedenta.
void skip_comment(FILE *file)
{
	char rubbish;
	fscanf(file, " %c", &rubbish);
	if (rubbish == '#') {
		fseek(file, -1, SEEK_CUR);
		char comment[MAX_LINE];
		fgets(comment, MAX_LINE, file);
	} else {
		fseek(file, -1, SEEK_CUR);
	}
}

// Incarca imaginea in memorie.
void command_load(image_t *image, image_t *select)
{
	char file_name[FMAX];
	scanf("%s", file_name);

	// Fisierul este deschis in format citire text.
	FILE *file = fopen(file_name, "rt");
	if (!file) {
		destroy_image(image);
		destroy_select(select);
		printf("Failed to load %s\n", file_name);
		return;
	}

	// Se citesc atributele imaginii.
	int type = get_file_type(file);

	skip_comment(file);

	int width, height;
	fscanf(file, "%d%d", &width, &height);

	// Se construieste imaginea.
	build_image(image, height, width, type);
	if (!image->img_grid) {
		destroy_image(image);
		destroy_select(select);
		printf("Failed to load %s\n", file_name);
		return;
	}

	// Daca este cazul, se citeste si valoarea maxima a unui pixel.
	if (image->type != P1 && image->type != P4) {
		int max_val;
		fscanf(file, "%d", &max_val);
	}

	// Daca matricea e salvata in format text, se continua citirea.
	if (image->type < P4) {
		read_matrix(image->height, image->width, image->img_grid, file);
		fclose(file);

		// Automat, la incarcarea imaginii, aceasta este selectata complet.
		select_all(image, select);
		printf("Loaded %s\n", file_name);
		return;
	}

	// Salveaza pozitia la care a ramas cursorul si se inchide fisierul.
	int cursor_location = ftell(file) + 1;
	fclose(file);

	// Redeschide fisierul in format de citire binara.
	file = fopen(file_name, "rb");
	if (!file) {
		destroy_image(image);
		destroy_select(select);
		printf("Failed to load %s\n", file_name);
		return;
	}
	// Repozitioneaza cursorul.
	fseek(file, cursor_location, SEEK_SET);

	read_matrix_binary(image->height, image->width, image->img_grid, file);
	fclose(file);

	// Automat, la incarcarea imaginii, aceasta este selectata complet.
	select_all(image, select);
	printf("Loaded %s\n", file_name);
}

// Roteste 90°, in sensul acelor de ceasornic, o selectie patratica.
void rotate90_square(int n, unsigned char **a, int num_of_colors)
{
	// Calculeaza transpusa direct pe matricea originala.
	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			for (int k = 0; k < num_of_colors; k++)
				swap_char(&a[i][j * num_of_colors + k],
						  &a[j][i * num_of_colors + k]);

	// Inverseaza liniile.
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n / 2; j++)
			for (int k = 0; k < num_of_colors; k++)
				swap_char(&a[i][j * num_of_colors + k],
						  &a[i][(n - j - 1) * num_of_colors + k]);
}

// Roteste 90°, in sensul acelor de ceasornic, o imagine.
// Utiizeaza o matrice ajutatoare.
void rotate90_all(image_t *image)
{
	// Pentru simplificarea intelgerii codului, se copiaza in variabile noi
	// dimensiunile in pixeli.
	int num_col = image->num_of_colors;
	int height = image->height, width = image->width / num_col;

	unsigned char **grid_dup = alloc_matrix(width, height * num_col);
	if (!grid_dup)
		return;

	// Calculeaza transpusa in matricea ajutatoare.
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			for (int k = 0; k < num_col; k++)
				grid_dup[j][i * num_col + k] = image->img_grid[i][j * num_col
																  + k];

	// Inverseaza liniile.
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height / 2; j++)
			for (int k = 0; k < num_col; k++)
				swap_char(&grid_dup[i][j * num_col + k],
						  &grid_dup[i][(image->height - j - 1) * num_col + k]);

	// Elibereaza memoria ocupata si copiaza noua imagine.
	image->img_grid = free_matrix(image->height, image->img_grid);
	image->img_grid = grid_dup;

	// Se modifica dimensiunile pentru imaginea rotita.
	image->width = width;
	swap(&image->width, &image->height);
	image->width *= num_col;
}

// Roteste o matrice in functie de un numar de grade divizibil cu 90.
// Parseaza si trimite parametrii la functiile ajutatoare.
// Toate rotatiile se reduc la rotirea unei imagini cu 90°.
void command_rotate(image_t *image, image_t *select)
{
	int rotation_angle;
	scanf("%d", &rotation_angle);

	if (!select->img_grid) {
		printf("No image loaded\n");
		return;
	}

	if (rotation_angle % 90) {
		printf("Unsupported rotation angle\n");
		return;
	}

	// Rotirea este o operatie periodica.
	// Calculeaza numarul de rotatii cu 90° necesare.
	int num_of_rotations = (360 + rotation_angle) % 360 / 90;

	if (image->width == select->width && image->height == select->height) {
		for (int i = 0; i < num_of_rotations; i++)
			rotate90_all(image);

		select_all(image, select);

	} else if (select->width / select->num_of_colors == select->height) {
		for (int i = 0; i < num_of_rotations; i++)
			rotate90_square(select->height, select->img_grid,
							select->num_of_colors);
	} else {
		printf("The selection must be square\n");
		return;
	}
	printf("Rotated %d\n", rotation_angle);
}

// Reduce imaginea la cea cuprinsa in cadrul selectiei curente.
void command_crop(image_t *image, image_t *select)
{
	if (!select->img_grid) {
		printf("No image loaded\n");
		return;
	}

	for (int i = 0; i < select->height; i++) {
		// Copiaza linia i a selectiei intr-un vector ajutator.
		unsigned char *buf = (unsigned char *)malloc(select->width
													 * sizeof(unsigned char *));
		mem_cpy(buf, select->img_grid[i],
				select->width * sizeof(unsigned char *));

		// Elibereaza linia i a imaginii.
		free(image->img_grid[i]);

		// Copiaza linia salvata in vectorul ajutator pe pozitia i din imagine.
		image->img_grid[i] = buf;
	}

	// Elibereaza liniile in plus din imagine.
	for (int i = select->height; i < image->height; i++)
		free(image->img_grid[i]);

	// Actualizeaza dimensiunile imaginii.
	image->height = select->height;
	image->width = select->width;

	// Selecteaza noua imagine.
	free(select->img_grid);
	select->img_grid = NULL;
	select_all(image, select);

	printf("Image cropped\n");
}

// Aplica un filtru Grayscale selectiei.
// Disponibil doar pentru imagini RGB.
//      * newRed = newGreen = newBlue = (Red + Greed + Blue) / 3
// Rezultatul este rotunjit la cea mai apropiata valoare intreaga, de aceea s-a
// folosit un indice de corectie.
void command_grayscale(image_t *select)
{
	if (!select->img_grid) {
		printf("No image loaded\n");
		return;
	}

	if (select->is_rgb) {
		for (int i = 0; i < select->height; i++)
			for (int j = 0; j < select->width; j += select->num_of_colors) {
				double red = select->img_grid[i][j];
				double green = select->img_grid[i][j + 1];
				double blue = select->img_grid[i][j + 2];

				double new_gray = (red + blue + green) / select->num_of_colors;
				// Rotunjeste la cel mai apropiat numar intreg.
				new_gray += EPS;

				select->img_grid[i][j] = new_gray;
				select->img_grid[i][j + 1] = new_gray;
				select->img_grid[i][j + 2] = new_gray;
			}
	} else {
		printf("Grayscale filter not available\n");
		return;
	}

	printf("Grayscale filter applied\n");
}

// Aplica un filtru Sepia selectiei.
// Disponibil doar pentru imagini RGB.
//      ∗ newRed = 0.393*R + 0.769*G + 0.189*B
//      ∗ newGreen = 0.349*R + 0.686*G + 0.168*B
//      ∗ newBlue = 0.272*R + 0.534*G + 0.131*B
// Rezultatul este rotunjit la cea mai apropiata valoare intreaga, de aceea s-a
// folosit un indice de corectie.
void command_sepia(image_t *select)
{
	if (!select->img_grid) {
		printf("No image loaded\n");
		return;
	}

	if (select->is_rgb) {
		for (int i = 0; i < select->height; i++)
			for (int j = 0; j < select->width; j += select->num_of_colors) {
				double red = select->img_grid[i][j];
				double green = select->img_grid[i][j + 1];
				double blue = select->img_grid[i][j + 2];

				double new_red = 0.393 * red + 0.769 * green + 0.189 * blue;
				double new_green = 0.349 * red + 0.686 * green + 0.168 * blue;
				double new_blue = 0.272 * red + 0.534 * green + 0.131 * blue;
				// Rotunjeste la cel mai apropiat numar intreg.
				new_red += EPS;
				new_green += EPS;
				new_blue += EPS;

				select->img_grid[i][j] = min_int(MAX_VALUE, (int)new_red);
				select->img_grid[i][j + 1] = min_int(MAX_VALUE, (int)new_green);
				select->img_grid[i][j + 2] = min_int(MAX_VALUE, (int)new_blue);
			}
	} else {
		printf("Sepia filter not available\n");
		return;
	}

	printf("Sepia filter applied\n");
}

// Salveaza imaginea data in fisier, sub format text sau binar.
void command_save(image_t *image)
{
	char opts[MAX_OPT];

	fgets(opts, MAX_OPT, stdin);
	opts[strlen(opts) - 1] = '\0';

	if (!image->img_grid) {
		printf("No image loaded\n");
		return;
	}

	// Verifica daca imaginea trebuie salvata sub format text sau binar.
	int save_in_binary = TRUE;
	if (strstr(opts, "ascii"))
		save_in_binary = FALSE;

	// Extrage numele fisierului.
	for (int i = 1; i < (int)strlen(opts); i++) {
		if (opts[i] == ' ') {
			opts[i] = '\0';
			break;
		}
	}
	char *filename = opts + 1;

	FILE *file = fopen(filename, "wt");

	// Printeaza in fisier tipul imaginii.
	int file_type = save_in_binary ? (image->type - 1) % 3 + 4 :
									 (image->type - 1) % 3 + 1;
	fprintf(file, "P%d\n", file_type);

	// Printeaza dimensiunile.
	fprintf(file, "%d %d\n", image->width / image->num_of_colors,
			image->height);

	// Daca e cazul, printeaza max_value.
	if (image->type != P1 && image->type != P4)
		fprintf(file, "%d\n", MAX_VALUE);

	if (save_in_binary) {
		fclose(file);
		file = fopen(filename, "ab");

		for (int i = 0; i < image->height; i++)
			fwrite(image->img_grid[i], sizeof(unsigned char), image->width,
				   file);
	} else {
		for (int i = 0; i < image->height; i++) {
			for (int j = 0; j < image->width; j++)
				fprintf(file, "%d ", image->img_grid[i][j]);
			fprintf(file, "\n");
		}
	}
	printf("Saved %s\n", filename);

	fclose(file);
}

// Elibereaza memoria si exiteaza programul.
int command_exit(image_t *image, image_t *select)
{
	if (!image->img_grid) {
		printf("No image loaded\n");
		return 0;
	}

	destroy_image(image);
	destroy_select(select);

	return 0;
}
