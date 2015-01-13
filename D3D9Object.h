// --- Author                  : Moreau Cyril - Spl3en
#pragma once

// ---------- Includes ------------
#include "Utils/Utils.h"
#include "dx/d3d9.h"
#include "dx/d3dx9.h"
#include "BbQueue/BbQueue.h"
#include "Win32Tools/Win32Tools.h"

// ---------- Defines -------------


// ------ Structure declaration -------




// --------- Allocators ---------

typedef enum {

	D3D9_OBJECT_RECTANGLE,
	D3D9_OBJECT_TEXT,
	D3D9_OBJECT_SPRITE

}	D3D9ObjectType;

typedef enum {

	D3D9_OBJECT_SPRITE_NOT_READY,
	D3D9_OBJECT_SPRITE_READY,
	D3D9_OBJECT_SPRITE_ERROR

}	D3D9ObjectSpriteStatus;

typedef struct
{
	int w, h;
	byte r, g, b;

} 	D3D9ObjectRect;

typedef struct
{
	ID3DXFont *font;
	byte r, g, b;
	int opacity;
	char *string;

} 	D3D9ObjectText;

typedef struct
{
	int opacity;
	char * filePath;
	ID3DXSprite * sprite;
	IDirect3DTexture9 * texture;
	D3D9ObjectSpriteStatus status;
	int w, h;

} 	D3D9ObjectSprite;

typedef struct
{
	int id;
	D3D9ObjectType type;
	int x, y;

	union {
		D3D9ObjectRect rect;
		D3D9ObjectText text;
		D3D9ObjectSprite sprite;
	};

	HANDLE mutex;

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
 * Description     : Remove all the allocated D3D9Object from the working factory lists
 * Return          : void
 */
void
D3D9ObjectFactory_delete_all (
	void
);

/*
 * Description     : Remove all the allocated D3D9Object from all the factory lists
 * Return          : void
 */
void
D3D9ObjectFactory_clean_memory (
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

/*
 * Description : Lock the mutex shared with all the d3d9objects
 * Return      : void
 */
void
D3D9ObjectFactory_lock (
	void
);

/*
 * Description : Release the mutex shared with all the d3d9objects
 * Return      : void
 */
void
D3D9ObjectFactory_release (
	void
);

/*
 * Description                 : Initialize D3D9ObjectSprite DirectX objects.
 *                               /!\ This function must be called only from the DirectX thread.
 * IDirect3DDevice9 * pDevice  : An allocated IDirect3DDevice9
 * Return                      : void
 */
void
D3D9ObjectSprite_init_directx (
	IDirect3DDevice9 * pDevice
);

/*
 * Description : Get the top level object that is hovered. If no object is hovered, return NULL
 * HWND hWindow : The window containing the directX context
 * Return : A pointer to the hovered object, or NULL
 */
D3D9Object *
D3D9ObjectFactory_get_hovered_object (
	HWND hWindow
);

/// ===== D3D9Object =====

/*
 * Description : Move an allocated D3D9Object on the screen
 * D3D9Object *this : An allocated D3D9Object
 # int x, int y : The new position on the screen
 * Return : void
 */
void
D3D9Object_move (
	D3D9Object *this,
	int x, int y
);

/*
 * Description                 : Initialize an allocated D3D9ObjectRect object.
 * int x, y                    : {x, y} position of the rectangle
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
 * Description : Set attributes of a D3D9ObjectRect
 * D3D9ObjectRect *this : An allocated D3D9ObjectRect
 * byte r, byte g, byte b : New color of the rectangle
 * int w, int h : Width and height of the rectangle
 * Return : void
 */
void
D3D9ObjectRect_set (
	D3D9ObjectRect *this,
	byte r, byte g, byte b,
	int w, int h
);


/*
 * Description                 : Initialize an allocated D3D9ObjectText object.
 * D3D9Object * this           : An allocated D3D9Object
 * IDirect3DDevice9 * pDevice  : An allocated IDirect3DDevice9
 * int x, y                    : {x, y} position of the text
 * byte r, byte g, byte b      : color of the text
 * float opacity : opacity of the text, value between 0.0 and 1.0
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
	float opacity,
	char *string,
	int fontSize,
	char *fontFamily
);

/*
 * Description : Set new attribute to D3D9ObjectText
 * D3D9ObjectText *this : An allocated D3D9ObjectText
 * char *string : New string of the text
 * byte r, byte g, byte b : New color of the text
 * float opacity : opacity of the text
 * Return : void
 */
void
D3D9ObjectText_set (
	D3D9ObjectText *this,
	char *string,
	byte r, byte g, byte b,
	float opacity
);

/*
 * Description                 : Initialize an allocated D3D9ObjectSprite object.
 * D3D9Object * this           : An allocated D3D9Object
 * IDirect3DDevice9 * pDevice  : An allocated IDirect3DDevice9
 * char * filePath             : Absolute or relative path of the image (.bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga)
 * int x, y                    : {x, y} position of the text
 # float opacity   : opacity of the image, value between 0.0 and 1.0
 * Return                      : bool True on success, false otherwise
 */
bool
D3D9ObjectSprite_init (
	D3D9Object * this,
	IDirect3DDevice9 * pDevice,
	char *filePath,
	int x, int y,
	float opacity
);

/*
 * Description : Set new attribute to D3D9ObjectSprite
 * D3D9ObjectText *this : An allocated D3D9ObjectSprite
 * float opacity : opacity of the sprite
 * Return : void
 */
void
D3D9ObjectSprite_set (
	D3D9ObjectSprite *this,
	float opacity
);

/// ===== Drawing utilities =====


/*
 * Description                 : Draw a rectangle at a given position / color on the screen
 * D3D9ObjectRect *rect        : An allocated D3D9ObjectRect
 * int x, y                    : {x, y} position of the rectangle
 * IDirect3DDevice9 * pDevice  : An allocated d3d9 device
 */
void
D3D9ObjectRect_draw (
	D3D9ObjectRect *rect,
	int x, int y,
	IDirect3DDevice9 * pDevice
);

/*
 * Description                 : Draw text at a given position / color on the screen
 * D3D9ObjectText *text        : An allocated D3D9ObjectText
 * int x, y                    : {x, y} position of the text
 * IDirect3DDevice9 * pDevice  : An allocated d3d9 device
 */
void
D3D9ObjectText_draw (
	D3D9ObjectText *text,
	int x, int y,
	IDirect3DDevice9 * pDevice
);

/*
 * Description                    : Draw a sprite at a given position on the screen
 * D3D9ObjectSprite *spriteObject : An allocated D3D9ObjectSprite.
 * int x, y                       : {x, y} position of the text
 * Return                         : void
 */
void
D3D9ObjectSprite_draw (
	D3D9ObjectSprite *spriteObject,
	int x, int y
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

