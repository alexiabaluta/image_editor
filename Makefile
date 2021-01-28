# Copyright 2020 Alexia-Elena Baluta 311CA

CFLAGS=-Wall -Wextra -std=c99

build:
	gcc $(CFLAGS) -o image_editor utils.c matrix_utils.c image.c commands.c image_editor.c

pack:
	zip -FSr 311CA_BalutaAlexia_Tema3.zip README Makefile *.c *.h

clean:
	rm -f star_dust utils

.PHONY: pack clean