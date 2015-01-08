// --- Author                  : Moreau Cyril - Spl3en
#pragma once

// ---------- Includes ------------
#include "Utils/Utils.h"
#include "dx/d3d9.h"
#include "dx/d3dx9.h"

// ---------- Defines -------------


// ------ Structure declaration -------




// --------- Allocators ---------

typedef enum {

	D3D9_OBJECT_RECTANGLE,
	D3D9_OBJECT_TEXT,
	D3D9_OBJECT_SPRITE

}	D3D9ObjectType;

typedef struct {

	int x, y;
	int w, h;
	D3DCOLOR color;

} 	D3D9ObjectRect;

typedef struct {

	int x, y;
	ID3DXFont *font;
	D3DCOLOR color;
	char *string;

} 	D3D9ObjectText;

typedef struct {

	int x, y;
	ID3DXSprite * sprite;
	IDirect3DTexture9 * texture;

} 	D3D9ObjectSprite;

typedef struct
{
	unsigned int id;
	D3D9ObjectType type;

	union {
		D3D9ObjectRect rect;
		D3D9ObjectText text;
		D3D9ObjectSprite sprite;
	};

}	D3D9Object;

// ----------- Functions ------------


/*
 * Description                 : Initialize an allocated D3D9ObjectRect object.
 * int x, y                    : {x, y} position of the text
 * int w, h                    : width and height
 * D3DCOLOR color              : color of the rectangle
 * Return                      : bool True on success, false otherwise
 */
bool
D3D9ObjectRect_init (
	D3D9Object * this,
	int x, int y,
	int w, int h,
	D3DCOLOR color
);

/*
 * Description                 : Initialize an allocated D3D9ObjectText object.
 * D3D9Object * this           : An allocated D3D9Object
 * IDirect3DDevice9 * pDevice  : An allocated IDirect3DDevice9
 * int x, y                    : {x, y} position of the text
 * D3DCOLOR color              : color of the text
 * char * string               : String of the text
 * int fontSize                : the size of the font
 * char * fontFamily           : The name of the family font. If NULL, "Arial" is used.
 * Return                      : bool True on success, false otherwise
 */
bool
D3D9ObjectText_init (
	D3D9Object * this,
	IDirect3DDevice9 * pDevice,
	int x, int y,
	D3DCOLOR color,
	char *string,
	int fontSize,
	char *fontFamily
);

/*
 * Description                 : Initialize an allocated D3D9ObjectSprite object.
 * D3D9Object * this           : An allocated D3D9Object
 * IDirect3DDevice9 * pDevice  : An allocated IDirect3DDevice9
 * char * filePath             : Absolute or relative path of the image
 * int x, y                    : {x, y} position of the text
 * int w, h                    : width and height
 * D3DCOLOR color              : color of the rectangle
 * Return                      : bool True on success, false otherwise
 */
bool
D3D9ObjectSprite_init (
	D3D9Object * this,
	IDirect3DDevice9 * pDevice,
	char *filePath,
	int x, int y,
	int w, int h,
	D3DCOLOR color
);

/// ===== Drawing utilities =====

/*
 * Description                 : Draw text at a given position / color on the screen
 * IDirect3DDevice9 * pDevice  : An allocated d3d9 device
 * ID3DXFont * font            : An allocated DXFONT
 * int x, int y                : Position of the text on the screen
 * D3DCOLOR color              : Color of the text
 * char *text                  : Text to write on the screen
 */
void
D3D9Object_drawText (
	IDirect3DDevice9 * pDevice,
	ID3DXFont * font,
	int x, int y,
	D3DCOLOR color, char *text
);

/*
 * Description                 : Draw a rectangle at a given position / color on the screen
 * IDirect3DDevice9 * pDevice  : An allocated d3d9 device
 * int x, int y                : Position of the text on the screen
 * int w, int h                : Width and Height of the rectangle
 * D3DCOLOR color              : Color of the rectangle
 */
void
D3D9Object_drawRect (
	IDirect3DDevice9 * pDevice,
	int x, int y,
	int w, int h,
	D3DCOLOR color
);


/*
 * Description                 : Draw a sprite at a given position on the screen
 * ID3DXSprite * sprite        : Sprite of the image
 * IDirect3DTexture9 * texture : Texture of the image
 * int x, y                    : Position {x, y} where to display the sprite on the screen
 */
void
D3D9Object_drawSprite (
	ID3DXSprite * sprite,
	IDirect3DTexture9 * texture,
	int x, int y
);


// --------- Destructors ----------



