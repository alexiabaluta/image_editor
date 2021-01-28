// Copyright 2021 Alexia-Elena Baluta 311CA

#ifndef IMAGE_H
#define IMAGE_H

#include "env.h"

image_t init_image(void);

void destroy_image(image_t *image);

void destroy_select(image_t *select);

void build_image(image_t *image, int height, int width, int type);

void build_select(image_t *image, image_t *select, int x1, int y1, int x2,
				  int y2);

#endif
