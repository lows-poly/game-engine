#ifndef ENGINE_COLOUR_H
#define ENGINE_COLOUR_H

#include <stdint.h>

typedef struct colour {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} colour;

#define VINTAGE_GOLD	((colour){ 42, 48, 41 })

#define BLACK		((colour){ 0, 0, 0 })
#define RED		((colour){ 255, 0, 0 })
#define GREEN		((colour){ 0, 255, 0 })
#define BLUE		((colour){ 0, 0, 255 })

#endif
