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

#include <stddef.h>

#include "ppm.h"

static void
image_fill(struct ppm_image *img, uint32_t color)
{
	uint32_t i;
	for (i = 0; i < img->width * img->height; ++i)
		img->pixels[i] = color;
}

static void
image_rect(struct ppm_image *img, int x, int y, int w, int h,
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
	size_t i;
	struct ppm_image *img;

	img = ppm_image_create(256, 256);

	image_fill(img, 0xff0000);
	image_rect(img, 10, 10, img->width - 20, img->height - 20, 0x1239e6);

	for (i = 0; i < 10; ++i)
		ppm_image_text_5x7(img, 20, 20 * (i + 1), 0xffff00 - 0x3300 * i, 3, "Hello world!");

	ppm_image_save(img, "hello_world.ppm");
	ppm_image_destroy(img);

	return 0;
}
