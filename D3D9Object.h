// --- Author                  : Moreau Cyril - Spl3en
#pragma once

// ---------- Includes ------------
#include "Utils/Utils.h"
#include "dx/d3d9.h"
#include "dx/d3dx9.h"
#include "BbQueue/BbQueue.h"

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
	int id;
	D3D9ObjectType type;
	HANDLE mutex;

	union {
		D3D9ObjectRect rect;
		D3D9ObjectText text;
		D3D9ObjectSprite sprite;
	};

}	D3D9Object;


// ----------- Functions ------------

/// ===== D3D9ObjectFactory =====

/*
 * Description                 : Allocate a new D3D9Object
 * D3D9ObjectType type         : Type of the object to create
 * Return                      : D3D9Object * an allocated D3D9Object
 */
D3D9Object *
D3D9ObjectFactory_createD3D9Object (
	D3D9ObjectType type
);

/*
 * Description     : Return an allocated D3D9Object from the factory list
 * unsigned int id : A D3D9ObjectFactory ID already allocated
 * Return          : void
 */
D3D9Object *
D3D9ObjectFactory_get (
	unsigned int id
);

/*
 * Description     : Add an allocated D3D9Object to the factory draw list.
					 Don't add it again if it already exists in the list.
 * unsigned int id : A D3D9ObjectFactory ID already allocated
 * Return          : The D3D9Object hidden, or NULL if not found
 */
D3D9Object *
D3D9ObjectFactory_show (
	unsigned int id
);

/*
 * Description     : Add all allocated D3D9Object to the factory draw list.
					 Don't add it again if it already exists in the list.
 * Return          : void
 */
void
D3D9ObjectFactory_show_all (
	void
);

/*
 * Description     : Remove an allocated D3D9Object from the factory draw list, so it isn't displayed anymore
 * unsigned int id : A D3D9ObjectFactory ID already allocated
 * Return          : The D3D9Object hidden, or NULL if not found
 */
D3D9Object *
D3D9ObjectFactory_hide (
	unsigned int id
);


/*
 * Description     : Remove all allocated D3D9Object from the factory draw list, so they aren't displayed anymore
 * Return          : void
 */
void
D3D9ObjectFactory_hide_all (
	void
);

/*
 * Description : Return a pointer to the objects list
 * Return      : BbQueue * A list of D3D9Objects pointer
 */
BbQueue *
D3D9ObjectFactory_get_objects (
	void
);

/*
 * Description     : Remove all the allocated D3D9Object from all the factory lists
 * Return          : void
 */
void
D3D9ObjectFactory_delete_all (
	void
);

/*
 * Description     : Remove an allocated D3D9Object from all the factory lists
 * unsigned int id : A D3D9ObjectFactory ID already allocated
 * Return          : void
 */
void
D3D9ObjectFactory_delete (
	unsigned int id
);

/// ===== D3D9Object =====

/*
 * Description                 : Initialize an allocated D3D9ObjectRect object.
 * int x, y                    : {x, y} position of the text
 * int w, h                    : width and height
 * byte r, byte g, byte b      : color of the rectangle
 * Return                      : void
 */
bool
D3D9ObjectRect_init (
	D3D9Object * this,
	int x, int y,
	int w, int h,
	byte r, byte g, byte b
);

/*
 * Description                 : Initialize an allocated D3D9ObjectText object.
 * D3D9Object * this           : An allocated D3D9Object
 * IDirect3DDevice9 * pDevice  : An allocated IDirect3DDevice9
 * int x, y                    : {x, y} position of the text
 * byte r, byte g, byte b      : color of the text
 * char * string               : String of the text
 * int fontSize                : the size of the font
 * char * fontFamily           : The name of the family font. If NULL, "Arial" is used.
 * Return                      : void
 */
bool
D3D9ObjectText_init (
	D3D9Object * this,
	IDirect3DDevice9 * pDevice,
	int x, int y,
	byte r, byte g, byte b,
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
 * Return                      : bool True on success, false otherwise
 */
bool
D3D9ObjectSprite_init (
	D3D9Object * this,
	IDirect3DDevice9 * pDevice,
	char *filePath,
	int x, int y,
	int w, int h
);

/// ===== Drawing utilities =====

/*
 * Description                 : Draw a rectangle at a given position / color on the screen
 * D3D9ObjectRect *rect        : An allocated D3D9ObjectRect
 * IDirect3DDevice9 * pDevice  : An allocated d3d9 device
 */
void
D3D9ObjectRect_draw (
	D3D9ObjectRect *rect,
	IDirect3DDevice9 * pDevice
);

/*
 * Description                 : Draw text at a given position / color on the screen
 * D3D9ObjectText *text        : An allocated D3D9ObjectText
 * IDirect3DDevice9 * pDevice  : An allocated d3d9 device
 */
void
D3D9ObjectText_draw (
	D3D9ObjectText *text,
	IDirect3DDevice9 * pDevice
);

/*
 * Description                    : Draw a sprite at a given position on the screen
 * D3D9ObjectSprite *spriteObject : An allocated D3D9ObjectSprite.
 * Return                         : void
 */
void
D3D9ObjectSprite_draw (
	D3D9ObjectSprite *spriteObject
);


// --------- Destructors ----------


/*
 * Description : Free an allocated D3D9Object
 * D3D9Object *this : An allocated D3D9Object
 * Return : void
 */
void
D3D9Object_free (
	D3D9Object *this
);

