/*
	Copyright (C) 2022-2023 <alpheratz99@protonmail.com>

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License version 2 as published by
	the Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
	more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc., 59
	Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "fo.c"
#include "ppm.h"

extern struct ppm_image *
ppm_image_create(int width, int height)
{
	struct ppm_image *image;

	assert(width > 0);
	assert(height > 0);

	image = malloc(sizeof(struct ppm_image));

	if (NULL == image)
		return NULL;

	image->width = width;
	image->height = height;
	image->pixels = calloc(width * height, sizeof(uint32_t));

	if (NULL == image->pixels) {
		free(image);
		return NULL;
	}

	return image;
}

static struct ppm_image *
_ppm_image_load_v6(FILE *fp)
{
	struct ppm_image *image;
	int width, height;
	uint8_t pixel[3];
	uint32_t *image_pixel, *last_image_pixel;

	fscanf(fp, "%d %d 255\n", &width, &height);
	image = ppm_image_create(width, height);
	image_pixel = &image->pixels[0];
	last_image_pixel = image_pixel + (image->width * image->height - 1);

	while (fread(pixel, 1, 3, fp) == 3) {
		*image_pixel = (pixel[0] << 16) | (pixel[1] << 8) | pixel[2];
		if (image_pixel++ == last_image_pixel)
			break;
	}

	return image;
}

extern struct ppm_image *
ppm_image_load(const char *path)
{
	FILE *fp;
	int version;
	struct ppm_image *image;

	image = NULL;
	fp = fopen(path, "r");
	fscanf(fp, "P%d\n", &version);

	switch (version) {
		case 6: image = _ppm_image_load_v6(fp); break;
	}

	fclose(fp);

	return image;
}

extern void
ppm_image_crop(struct ppm_image *image,
               int x, int y, int width, int height)
{
	int dx, dy;
	uint32_t *crop_area;

	if (x < 0) { width  += x, x = 0; }
	if (y < 0) { height += y, y = 0; }
	if (x + width >= image->width) width = image->width - x;
	if (y + height >= image->height) height = image->height - y;

	if (width < 1 || height < 1)
		return;

	crop_area = malloc(sizeof(uint32_t) * width * height);

	for (dy = 0; dy < height; ++dy)
		for (dx = 0; dx < width; ++dx)
			crop_area[dy * width + dx] =
				image->pixels[(y + dy) * image->width + x + dx];

	free(image->pixels);

	image->pixels = crop_area;
	image->width = width;
	image->height = height;
}

extern void
ppm_image_blur(struct ppm_image *image,
               int x, int y, int width, int height,
               int strength)
{
	int dx, dy;
	int pass;
	int numpx, r, g, b, kdx, kdy;
	uint32_t *blur_area, *blur_area_previous, *tmp;

	if (x < 0) { width  += x; x = 0; }
	if (y < 0) { height += y; y = 0; }
	if (x + width >= image->width) width = image->width - x;
	if (y + height >= image->height) height = image->height - y;

	if (width < 1 || height < 1)
		return;

	blur_area          = malloc(sizeof(uint32_t) * width * height);
	blur_area_previous = malloc(sizeof(uint32_t) * width * height);

	for (dy = 0; dy < height; ++dy)
		for (dx = 0; dx < width; ++dx)
			blur_area_previous[dy * width + dx] =
				blur_area[dy * width + dx] =
					image->pixels[(y + dy) * image->width + x + dx];

	for (pass = 0; pass < strength; ++pass) {
		tmp = blur_area_previous;
		blur_area_previous = blur_area;
		blur_area = tmp;

		for (dy = 0; dy < height; ++dy) {
			for (dx = 0; dx < width; ++dx) {
				numpx = r = g = b = 0;
				for (kdy = -3; kdy < 4; ++kdy) {
					if ((dy + kdy) < 0 || (dy + kdy) >= height)
						continue;
					for (kdx = -3; kdx < 4; ++kdx) {
						if ((dx + kdx) < 0 || (dx + kdx) >= width)
							continue;

						r += (blur_area_previous[(dy + kdy) * width + dx + kdx] >> 16) & 0xff;
						g += (blur_area_previous[(dy + kdy) * width + dx + kdx] >>  8) & 0xff;
						b += (blur_area_previous[(dy + kdy) * width + dx + kdx] >>  0) & 0xff;
						numpx++;
					}
				}
				blur_area[dy * width + dx] =
					((r / numpx) << 16) |
					((g / numpx) <<  8) |
					((b / numpx) <<  0);
			}
		}
	}

	for (dy = 0; dy < height; ++dy)
		for (dx = 0; dx < width; ++dx)
			image->pixels[(y + dy) * image->width + x + dx] = blur_area[dy * width + dx];

	free(blur_area);
	free(blur_area_previous);
}

extern void
ppm_image_text_5x7(struct ppm_image *image,
                   int x, int y, uint32_t color,
                   int letter_spacing, const char *text)
{
	int cx, cy, gx, gy;
	unsigned char *glyph;
	const char *p;

	p = text;
	cx = x;
	cy = y;

	while (*p != '\0') {
		if (*p == '\n') {
			cx = x;
			cy += 10;
			++p;
			continue;
		}
		glyph = five_by_seven + *p * 7;
		for (gy = 0; gy < 7; ++gy)
			for (gx = 0; gx < 5; ++gx)
				if (glyph[gy] & (1 << (4 - gx)) && cy + gy < image->height
						&& cx + gx < image->width)
					image->pixels[(cy + gy) * image->width + cx + gx] = color;
		cx += 5 + letter_spacing;
		++p;
	}
}

extern void
ppm_image_save(const struct ppm_image *image, const char *path)
{
	FILE *fp;
	uint8_t pixel[3];
	uint32_t *image_pixel, *last_image_pixel;

	image_pixel = &image->pixels[0];
	last_image_pixel = image_pixel + (image->width * image->height - 1);

	fp = fopen(path, "wb");
	fprintf(fp, "P6\n%d %d 255\n", image->width, image->height);

	while (1) {
		pixel[0] = (*image_pixel >> 16) & 0xff;
		pixel[1] = (*image_pixel >>  8) & 0xff;
		pixel[2] = (*image_pixel >>  0) & 0xff;
		fwrite(pixel, 1, 3, fp);
		if (image_pixel++ == last_image_pixel)
			break;
	}

	fclose(fp);
}

extern void
ppm_image_destroy(struct ppm_image *image)
{
	free(image->pixels);
	free(image);
}
