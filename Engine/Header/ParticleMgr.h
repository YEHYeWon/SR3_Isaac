#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CParticleMgr : public CBase
{
	DECLARE_SINGLETON(CParticleMgr)
private:
	explicit CParticleMgr();
	virtual ~CParticleMgr();

public:
	void PlaySound(TCHAR* pSoundKey, CHANNEL_ID eID, _float fVolume);
	void PlayBGM(TCHAR* pSoundKey, _float fVolume);
	void StopSound(CHANNEL_ID eID);
	void StopAll();
	void SetChannelVolume(CHANNEL_ID eID, _float fVolume);

	void	Ready_Sound();

private:
	void LoadSoundFile(); 

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem; 

private:
	virtual void		Free(void);
};

END