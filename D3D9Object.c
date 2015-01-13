#include "D3D9Object.h"

// ---------- Debugging -------------
#define __DEBUG_OBJECT__ "D3D9Object"
#include "dbg/dbg.h"

// Factory declaration and static initialization
struct D3D9ObjectFactory {
	BbQueue drawObjects;
	BbQueue allObjects;
	BbQueue spriteToInstanciate;
	HANDLE mutex;
	int id;
	bool initialized;
} d3d9ObjectFactory = {
	.drawObjects         = bb_queue_local_decl (),
	.allObjects          = bb_queue_local_decl (),
	.spriteToInstanciate = bb_queue_local_decl (),
	.mutex               = NULL,
	.id                  = 0,
	.initialized         = false
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

	// Check if the factory has been initialized
	if (!d3d9ObjectFactory.initialized) {
		d3d9ObjectFactory.mutex = CreateMutex (NULL, false, NULL);
		d3d9ObjectFactory.initialized = true;
	}

	// Allocate a new instance of D3D9Object
	if ((this = calloc (1, sizeof(D3D9Object))) == NULL)
		return NULL;

	this->type  = type;
	this->id    = d3d9ObjectFactory.id++;
	this->mutex = d3d9ObjectFactory.mutex;

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

	warn ("Object ID=%d not found in global list.", id);

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

	warn ("Object ID=%d not found in draw list.", id);

	return NULL;
}

/*
 * Description     : Add an allocated D3D9Object to the factory draw list.
 * unsigned int id : A D3D9ObjectFactory ID already allocated
 * Return          : The D3D9Object hidden, or NULL if not found
 */
D3D9Object *
D3D9ObjectFactory_show (
	unsigned int id
) {
	D3D9ObjectFactory_lock ();

	D3D9Object *object;

	if (!(object = D3D9ObjectFactory_get (id))) {
		warn ("Cannot show object ID=%d.", id);
		return NULL;
	}

	// Put the object shown in the last position of the allObject list, so
	// it keeps a coherent order when hide_all / show_all is called.
	bb_queue_put_last (&d3d9ObjectFactory.allObjects, object);

	// If already exists, put it to the last position
	if (bb_queue_exists (&d3d9ObjectFactory.drawObjects, object)) {
		bb_queue_put_last (&d3d9ObjectFactory.drawObjects, object);
	}
	else {
		// Add the object in the draw list
		bb_queue_add (&d3d9ObjectFactory.drawObjects, object);
	}

	D3D9ObjectFactory_release ();

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
	D3D9ObjectFactory_lock ();

	foreach_bbqueue_item (&d3d9ObjectFactory.allObjects, D3D9Object *object)
	{
		if (!bb_queue_exists (&d3d9ObjectFactory.drawObjects, object))
			bb_queue_add (&d3d9ObjectFactory.drawObjects, object);
	}

	D3D9ObjectFactory_release ();
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
	D3D9ObjectFactory_lock ();

	D3D9Object *object;

	if (!(object = D3D9ObjectFactory_get_draw (id))) {
		warn ("Cannot hide object ID=%d.", id);
		return NULL;
	}

	// Remove from the draw list
	if (!(bb_queue_exists (&d3d9ObjectFactory.drawObjects, object))) {
		warn ("Object ID=%d doesn't exist in the draw list. Abort hide.", id);
		return NULL;
	}

	bb_queue_remv (&d3d9ObjectFactory.drawObjects, object);

	D3D9ObjectFactory_release ();

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
	D3D9ObjectFactory_lock ();

	// Remove all D3D9Objects from the drawObjects list
	while (bb_queue_get_length (&d3d9ObjectFactory.drawObjects)) {
		bb_queue_pop (&d3d9ObjectFactory.drawObjects);
	}

	D3D9ObjectFactory_release ();
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
	D3D9ObjectFactory_lock ();

	D3D9Object * object;

	if (!(object = D3D9ObjectFactory_get (id))) {
		warn ("Cannot find the object to delete.");
		return;
	}

	if (bb_queue_exists (&d3d9ObjectFactory.drawObjects, object)) {
		bb_queue_remv (&d3d9ObjectFactory.drawObjects, object);
	}

	bb_queue_remv (&d3d9ObjectFactory.allObjects, object);

	D3D9Object_free (object);

	D3D9ObjectFactory_release ();
}


/*
 * Description     : Remove all the allocated D3D9Object from the working factory lists
 * Return          : void
 */
void
D3D9ObjectFactory_delete_all (
	void
) {
	D3D9ObjectFactory_lock ();

	// Remove all D3D9Objects from the drawObjects list
	while (bb_queue_get_length (&d3d9ObjectFactory.drawObjects)) {
		bb_queue_pop (&d3d9ObjectFactory.drawObjects);
	}

	// Remove all D3D9Objects from the global list
	while (bb_queue_get_length (&d3d9ObjectFactory.allObjects)) {
		D3D9Object *object = bb_queue_pop (&d3d9ObjectFactory.allObjects);

		// Free the memory
		D3D9Object_free (object);
	}

	D3D9ObjectFactory_release ();
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


/*
 * Description : Lock the mutex shared with all the d3d9objects
 * Return      : void
 */
void
D3D9ObjectFactory_lock (
	void
) {
	WaitForSingleObject (d3d9ObjectFactory.mutex, INFINITE);
}


/*
 * Description : Release the mutex shared with all the d3d9objects
 * Return      : void
 */
void
D3D9ObjectFactory_release (
	void
) {
	ReleaseMutex (d3d9ObjectFactory.mutex);
}


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
) {
	this->x = x;
	this->y = y;
}

/*
 * Description                 : Initialize an allocated D3D9ObjectRect object.
 * int x, y                    : {x, y} position of the rectangle
 * int w, h                    : width and height of the rectangle
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
	D3D9ObjectFactory_lock ();

	D3D9ObjectRect * rect = &this->rect;

	// Fill the structure
	this->x = x;
	this->y = y;
	rect->w = w;
	rect->h = h;
	rect->color = D3DCOLOR_RGBA (r, g, b, 255);

	dbg ("Rectangle <ID=%d | x=%d | y=%d | w=%d | h=%d | rgb=%02X%02X%02X> has been created.", this->id, x, y, w, h, r, g, b);

	D3D9ObjectFactory_add (this);

	D3D9ObjectFactory_release ();
	return true;
}

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
) {
	D3D9ObjectFactory_lock ();

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
		warn ("Cannot create the font ID=%d.", this->id);
		D3D9ObjectFactory_release ();
		return false;
	}

	// Fill the structure
	this->x = x;
	this->y = y;
	text->r = r;
	text->g = g;
	text->b = b;
	text->string = strdup (string);
	text->opacity = (opacity * 255 > 255) ? 255 : opacity * 255;

	dbg ("Text <ID=%d | string=<%s> | x=%d | y=%d | rgb=%02X%02X%02X | opacity=%d> has been created.", this->id, string, x, y, r, g, b, text->opacity);

	D3D9ObjectFactory_add (this);

	D3D9ObjectFactory_release ();
	return true;
}

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
) {
	this->string = strdup (string);
	this->r = r;
	this->g = g;
	this->b = b;
	this->opacity = (opacity * 255 > 255) ? 255 : opacity * 255;
	dbg ("opacity = %d / %f", this->opacity, opacity);
}

/*
 * Description                 : Initialize an allocated D3D9ObjectSprite object.
 * D3D9Object * this           : An allocated D3D9Object
 * IDirect3DDevice9 * pDevice  : An allocated IDirect3DDevice9
 * char * filePath             : Absolute or relative path of the image (.bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga)
 * int x, y                    : {x, y} position of the sprite
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
) {
	D3D9ObjectFactory_lock ();

	D3D9ObjectSprite * sprite = &this->sprite;

	// Fill the structure
	this->x = x;
	this->y = y;
	sprite->opacity = (opacity * 255 > 255) ? 255 : opacity * 255;
	sprite->filePath = strdup (filePath);

	dbg ("Sprite <ID=%d | filePath=<%s> | x=%d | y=%d | opacity=%.2f> has been created.", this->id, filePath, x, y, opacity);

	// Only DirectX thread can call safely D3DXCreateTextureFromFile and D3DXCreateSprite.
	bb_queue_add (&d3d9ObjectFactory.spriteToInstanciate, this);

	D3D9ObjectFactory_release ();

	while (sprite->status == D3D9_OBJECT_SPRITE_NOT_READY) {
		// Active waiting until the DirectX thread initialize the directx objects
		Sleep (1);
	}

	if (sprite->status == D3D9_OBJECT_SPRITE_ERROR) {
		return false;
	}

	return true;
}

/*
 * Description                 : Initialize D3D9ObjectSprite DirectX objects.
 *                               /!\ This function must be called only from the DirectX thread.
 * IDirect3DDevice9 * pDevice  : An allocated IDirect3DDevice9
 * Return                      : void
 */
void
D3D9ObjectSprite_init_directx (
	IDirect3DDevice9 * pDevice
) {
	while (bb_queue_get_length (&d3d9ObjectFactory.spriteToInstanciate))
	{
		D3D9Object * this = bb_queue_pop (&d3d9ObjectFactory.spriteToInstanciate);
		D3D9ObjectSprite * sprite = &this->sprite;

		// Create the texture
		if ((D3DXCreateTextureFromFile (pDevice, sprite->filePath, &sprite->texture)) != D3D_OK) {
			warn ("Cannot create the texture <%s>.", sprite->filePath);
			sprite->status = D3D9_OBJECT_SPRITE_ERROR;
			continue;
		}

		// Create the sprite
		if ((D3DXCreateSprite (pDevice, &sprite->sprite)) != D3D_OK) {
			warn ("Cannot create the sprite.");
			sprite->status = D3D9_OBJECT_SPRITE_ERROR;
			continue;
		}

		D3D9ObjectFactory_add (this);

		sprite->status = D3D9_OBJECT_SPRITE_READY;
	}
}


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
) {
	D3DRECT rectPosition = {x, y, x + rect->w, y + rect->h};
	pDevice->lpVtbl->Clear (pDevice, 1, &rectPosition, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, rect->color, 0,  0);
}

/*
 * Description                 : Draw text at a given position / color on the screen
 * D3D9ObjectText *text        : An allocated D3D9ObjectText
 * int x, y                    : {x, y} position of the text
 * IDirect3DDevice9 * pDevice  : An allocated d3d9 device
 */
void
D3D9ObjectText_draw (
	D3D9ObjectText *this,
	int x, int y,
	IDirect3DDevice9 * pDevice
) {
    RECT rect;
    ID3DXFont *font = this->font;
	D3DCOLOR color = D3DCOLOR_RGBA (this->r, this->g, this->b, this->opacity);

    SetRect (&rect, x, y, x, y);
    font->lpVtbl->DrawText (font, NULL, this->string, -1, &rect, DT_NOCLIP | DT_LEFT, color);
}

/*
 * Description                    : Draw a sprite at a given position on the screen
 * D3D9ObjectSprite *spriteObject : An allocated D3D9ObjectSprite.
 * int x, y                       : {x, y} position of the sprite
 * Return                         : void
 */
void
D3D9ObjectSprite_draw (
	D3D9ObjectSprite *this,
	int x, int y
) {
	ID3DXSprite * sprite        = this->sprite;
	IDirect3DTexture9 * texture = this->texture;
	D3DXVECTOR3 position3D      = {x, y, 0.0};
	int opacity = (this->opacity * 255 > 255) ? 255 : this->opacity * 255;
	D3DCOLOR color              = D3DCOLOR_ARGB (opacity, 255, 255, 255);

	sprite->lpVtbl->Begin (sprite, D3DXSPRITE_ALPHABLEND);
	sprite->lpVtbl->Draw (sprite, texture, NULL, NULL, &position3D, color);
	sprite->lpVtbl->End (sprite);
}

/*
 * Description : Free an allocated D3D9Object
 *               /!\ The factory MUST BE LOCKED when calling this function.
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
			if (font)
				font->lpVtbl->Release (font);
		} break;

		case D3D9_OBJECT_SPRITE: {
			ID3DXSprite * sprite = this->sprite.sprite;
			IDirect3DTexture9 * texture = this->sprite.texture;
			if (texture)
				texture->lpVtbl->Release (texture);
			if (sprite)
				sprite->lpVtbl->Release (sprite);
			free (this->sprite.filePath);
		} break;

		default : warn ("Cannot free completely an unknown type."); break;
	}

	free (this);
}
