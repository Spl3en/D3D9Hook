#include "D3D9Object.h"

// ---------- Debugging -------------
#define __DEBUG_OBJECT__ "D3D9Object"
#include "dbg/dbg.h"

// Factory declaration and static initialization
struct D3D9ObjectFactory {
	BbQueue drawObjects;
	BbQueue allObjects;
} d3d9ObjectFactory = {
	.drawObjects = bb_queue_local_decl (),
	.allObjects = bb_queue_local_decl ()
};

// Private headers
/*
 * Description      : Add a D3D9Object into the collection of the factory
 * D3D9Object *this : An allocated D3D9Object
 * Return           : void
 */
static void D3D9ObjectFactory_add (D3D9Object *this);


/// ===== D3D9ObjectFactory =====


/*
 * Description                 : Allocate a new D3D9Object
 * D3D9ObjectType type         : Type of the object to create
 * Return                      : D3D9Object * an allocated D3D9Object
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
	if (!(this->mutex = CreateMutex (NULL, FALSE, NULL))) {
		dbg ("Cannot create mutex for ID=%d. Reason = %d", this->id, GetLastError());
	}

	D3D9ObjectFactory_add (this);

	return this;
}

/*
 * Description      : Add a D3D9Object into the collection of the factory
 * D3D9Object *this : An allocated D3D9Object
 * Return           : void
 */
static void
D3D9ObjectFactory_add (
	D3D9Object *this
) {
	if (!bb_queue_exists (&d3d9ObjectFactory.drawObjects, this))
		bb_queue_add (&d3d9ObjectFactory.drawObjects, this);

	if (!bb_queue_exists (&d3d9ObjectFactory.allObjects, this))
		bb_queue_add (&d3d9ObjectFactory.allObjects, this);
}

/*
 * Description     : Return an allocated D3D9Object from the global factory list
 * unsigned int id : A D3D9ObjectFactory ID already allocated
 * Return          : void
 */
D3D9Object *
D3D9ObjectFactory_get (
	unsigned int id
) {
	foreach_bbqueue_item (&d3d9ObjectFactory.allObjects, D3D9Object *object)
	{
		if (object->id == id) {
			return object;
		}
	}

	dbg ("Object ID=%d not found in global list.", id);

	return NULL;
}

/*
 * Description     : Return an allocated D3D9Object from the draw factory list
 * unsigned int id : A D3D9ObjectFactory ID already allocated
 * Return          : void
 */
D3D9Object *
D3D9ObjectFactory_get_draw (
	unsigned int id
) {
	foreach_bbqueue_item (&d3d9ObjectFactory.drawObjects, D3D9Object *object)
	{
		if (object->id == id) {
			return object;
		}
	}

	dbg ("Object ID=%d not found in draw list.", id);

	return NULL;
}

/*
 * Description     : Add an allocated D3D9Object to the factory draw list.
					 Don't add it again if it already exists in the list.
 * unsigned int id : A D3D9ObjectFactory ID already allocated
 * Return          : The D3D9Object hidden, or NULL if not found
 */
D3D9Object *
D3D9ObjectFactory_show (
	unsigned int id
) {
	D3D9Object *object;

	if (!(object = D3D9ObjectFactory_get (id))) {
		dbg ("Cannot show object ID=%d.", id);
		return NULL;
	}

	// Add to the draw list
	if (bb_queue_exists (&d3d9ObjectFactory.drawObjects, object)) {
		dbg ("Object ID=%d already exists in the draw list. Abort show.", id);
		return NULL;
	}

	bb_queue_add (&d3d9ObjectFactory.drawObjects, object);

	return object;
}

/*
 * Description     : Add all allocated D3D9Object to the factory draw list.
					 Don't add it again if it already exists in the list.
 * Return          : void
 */
void
D3D9ObjectFactory_show_all (
	void
) {
	foreach_bbqueue_item (&d3d9ObjectFactory.allObjects, D3D9Object *object)
	{
		if (!bb_queue_exists (&d3d9ObjectFactory.drawObjects, object))
			bb_queue_add (&d3d9ObjectFactory.drawObjects, object);
	}
}

/*
 * Description     : Remove an allocated D3D9Object from the factory draw list, so it isn't displayed anymore
 * unsigned int id : A D3D9ObjectFactory ID already allocated
 * Return          : The D3D9Object hidden, or NULL if not found
 */
D3D9Object *
D3D9ObjectFactory_hide (
	unsigned int id
) {
	D3D9Object *object;

	if (!(object = D3D9ObjectFactory_get_draw (id))) {
		dbg ("Cannot hide object ID=%d.", id);
		return NULL;
	}

	// Remove from the draw list
	if (!(bb_queue_exists (&d3d9ObjectFactory.drawObjects, object))) {
		dbg ("Object ID=%d doesn't exist in the draw list. Abort hide.", id);
		return NULL;
	}

	bb_queue_remv (&d3d9ObjectFactory.drawObjects, object);

	return object;
}


/*
 * Description     : Remove all allocated D3D9Object from the factory draw list, so they aren't displayed anymore
 * Return          : void
 */
void
D3D9ObjectFactory_hide_all (
	void
) {
	// Remove all D3D9Objects from the drawObjects list
	while (bb_queue_get_length (&d3d9ObjectFactory.drawObjects)) {
		bb_queue_pop (&d3d9ObjectFactory.drawObjects);
	}
}

/*
 * Description     : Remove all the allocated D3D9Object from all the factory lists
 * Return          : void
 */
void
D3D9ObjectFactory_delete_all (
	void
) {
	// Remove all D3D9Objects from the drawObjects list
	while (bb_queue_get_length (&d3d9ObjectFactory.drawObjects)) {
		bb_queue_pop (&d3d9ObjectFactory.drawObjects);
		// Those objects are all shared with the global list
		// Free them in the global list
	}

	// Remove all D3D9Objects from the global list
	while (bb_queue_get_length (&d3d9ObjectFactory.allObjects)) {
		D3D9Object *object = bb_queue_pop (&d3d9ObjectFactory.allObjects);
		D3D9Object_free (object);
	}
}

/*
 * Description     : Remove an allocated D3D9Object from all the factory lists
 * unsigned int id : A D3D9ObjectFactory ID already allocated
 * Return          : void
 */
void
D3D9ObjectFactory_delete (
	unsigned int id
) {
	D3D9Object * object;

	if (!(object = D3D9ObjectFactory_get (id))) {
		dbg ("Cannot find the object to delete.");
		return;
	}

	if (bb_queue_exists (&d3d9ObjectFactory.drawObjects, object)) {
		bb_queue_remv (&d3d9ObjectFactory.drawObjects, object);
	}

	bb_queue_remv (&d3d9ObjectFactory.allObjects, object);
	D3D9Object_free (object);
}


/*
 * Description : Return a pointer to the objects list
 * Return      : BbQueue * A list of D3D9Objects pointer
 */
BbQueue *
D3D9ObjectFactory_get_objects (
	void
) {
	return &d3d9ObjectFactory.drawObjects;
}


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
) {
	D3D9ObjectRect * rect = &this->rect;

	// Fill the structure
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	rect->color = D3DCOLOR_RGBA (r, g, b, 255);

	dbg ("Rectangle <x=%d | y=%d | w=%d | h=%d> has been created.", x, y, w, h);

	return true;
}

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
	text->color = D3DCOLOR_RGBA (r, g, b, 255);
	text->string = strdup (string);

	dbg ("Text <%s> at <x=%d | y=%d> has been created.", string, x, y);

	return true;
}

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
 * Description                 : Draw a rectangle at a given position / color on the screen
 * D3D9ObjectRect *rect        : An allocated D3D9ObjectRect
 * IDirect3DDevice9 * pDevice  : An allocated d3d9 device
 */
void
D3D9ObjectRect_draw (
	D3D9ObjectRect *rect,
	IDirect3DDevice9 * pDevice
) {
	D3DRECT rectPosition = {rect->x, rect->y, rect->x + rect->w, rect->y + rect->h};
	pDevice->lpVtbl->Clear (pDevice, 1, &rectPosition, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, rect->color, 0,  0);
}

/*
 * Description                 : Draw text at a given position / color on the screen
 * D3D9ObjectText *text        : An allocated D3D9ObjectText
 * IDirect3DDevice9 * pDevice  : An allocated d3d9 device
 */
void
D3D9ObjectText_draw (
	D3D9ObjectText *text,
	IDirect3DDevice9 * pDevice
) {
    RECT rect;
    ID3DXFont *font = text->font;

    SetRect (&rect, text->x, text->y, text->x, text->y);
    font->lpVtbl->DrawText (font, NULL, text->string, -1, &rect, DT_NOCLIP | DT_LEFT, text->color);
}

/*
 * Description                    : Draw a sprite at a given position on the screen
 * D3D9ObjectSprite *spriteObject : An allocated D3D9ObjectSprite.
 * Return                         : void
 */
void
D3D9ObjectSprite_draw (
	D3D9ObjectSprite *spriteObject
) {
	ID3DXSprite * sprite = spriteObject->sprite;
	IDirect3DTexture9 * texture = spriteObject->texture;

	D3DXVECTOR3 position3D = {spriteObject->x, spriteObject->y, 0.0};
	D3DCOLOR color = D3DCOLOR_ARGB (255, 255, 255, 255);

	sprite->lpVtbl->Begin (sprite, D3DXSPRITE_ALPHABLEND);
	sprite->lpVtbl->Draw (sprite, texture, NULL, NULL, &position3D, color);
	sprite->lpVtbl->End (sprite);
}

/*
 * Description : Free an allocated D3D9Object
 * D3D9Object *this : An allocated D3D9Object
 * Return : void
 */
void
D3D9Object_free (
	D3D9Object *this
) {
	switch (this->type)
	{
		case D3D9_OBJECT_RECTANGLE: {
            // Nothing to do
		} break;

		case D3D9_OBJECT_TEXT: {
			ID3DXFont *font = this->text.font;
			font->lpVtbl->Release (font);
		} break;

		case D3D9_OBJECT_SPRITE: {
			ID3DXSprite * sprite = this->sprite.sprite;
			IDirect3DTexture9 * texture = this->sprite.texture;
			sprite->lpVtbl->Release (sprite);
			texture->lpVtbl->Release (texture);
		} break;

		default : dbg ("Cannot free completely an unknown type."); break;
	}

	free (this);
}
