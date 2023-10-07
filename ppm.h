/*
	Copyright (C) 2022 <alpheratz99@protonmail.com>

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

#ifndef __LIBPPM_PPM_H__
#define __LIBPPM_PPM_H__

#include <stdint.h>

typedef int (*ppm_image_text_highlighter)(const char *text, uint32_t *color);

#define HIGHLIGHT(label,c) do { \
	if (NULL == label) { \
		*color = c; \
		return 1; \
	} else if (strncmp(text,label,strlen(label))==0) { \
		*color = c; \
		return strlen(label); \
	} \
} while (0)

struct ppm_image {
	int width;
	int height;
	uint32_t *pixels;
};

extern struct ppm_image *
ppm_image_create(int width, int height);

extern struct ppm_image *
ppm_image_load(const char *path);

extern void
ppm_image_crop(struct ppm_image *image,
               int x, int y, int width, int height);

extern void
ppm_image_blur(struct ppm_image *image,
               int x, int y, int width, int height,
               int strength);

extern void
ppm_image_text_5x7(struct ppm_image *image,
                   int x, int y, uint32_t color,
                   int letter_spacing, const char *text);

extern void
ppm_image_text_highlight_5x7(struct ppm_image *image,
                   int x, int y, ppm_image_text_highlighter highlighter,
                   int letter_spacing, const char *text);

extern void
ppm_image_save(const struct ppm_image *image, const char *path);

extern void
ppm_image_destroy(struct ppm_image *image);

#endif
