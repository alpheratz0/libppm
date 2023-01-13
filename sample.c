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

#include "ppm.h"

static void
image_fill(const struct ppm_image *img, uint32_t color)
{
	uint32_t i;
	for (i = 0; i < img->width * img->height; ++i)
		img->pixels[i] = color;
}

int
main(void)
{
	struct ppm_image *img;

	img = ppm_image_create(256, 256);
	image_fill(img, 0xff0000);
	ppm_image_text_5x7(img, 15, 15, 0xffff00, 3, "Hello world!");
	ppm_image_save(img, "hello_world.ppm");
	ppm_image_destroy(img);

	return 0;
}
