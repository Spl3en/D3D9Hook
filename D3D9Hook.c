#include "D3D9Hook.h"
#include "HookEngine/HookEngine.h"
#include "Scanner/Scanner.h"
#include <stdlib.h>

// ---------- Debugging -------------
#define __DEBUG_OBJECT__ "D3D9Hook"
#include "dbg/dbg.h"


static EnumerationStringAssociation association [D3D9INDEX_VFTABLE_SIZE] = {

	[0 ... D3D9INDEX_VFTABLE_SIZE-1] = {D3D9INDEX_Undefined, "D3D9INDEX_Undefined"},

	associate (D3D9INDEX_QueryInterface),
	associate (D3D9INDEX_AddRef),
	associate (D3D9INDEX_Release),
	associate (D3D9INDEX_TestCooperativeLevel),
	associate (D3D9INDEX_GetAvailableTextureMem),
	associate (D3D9INDEX_EvictManagedResources),
	associate (D3D9INDEX_GetDirect3D),
	associate (D3D9INDEX_GetDeviceCaps),
	associate (D3D9INDEX_GetDisplayMode),
	associate (D3D9INDEX_GetCreationParameters),
	associate (D3D9INDEX_SetCursorProperties),
	associate (D3D9INDEX_SetCursorPosition),
	associate (D3D9INDEX_ShowCursor),
	associate (D3D9INDEX_CreateAdditionalSwapChain),
	associate (D3D9INDEX_GetSwapChain),
	associate (D3D9INDEX_GetNumberOfSwapChains),
	associate (D3D9INDEX_Reset),
	associate (D3D9INDEX_Present),
	associate (D3D9INDEX_GetBackBuffer),
	associate (D3D9INDEX_GetRasterStatus),
	associate (D3D9INDEX_SetDialogBoxMode),
	associate (D3D9INDEX_SetGammaRamp),
	associate (D3D9INDEX_GetGammaRamp),
	associate (D3D9INDEX_CreateTexture),
	associate (D3D9INDEX_CreateVolumeTexture),
	associate (D3D9INDEX_CreateCubeTexture),
	associate (D3D9INDEX_CreateVertexBuffer),
	associate (D3D9INDEX_CreateIndexBuffer),
	associate (D3D9INDEX_CreateRenderTarget),
	associate (D3D9INDEX_CreateDepthStencilSurface),
	associate (D3D9INDEX_UpdateSurface),
	associate (D3D9INDEX_UpdateTexture),
	associate (D3D9INDEX_GetRenderTargetData),
	associate (D3D9INDEX_GetFrontBufferData),
	associate (D3D9INDEX_StretchRect),
	associate (D3D9INDEX_ColorFill),
	associate (D3D9INDEX_CreateOffscreenPlainSurface),
	associate (D3D9INDEX_SetRenderTarget),
	associate (D3D9INDEX_GetRenderTarget),
	associate (D3D9INDEX_SetDepthStencilSurface),
	associate (D3D9INDEX_GetDepthStencilSurface),
	associate (D3D9INDEX_BeginScene),
	associate (D3D9INDEX_EndScene),
	associate (D3D9INDEX_Clear),
	associate (D3D9INDEX_SetTransform),
	associate (D3D9INDEX_GetTransform),
	associate (D3D9INDEX_MultiplyTransform),
	associate (D3D9INDEX_SetViewport),
	associate (D3D9INDEX_GetViewport),
	associate (D3D9INDEX_SetMaterial),
	associate (D3D9INDEX_GetMaterial),
	associate (D3D9INDEX_SetLight),
	associate (D3D9INDEX_GetLight),
	associate (D3D9INDEX_LightEnable),
	associate (D3D9INDEX_GetLightEnable),
	associate (D3D9INDEX_SetClipPlane),
	associate (D3D9INDEX_GetClipPlane),
	associate (D3D9INDEX_SetRenderState),
	associate (D3D9INDEX_GetRenderState),
	associate (D3D9INDEX_CreateStateBlock),
	associate (D3D9INDEX_BeginStateBlock),
	associate (D3D9INDEX_EndStateBlock),
	associate (D3D9INDEX_SetClipStatus),
	associate (D3D9INDEX_GetClipStatus),
	associate (D3D9INDEX_GetTexture),
	associate (D3D9INDEX_SetTexture),
	associate (D3D9INDEX_GetTextureStageState),
	associate (D3D9INDEX_SetTextureStageState),
	associate (D3D9INDEX_GetSamplerState),
	associate (D3D9INDEX_SetSamplerState),
	associate (D3D9INDEX_ValidateDevice),
	associate (D3D9INDEX_SetPaletteEntries),
	associate (D3D9INDEX_GetPaletteEntries),
	associate (D3D9INDEX_SetCurrentTexturePalette),
	associate (D3D9INDEX_GetCurrentTexturePalette),
	associate (D3D9INDEX_SetScissorRect),
	associate (D3D9INDEX_GetScissorRect),
	associate (D3D9INDEX_SetSoftwareVertexProcessing),
	associate (D3D9INDEX_GetSoftwareVertexProcessing),
	associate (D3D9INDEX_SetNPatchMode),
	associate (D3D9INDEX_GetNPatchMode),
	associate (D3D9INDEX_DrawPrimitive),
	associate (D3D9INDEX_DrawIndexedPrimitive),
	associate (D3D9INDEX_DrawPrimitiveUP),
	associate (D3D9INDEX_DrawIndexedPrimitiveUP),
	associate (D3D9INDEX_ProcessVertices),
	associate (D3D9INDEX_CreateVertexDeclaration),
	associate (D3D9INDEX_SetVertexDeclaration),
	associate (D3D9INDEX_GetVertexDeclaration),
	associate (D3D9INDEX_SetFVF),
	associate (D3D9INDEX_GetFVF),
	associate (D3D9INDEX_CreateVertexShader),
	associate (D3D9INDEX_SetVertexShader),
	associate (D3D9INDEX_GetVertexShader),
	associate (D3D9INDEX_SetVertexShaderConstantF),
	associate (D3D9INDEX_GetVertexShaderConstantF),
	associate (D3D9INDEX_SetVertexShaderConstantI),
	associate (D3D9INDEX_GetVertexShaderConstantI),
	associate (D3D9INDEX_SetVertexShaderConstantB),
	associate (D3D9INDEX_GetVertexShaderConstantB),
	associate (D3D9INDEX_SetStreamSource),
	associate (D3D9INDEX_GetStreamSource),
	associate (D3D9INDEX_SetStreamSourceFreq),
	associate (D3D9INDEX_GetStreamSourceFreq),
	associate (D3D9INDEX_SetIndices),
	associate (D3D9INDEX_GetIndices),
	associate (D3D9INDEX_CreatePixelShader),
	associate (D3D9INDEX_SetPixelShader),
	associate (D3D9INDEX_GetPixelShader),
	associate (D3D9INDEX_SetPixelShaderConstantF),
	associate (D3D9INDEX_GetPixelShaderConstantF),
	associate (D3D9INDEX_SetPixelShaderConstantI),
	associate (D3D9INDEX_GetPixelShaderConstantI),
	associate (D3D9INDEX_SetPixelShaderConstantB),
	associate (D3D9INDEX_GetPixelShaderConstantB),
	associate (D3D9INDEX_DrawRectPatch),
	associate (D3D9INDEX_DrawTriPatch),
	associate (D3D9INDEX_DeletePatch),
	associate (D3D9INDEX_CreateQuery)
};


/*
 * Description 	: Allocate a new D3D9Hook structure.
 * DWORD baseAddress : Base address of the module
 * DWORD hudChatInstance : The address of hudChat
 * Return		: A pointer to an allocated D3D9Hook.
 */
D3D9Hook *
D3D9Hook_new (
	DWORD baseAddress,
	DWORD sizeOfModule
) {
	D3D9Hook *this;

	if ((this = calloc (1, sizeof(D3D9Hook))) == NULL)
		return NULL;

	if (!D3D9Hook_init (this, baseAddress, sizeOfModule)) {
		D3D9Hook_free (this);
		return NULL;
	}

	return this;
}

/*
 * Description : Initialize an allocated D3D9Hook structure.
 * D3D9Hook *this : An allocated D3D9Hook to initialize.
 * DWORD baseAddress : Base address of the module
 * DWORD hudChatInstance : The address of hudChat
 * Return : true on success, false on failure.
 */
bool
D3D9Hook_init (
	D3D9Hook *this,
	DWORD baseAddress,
	DWORD sizeOfModule
) {
	unsigned char pattern [] = {
		/*	C706 084E8C5C     mov [dword ds:esi], d3d9.5C8C4E08
			8986 68300000     mov [dword ds:esi+3068], eax
			8986 60300000     mov [dword ds:esi+3060], eax 		*/
			0xC7, 0x06, '?', '?', '?', '?',
			0x89, 0x86, '?', '?', '?', '?',
			0x89, 0x86, '?', '?', '?', '?'
	};

	// Get the d3d9 device vftable
	DWORD * pDeviceVftable = (DWORD *) mem_scanner ("pDeviceVftable",
		baseAddress, sizeOfModule,
		pattern,

		"xx????"
		"xx????"
		"xx????",

		"xx????"
		"xxxxxx"
		"xxxxxx"
	);

	if (!pDeviceVftable) {
		dbg ("pDeviceVftable pattern not found.");
		return false;
	}

	dbg ("pDeviceVftable found : 0x%.08X",  pDeviceVftable);
	this->vftable = pDeviceVftable;

	return true;
}


/*
 * Description :
 * D3D9Hook *this : An allocated D3D9Hook
 * D3D9VirtualFunctionTableIndex index : Index of the function to hook
 * ULONG_PTR hookFunction : Hook function
 * Return : DWORD address of the original function hooked, or 0 if error
 */
void *
D3D9Hook_hook (
	D3D9Hook *this,
	D3D9VirtualFunctionTableIndex index,
	ULONG_PTR hookFunction
) {
	if (!D3D9VirtualFunctionTableIndex_is_valid (index)) {
		dbg ("Invalid D3D9VirtualFunctionTableIndex : %d", index);
		return 0;
	}

	char * functionName = D3D9VirtualFunctionTableIndex_to_string (index);
	if (!HookEngine_hook (this->vftable [index], hookFunction)) {
		dbg ("Cannot hook %s.", functionName);
		return 0;
	}

	void * originalFunction = (void *) HookEngine_get_original_function (hookFunction);

	if (!originalFunction) {
		dbg ("Cannot get original function for 0x%.08X.", hookFunction);
		return 0;
	}

	dbg ("%s has been hooked. Original function address = 0x%.08X.", functionName, originalFunction);

	return originalFunction;
}

/*
 * Description : Free an allocated D3D9Hook structure.
 * D3D9Hook *this : An allocated D3D9Hook to free.
 */
void
D3D9Hook_free (
	D3D9Hook *this
) {
	if (this != NULL) {
		free (this);
	}
}

/*
 * Description : Check if a D3D9VirtualFunctionTableIndex is valid
 * D3D9VirtualFunctionTableIndex index : An allocated D3D9VirtualFunctionTableIndex
 * Return : bool true on success, false otherwise
 */
bool
D3D9VirtualFunctionTableIndex_is_valid (
	D3D9VirtualFunctionTableIndex index
) {
	return (index < D3D9INDEX_VFTABLE_SIZE
		 && index >= 0);
}


/*
 * Description : Convert a D3D9VirtualFunctionTableIndex to string
 * D3D9VirtualFunctionTableIndex index : The D3D9VirtualFunctionTableIndex to convert
 * Return : char * the string
 */
char *
D3D9VirtualFunctionTableIndex_to_string (
	D3D9VirtualFunctionTableIndex index
) {
	if (!D3D9VirtualFunctionTableIndex_is_valid (index)) {
		return NULL;
	}

	return association [index].string;
}


/*
 * Description : Unit tests checking if a D3D9Hook is coherent
 * D3D9Hook *this : The instance to test
 * Return : true on success, false on failure
 */
bool
D3D9Hook_test (
	D3D9Hook *this
) {
	if (!this) {
		fail ("Instance is NULL");
		return false;
	}

	return true;
}
