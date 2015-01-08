#include "D3D9Object.h"

// ---------- Debugging -------------
#define __DEBUG_OBJECT__ "D3D9Object"
#include "dbg/dbg.h"

/*
 * Description : Allocate a new D3D9Object
 * D3D9ObjectType type : Type of the object to create
 * Return : D3D9Object * an allocated D3D9Object
 */
D3D9Object *
D3D9ObjectFactory_createD3D9Object (
	D3D9ObjectType type
) {
	D3D9Object *this = NULL;
	static typeof (this->id) id = 0;

	if ((this = calloc (1, sizeof(D3D9Object))) == NULL)
		return NULL;

	this->type = type;
	this->id   = id++;

	return this;
}

/*
 * Description : Initialize an allocated D3D9ObjectRect object.
 * int x, y : {x, y} position of the text
 * int w, h : width and height
 * D3DCOLOR color : color of the rectangle
 * Return : void
 */
bool
D3D9ObjectRect_init (
	D3D9Object * this,
	int x, int y,
	int w, int h,
	D3DCOLOR color
) {
	D3D9ObjectRect * rect = &this->rect;

	// Fill the structure
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	rect->color = color;

	return true;
}

/*
 * Description : Initialize an allocated D3D9ObjectText object.
 * D3D9Object * this : An allocated D3D9Object
 * IDirect3DDevice9 * pDevice : An allocated IDirect3DDevice9
 * int x, y : {x, y} position of the text
 * D3DCOLOR color : color of the text
 * char * string : String of the text
 * int fontSize : the size of the font
 * char * fontFamily : The name of the family font. If NULL, "Arial" is used.
 * Return : void
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
) {
	D3D9ObjectText * text = &this->text;

	// Default parameters
	if (fontFamily == NULL) {
		fontFamily = "Arial";
	}

	// Allocate the font
	if ((D3DXCreateFont (
		pDevice,
		fontSize,
		0,
		FW_BOLD,
		1,
		0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		fontFamily,
		&text->font
	)) != S_OK) {
		return false;
	}

	// Fill the structure
	text->x = x;
	text->y = y;
	text->color = color;
	text->string = string;

	return true;
}

/*
 * Description : Initialize an allocated D3D9ObjectSprite object.
 * D3D9Object * this : An allocated D3D9Object
 * IDirect3DDevice9 * pDevice : An allocated IDirect3DDevice9
 * char * filePath : Absolute or relative path of the image
 * int x, y : {x, y} position of the text
 * int w, h : width and height
 * D3DCOLOR color : color of the rectangle
 * Return : bool True on success, false otherwise
 */
bool
D3D9ObjectSprite_init (
	D3D9Object * this,
	IDirect3DDevice9 * pDevice,
	char *filePath,
	int x, int y,
	int w, int h,
	D3DCOLOR color
) {
	D3D9ObjectSprite * sprite = &this->sprite;

	// Create the texture
	if ((D3DXCreateTextureFromFile (pDevice, filePath, &sprite->texture)) != D3D_OK) {
		dbg ("Cannot creature the texture <%s>.", filePath);
		return false;
	}

	// Create the sprite
	if ((D3DXCreateSprite (pDevice, &sprite->sprite)) != D3D_OK) {
		dbg ("Cannot create the sprite.");
		return false;
	}

	// Fill the structure
	sprite->x = x;
	sprite->y = y;

	dbg ("Sprite <%s> has been created.", filePath);

	return true;
}



/// ===== Drawing utilities =====

/*
 * Description : Draw text at a given position / color on the screen
 * IDirect3DDevice9 * pDevice : An allocated d3d9 device
 * ID3DXFont * font : An allocated DXFONT
 * int x, int y : Position of the text on the screen
 * D3DCOLOR color : Color of the text
 * char *text : Text to write on the screen
 */
void
D3D9Hook_drawText (
	IDirect3DDevice9 * pDevice,
	ID3DXFont * font,
	int x, int y,
	D3DCOLOR color, char *text
) {
    RECT rect;
    SetRect (&rect, x, y, x, y);
    font->lpVtbl->DrawText (font, NULL, text, -1, &rect, DT_NOCLIP | DT_LEFT, color);
}

/*
 * Description : Draw a rectangle at a given position / color on the screen
 * IDirect3DDevice9 * pDevice : An allocated d3d9 device
 * int x, int y : Position of the text on the screen
 * int w, int h : Width and Height of the rectangle
 * D3DCOLOR color : Color of the rectangle
 */
void
D3D9Hook_drawRect (
	IDirect3DDevice9 * pDevice,
	int x, int y,
	int w, int h,
	D3DCOLOR color
) {
	D3DRECT rect = {x, y, x + w, y + h};
	pDevice->lpVtbl->Clear (pDevice, 1, &rect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 0,  0);
}

/*
 * Description : Draw a sprite at a given position on the screen
 * ID3DXSprite * sprite : Sprite of the image
 * IDirect3DTexture9 * texture : Texture of the image
 * int x, y : Position {x, y} where to display the sprite on the screen
 */
void
D3D9Hook_drawSprite (
	ID3DXSprite * sprite,
	IDirect3DTexture9 * texture,
	int x, int y
) {
	D3DXVECTOR3 position3D = {x, y, 0.0};
	D3DCOLOR color = D3DCOLOR_ARGB (255, 255, 255, 255);

	sprite->lpVtbl->Begin (sprite, D3DXSPRITE_ALPHABLEND);
	sprite->lpVtbl->Draw (sprite, texture, NULL, NULL, &position3D, color);
	sprite->lpVtbl->End (sprite);
}

