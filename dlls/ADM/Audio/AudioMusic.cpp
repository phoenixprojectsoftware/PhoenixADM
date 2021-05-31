/*
#include "Base/ExtDLL.h"
#include "Util.h"
#include "Base/CBase.h"
#include "Player.h"

#include "AudioShared.h"
#include "AudioMusic.h"

LINK_ENTITY_TO_CLASS(audio_music, AudioMusic);

TYPEDESCRIPTION AudioMusic::m_SaveData[] =
{
	DEFINE_FIELD(AudioMusic, soundName, FIELD_STRING),
	DEFINE_FIELD(AudioMusic, volume, FIELD_FLOAT),
	DEFINE_FIELD(AudioMusic, channelNumber, FIELD_INTEGER)
};

IMPLEMENT_SAVERESTORE(AudioMusic, CBaseEntity);

void AudioMusic::Spawn()
{

}

void AudioMusic::OnPlayerJoin(CBasePlayer* player)
{
	CBaseEntity::OnPlayerJoin(player);

	MESSAGE_BEGIN(MSG_ONE, gmsgSound2DViaString, pev->origin, player->pev);

	WRITE_BYTE(channelNumber);
	WRITE_STRING(STRING(soundName));
	WRITE_BYTE(volume * 255);
	WRITE_BYTE(pev->spawnflags);
	WRITE_SHORT(entindex());

	MESSAGE_END();
}

void AudioMusic::KeyValue(KeyValueData* pkvd)
{
	if (KeyvalueToken(soundName))
	{
		KeyvalueToString(soundName);
	}
	else if (KeyvalueToken(volume))
	{
		KeyvalueToFloat(volume);
		char string[64];
		sprintf( string, "MP3Volume %3d\n");
	}
	else if (KeyvalueToken(channelNumber))
	{
		KeyvalueToInt(channelNumber);
	}
	else
	{
		KeyvaluesFromBase(CBaseEntity);
	}
}

void AudioMusic::Use(CBaseEntity* activator, CBaseEntity* caller, USE_TYPE useType, float value)
{
	MESSAGE_BEGIN(MSG_ALL, gmsgSoundManipulate);

	WRITE_BYTE(Sound_Play);
	WRITE_SHORT(entindex());

	MESSAGE_END();
}

void AudioMusic::Think()
{

}
*/