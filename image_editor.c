// Copyright 2021 Alexia-Elena Baluta 311CA

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "matrix_utils.h"
#include "env.h"
#include "image.h"
#include "commands.h"

#define MAX_OPT 256

int main(void)
{
	image_t image = init_image();
	image_t select = init_image();

	while (TRUE) {
		char command[MAX_OPT];
		scanf("%s", command);

		if (!strcmp(command, "LOAD")) {
			command_load(&image, &select);

		} else if (!strcmp(command, "SELECT")) {
			command_select(&image, &select);

		} else if (!strcmp(command, "ROTATE")) {
			command_rotate(&image, &select);

		} else if (!strcmp(command, "CROP")) {
			command_crop(&image, &select);

		} else if (!strcmp(command, "GRAYSCALE")) {
			command_grayscale(&select);

		} else if (!strcmp(command, "SEPIA")) {
			command_sepia(&select);

		} else if (!strcmp(command, "SAVE")) {
			command_save(&image);

		} else if (!strcmp(command, "EXIT")) {
			return command_exit(&image, &select);

		} else {
			skip_to_line();
			printf("Invalid command\n");
		}
	}

	return 0;
}
