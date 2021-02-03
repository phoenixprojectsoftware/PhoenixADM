#include "HUD.h"
#include "CL_Util.h"
#include "parsemsg.h"
#include "Watermark.h"

int CHudWatermark::Init()
{
	m_iFlags = 0;

	gHUD.AddHudElem(this);
	return 0;
}

int CHudWatermark::VidInit()
{
	m_iFlags |= HUD_ACTIVE;
	refresh_draw_until = true;
	return 1;
}

int CHudWatermark::Draw(float time)
{
	if (refresh_draw_until) {
		refresh_draw_until = false;
		draw_until = gHUD.m_flTime + 15.0f;
	}

	if (gHUD.m_flTime >= draw_until) {
		m_iFlags &= ~HUD_ACTIVE;
		return 0;
	}

	int r, g, b;

	gEngfuncs.pfnDrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight, "Shooter Tourneys 1.0", r, g, b);
	gEngfuncs.pfnDrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight * 2, "Developed by The Phoenix Project", r, g, b);
	gEngfuncs.pfnDrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight * 3, "Powered by OpenAG and Adrenaline Gamer", r, g, b);
	gEngfuncs.pfnDrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight * 4, "Build date " __DATE__, r, g, b);
	gEngfuncs.pfnDrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight * 5, "Running on creaTive Deathmatch Player", r, g, b);

	if (update_is_available)
		gEngfuncs.pfnDrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight / 2 * 7, "An update is available.", r, g, b);

	return 0;
}