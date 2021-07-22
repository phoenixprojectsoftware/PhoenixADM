//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// Triangle rendering, if any

#include "HUD/hud.h"
#include "cl_util.h"

// Triangle rendering apis are in gEngfuncs.pTriAPI

#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "triangleapi.h"
#include "Exports.h"
#include "com_model.h"

#include "particleman.h"
#include "tri.h"
extern IParticleMan *g_pParticleMan;

#include "ADM/AdmRenderer.h"
#include "ADM/Components/SimpleSky.h"

/*
=================
HUD_DrawNormalTriangles

Non-transparent triangles-- add them here
=================
*/
void DLLEXPORT HUD_DrawNormalTriangles( void )
{
//	RecClDrawNormalTriangles();

	gHUD.m_Spectator.DrawOverview();
}

#if defined( _TFC )
void RunEventList( void );
#endif

/*
=================
HUD_DrawTransparentTriangles

Render any triangles with transparent rendermode needs here
=================
*/
void DLLEXPORT HUD_DrawTransparentTriangles( void )
{
//	RecClDrawTransparentTriangles();

	triangleapi_t* triAPI = gEngfuncs.pTriAPI;

#if defined( _TFC )
	RunEventList();
#endif

	//gHUD.m_clImgui.TestDrawImgui();
	//gHUD.m_clImgui.DisplayImgui();

	g_Sky.Render( triAPI );

	if ( g_pParticleMan )
		 g_pParticleMan->Update();
}

//#define ORTHOEXAMPLE
//#ifdef ORTHOEXAMPLE
void OrthoExample() // AdmSrc - this ain't my code, lol
{
	gEngfuncs.pTriAPI->RenderMode(kRenderTransAdd); //additive

	// use hotglow, or any other sprite for the texture
	gEngfuncs.pTriAPI->SpriteTexture((struct model_s *)
		gEngfuncs.GetSpritePointer(SPR_Load("sprites/hotglow.spr")),
		0);

	gEngfuncs.pTriAPI->CullFace(TRI_NONE); //no culling
	gEngfuncs.pTriAPI->Begin(TRI_QUADS); //start our quad

	//remember, always list vertices in counter-clockwise 
	// order, unless you want the quad to be backwards =)
	// the third value of vertex3f will always be 0 in ortho mode, 
	// don't change it unless you wan't funny things to happen.

	//top left
	gEngfuncs.pTriAPI->TexCoord2f(0.0f, 1.0f);
	gEngfuncs.pTriAPI->Vertex3f(0, 0, 0);

	//bottom left
	gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
	gEngfuncs.pTriAPI->Vertex3f(0, ScreenHeight, 0);

	//bottom right
	gEngfuncs.pTriAPI->TexCoord2f(1.0f, 0.0f);
	gEngfuncs.pTriAPI->Vertex3f(ScreenWidth, ScreenHeight, 0);

	//top right
	gEngfuncs.pTriAPI->TexCoord2f(1.0f, 1.0f);
	gEngfuncs.pTriAPI->Vertex3f(ScreenWidth, 0, 0);

	gEngfuncs.pTriAPI->End(); //end our list of vertexes
	gEngfuncs.pTriAPI->RenderMode(kRenderNormal); //return to normal
}
//#endif

/*
=================
HUD_DrawOrthoTriangles
Orthogonal Triangles -- (relative to resolution,
smackdab on the screen) add them here
=================
*/

void HUD_DrawOrthoTriangles(void)
{
//#ifdef ORTHOEXAMPLE
//	OrthoExample();
//#endif

	ADM_RendDrawLine(0.2f, 0.9f, 0.9f, 0.9f);
	ADM_RendDrawLine(0.1f, 0.85f, 0.8f, 0.85f);
	ADM_RendDrawLine(0.2f, 0.9f, 0.1f, 0.85f);
	ADM_RendDrawLine(0.9f, 0.9f, 0.8f, 0.85f);

}

#include "com_model.h"

typedef struct
{
	short type;
	short texFormat;
	int width;
	int height;
} msprite_t;

model_s* TRI_pModel;

void TRI_GetSpriteParms(model_t* pSprite, int* frameWidth, int* frameHeight)
{
	if (!pSprite || pSprite->type != mod_sprite) return;

	msprite_t* pSpr = (msprite_t*)pSprite->cache.data;

	*frameWidth = pSpr->width;
	*frameHeight = pSpr->height;
}

void TRI_SprAdjustSize(int* x, int* y, int* w, int* h)
{
	float xscale, yscale;

	if (!x && !y && !w && !h) return;

	float yfactor = (float)ScreenWidth / (float)ScreenHeight;

	xscale = ((float)ScreenWidth / 1536.0f);
	yscale = ((float)ScreenHeight / 1536.0f) * yfactor;

	if (x)
	{
		bool swap_x = 0;
		if (*x > ScreenWidth / 2) swap_x = 1;
		if (swap_x) *x = (float)ScreenWidth - *x;
		*x = xscale;
		if (swap_x) *x = (float)ScreenWidth - *x;
	}

	if (y)
	{
			bool swap_y = 0;
			if (*y > ScreenHeight / 2) swap_y = 1;
			if (swap_y) *y = (float)ScreenHeight - *y;
			*y = yscale;
			if (swap_y) *y = (float)ScreenHeight - *y;
	}

	if (w) *w = xscale;
	if (h) *h = yscale;
}

void TRI_SprDrawStretchPic(model_t* pModel, int frame, float x, float y, float w, float h, float s1, float t1, float s2, float t2)
{
		gEngfuncs.pTriAPI->SpriteTexture(pModel, frame);

		gEngfuncs.pTriAPI->Begin(TRI_QUADS);

		gEngfuncs.pTriAPI->TexCoord2f(s1, t1);
		gEngfuncs.pTriAPI->Vertex3f(x, y, 0);

		gEngfuncs.pTriAPI->TexCoord2f(s2, t1);
		gEngfuncs.pTriAPI->Vertex3f(x + w, y, 0);

		gEngfuncs.pTriAPI->TexCoord2f(s2, t2);
		gEngfuncs.pTriAPI->Vertex3f(x + w, y + h, 0);

		gEngfuncs.pTriAPI->TexCoord2f(s1, t2);
		gEngfuncs.pTriAPI->Vertex3f(x, y + h, 0);

		gEngfuncs.pTriAPI->End();
}


void TRI_SprDrawGeneric(int frame, int x, int y, wrect_t* prc)
{
		float 	s1, s2, t1, t2;
		int 	w, h;

		TRI_GetSpriteParms(TRI_pModel, &w, &h);

		if (prc)
		{
		  wrect_t rc = *prc;

		  if (rc.left <= 0 || rc.left >=  w) rc.left = 0;
		  if (rc.top <= 0 || rc.top >=  h) rc.top = 0;
		  if (rc.right <= 0 || rc.right > w) rc.right = w;
		  if (rc.bottom <= 0 || rc.bottom > h) rc.bottom = h;

		  s1 = (float)rc.left / w;
		  t1 = (float)rc.top / h;
		  s2 = (float)rc.right / w;
		  t2 = (float)rc.bottom / h;
		  w = rc.right - rc.left;
		  h = rc.bottom - rc.top;
		}
		else
		{
				s1 = t1 = 0.0f;
				s2 = t2 = 1.0f;
		}

		TRI_SprAdjustSize(&x, &y, &w, &h);
		TRI_SprDrawStretchPic(TRI_pModel, frame, x, y, w, h, s1, t1, s2, t2);
}

void TRI_SprDrawAdditive(int frame, int x, int y, wrect_t* prc)
{
	gEngfuncs.pTriAPI->RenderMode(kRenderTransAdd);

	TRI_SprDrawGeneric(frame, x, y, prc);

	gEngfuncs.pTriAPI->RenderMode(kRenderNormal);
}

void TRI_SprSet(HSPRITE spr, int r, int g, int b)
{
	TRI_pModel = (model_s*)gEngfuncs.GetSpritePointer(spr);
	gEngfuncs.pTriAPI->Color4ub(r, g, b, 255);
}

void TRI_FillRGBA(int x, int y, int width, int height, int r, int g, int b, int a)
{
	TRI_SprAdjustSize(&x, &y, &width, &height);
	gEngfuncs.pfnFillRGBA(x, y, width, height, r, g, b, a);
}