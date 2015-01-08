// --- Author : Moreau Cyril - Spl3en
#pragma once

// ---------- Includes ------------
#include "Utils/Utils.h"
#include "dx/d3d9.h"
#include "dx/d3dx9.h"

// ---------- Defines -------------


// ------ Structure declaration -------
typedef struct _D3D9Hook
{
	DWORD *vftable;

}	D3D9Hook;

typedef enum
{
	D3D9INDEX_QueryInterface, // 0
	D3D9INDEX_AddRef, // 1
	D3D9INDEX_Release, // 2
	D3D9INDEX_TestCooperativeLevel, // 3
	D3D9INDEX_GetAvailableTextureMem, // 4
	D3D9INDEX_EvictManagedResources, // 5
	D3D9INDEX_GetDirect3D, // 6
	D3D9INDEX_GetDeviceCaps, // 7
	D3D9INDEX_GetDisplayMode, // 8
	D3D9INDEX_GetCreationParameters, // 9
	D3D9INDEX_SetCursorProperties, // 10
	D3D9INDEX_SetCursorPosition, // 11
	D3D9INDEX_ShowCursor, // 12
	D3D9INDEX_CreateAdditionalSwapChain, // 13
	D3D9INDEX_GetSwapChain, // 14
	D3D9INDEX_GetNumberOfSwapChains, // 15
	D3D9INDEX_Reset, // 16
	D3D9INDEX_Present, // 17
	D3D9INDEX_GetBackBuffer, // 18
	D3D9INDEX_GetRasterStatus, // 19
	D3D9INDEX_SetDialogBoxMode, // 20
	D3D9INDEX_SetGammaRamp, // 21
	D3D9INDEX_GetGammaRamp, // 22
	D3D9INDEX_CreateTexture, // 23
	D3D9INDEX_CreateVolumeTexture, // 24
	D3D9INDEX_CreateCubeTexture, // 25
	D3D9INDEX_CreateVertexBuffer, // 26
	D3D9INDEX_CreateIndexBuffer, // 27
	D3D9INDEX_CreateRenderTarget, // 28
	D3D9INDEX_CreateDepthStencilSurface, // 29
	D3D9INDEX_UpdateSurface, // 30
	D3D9INDEX_UpdateTexture, // 31
	D3D9INDEX_GetRenderTargetData, // 32
	D3D9INDEX_GetFrontBufferData, // 33
	D3D9INDEX_StretchRect, // 34
	D3D9INDEX_ColorFill, // 35
	D3D9INDEX_CreateOffscreenPlainSurface, // 36
	D3D9INDEX_SetRenderTarget, // 37
	D3D9INDEX_GetRenderTarget, // 38
	D3D9INDEX_SetDepthStencilSurface, // 39
	D3D9INDEX_GetDepthStencilSurface, // 40
	D3D9INDEX_BeginScene, // 41
	D3D9INDEX_EndScene, // 42
	D3D9INDEX_Clear, // 43
	D3D9INDEX_SetTransform, // 44
	D3D9INDEX_GetTransform, // 45
	D3D9INDEX_MultiplyTransform, // 46
	D3D9INDEX_SetViewport, // 47
	D3D9INDEX_GetViewport, // 48
	D3D9INDEX_SetMaterial, // 49
	D3D9INDEX_GetMaterial, // 50
	D3D9INDEX_SetLight, // 51
	D3D9INDEX_GetLight, // 52
	D3D9INDEX_LightEnable, // 53
	D3D9INDEX_GetLightEnable, // 54
	D3D9INDEX_SetClipPlane, // 55
	D3D9INDEX_GetClipPlane, // 56
	D3D9INDEX_SetRenderState, // 57
	D3D9INDEX_GetRenderState, // 58
	D3D9INDEX_CreateStateBlock, // 59
	D3D9INDEX_BeginStateBlock, // 60
	D3D9INDEX_EndStateBlock, // 61
	D3D9INDEX_SetClipStatus, // 62
	D3D9INDEX_GetClipStatus, // 63
	D3D9INDEX_GetTexture, // 64
	D3D9INDEX_SetTexture, // 65
	D3D9INDEX_GetTextureStageState, // 66
	D3D9INDEX_SetTextureStageState, // 67
	D3D9INDEX_GetSamplerState, // 68
	D3D9INDEX_SetSamplerState, // 69
	D3D9INDEX_ValidateDevice, // 70
	D3D9INDEX_SetPaletteEntries, // 71
	D3D9INDEX_GetPaletteEntries, // 72
	D3D9INDEX_SetCurrentTexturePalette, // 73
	D3D9INDEX_GetCurrentTexturePalette, // 74
	D3D9INDEX_SetScissorRect, // 75
	D3D9INDEX_GetScissorRect, // 76
	D3D9INDEX_SetSoftwareVertexProcessing, // 77
	D3D9INDEX_GetSoftwareVertexProcessing, // 78
	D3D9INDEX_SetNPatchMode, // 79
	D3D9INDEX_GetNPatchMode, // 80
	D3D9INDEX_DrawPrimitive, // 81
	D3D9INDEX_DrawIndexedPrimitive, // 82
	D3D9INDEX_DrawPrimitiveUP, // 83
	D3D9INDEX_DrawIndexedPrimitiveUP, // 84
	D3D9INDEX_ProcessVertices, // 85
	D3D9INDEX_CreateVertexDeclaration, // 86
	D3D9INDEX_SetVertexDeclaration, // 87
	D3D9INDEX_GetVertexDeclaration, // 88
	D3D9INDEX_SetFVF, // 89
	D3D9INDEX_GetFVF, // 90
	D3D9INDEX_CreateVertexShader, // 91
	D3D9INDEX_SetVertexShader, // 92
	D3D9INDEX_GetVertexShader, // 93
	D3D9INDEX_SetVertexShaderConstantF, // 94
	D3D9INDEX_GetVertexShaderConstantF, // 95
	D3D9INDEX_SetVertexShaderConstantI, // 96
	D3D9INDEX_GetVertexShaderConstantI, // 97
	D3D9INDEX_SetVertexShaderConstantB, // 98
	D3D9INDEX_GetVertexShaderConstantB, // 99
	D3D9INDEX_SetStreamSource, // 100
	D3D9INDEX_GetStreamSource, // 101
	D3D9INDEX_SetStreamSourceFreq, // 102
	D3D9INDEX_GetStreamSourceFreq, // 103
	D3D9INDEX_SetIndices, // 104
	D3D9INDEX_GetIndices, // 105
	D3D9INDEX_CreatePixelShader, // 106
	D3D9INDEX_SetPixelShader, // 107
	D3D9INDEX_GetPixelShader, // 108
	D3D9INDEX_SetPixelShaderConstantF, // 109
	D3D9INDEX_GetPixelShaderConstantF, // 110
	D3D9INDEX_SetPixelShaderConstantI, // 111
	D3D9INDEX_GetPixelShaderConstantI, // 112
	D3D9INDEX_SetPixelShaderConstantB, // 113
	D3D9INDEX_GetPixelShaderConstantB, // 114
	D3D9INDEX_DrawRectPatch, // 115
	D3D9INDEX_DrawTriPatch, // 116
	D3D9INDEX_DeletePatch, // 117
	D3D9INDEX_CreateQuery, // 118

	D3D9INDEX_Undefined, // Unknown index
	D3D9INDEX_VFTABLE_SIZE // Always at the end

} D3D9VirtualFunctionTableIndex;


// --------- Allocators ---------

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
);

// ----------- Functions ------------

/*
 * Description : Initialize an allocated D3D9Hook structure.
 * D3D9Hook *this : An allocated D3D9Hook to initialize.
 * DWORD baseAddress : Base address of the module
 * DWORD hudChatInstance : The address of hudChat
 */
bool
D3D9Hook_init (
	D3D9Hook *this,
	DWORD baseAddress,
	DWORD sizeOfModule
);


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
);

/*
 * Description : Unit tests checking if a D3D9Hook is coherent
 * D3D9Hook *this : The instance to test
 * Return : true on success, false on failure
 */
bool
D3D9Hook_test (
	D3D9Hook *this
);

/*
 * Description : Convert a D3D9VirtualFunctionTableIndex to string
 * D3D9VirtualFunctionTableIndex index : The D3D9VirtualFunctionTableIndex to convert
 * Return : char * the string
 */
char *
D3D9VirtualFunctionTableIndex_to_string (
	D3D9VirtualFunctionTableIndex index
);

/*
 * Description : Check if a D3D9VirtualFunctionTableIndex is valid
 * D3D9VirtualFunctionTableIndex index : An allocated D3D9VirtualFunctionTableIndex
 * Return : bool true on success, false otherwise
 */
bool
D3D9VirtualFunctionTableIndex_is_valid (
	D3D9VirtualFunctionTableIndex index
);

// --------- Destructors ----------

/*
 * Description : Free an allocated D3D9Hook structure.
 * D3D9Hook *this : An allocated D3D9Hook to free.
 */
void
D3D9Hook_free (
	D3D9Hook *this
);


