/*
	Copyright (C) 2023 <alpheratz99@protonmail.com>

	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License version 2 as published by the
	Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include <stdint.h>
#include "ppm.h"

const char *filename = "main.c";
const char *src = \
	"#include <stdio.h>\n"
	"\n"
	"int main(argc, argv)\n"
	"    int argc;\n"
	"    char *argv[];\n"
	"{\n"
	"    for (;;)\n"
	"        printf(\"Thank you Dennis Ritchie!\\n\");\n"
	"}";

static void
ppm_image_fill(struct ppm_image *img, uint32_t color)
{
	int i;
	for (i = 0; i < img->width * img->height; ++i)
		img->pixels[i] = color;
}

static void
ppm_image_rect(struct ppm_image *img, int x, int y, int w, int h,
		uint32_t color)
{
	int i;

	for (i = 0; i < w; ++i)
		img->pixels[y*img->width+x+i] =
			img->pixels[(y+h-1)*img->width+x+i] =
				color;
	for (i = 0; i < h; ++i)
		img->pixels[(y+i)*img->width+x] =
			img->pixels[(y+i)*img->width+x+w-1] =
				color;
}

static void
ppm_image_fill_rect(struct ppm_image *img, int x, int y, int w, int h,
		uint32_t color)
{
	int i, j;
	for (i = 0; i < h; ++i)
		for (j = 0; j < w; ++j)
			img->pixels[(y+i)*img->width+x+j] = color;
}

int
main(void)
{
	struct ppm_image *img;

	img = ppm_image_create(400, 150);

	ppm_image_fill(img, 0x2e2e2e);
	ppm_image_fill_rect(img, 10, 10, img->width-20, img->height-42, 0x432f29);
	ppm_image_rect(img, 10, 10, img->width-20, img->height-42, 0xff4000);
	ppm_image_fill_rect(img, img->width-200, img->height-32, 190, 22, 0xff4000);
	ppm_image_text_5x7(img, img->width-200+(22-7)/2, img->height-32+(22-7)/2, 0xffffff, 2, filename);
	ppm_image_text_5x7(img, 10+(22-7)/2, 10+(22-7)/2, 0xffffff, 2, src);
	ppm_image_save(img, "code.ppm");
	ppm_image_destroy(img);

	return 0;
}
