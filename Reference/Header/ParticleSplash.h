#pragma once
#include "ParticleSystem.h"

BEGIN(Engine)

class CTexture;

class ENGINE_DLL CParticleSplash : public CParticleSystem
{
public:
	explicit CParticleSplash(int numParticles, _float fSize);
	virtual ~CParticleSplash();

public:
	bool Ready_Particle(IDirect3DDevice9* pDevice) override;
	virtual void Reset_Partice(Attribute* attribute) override;
	virtual void Update_Particle(_float fTimeDelat) override;
	virtual void Render_GameObject() override;

	void	Create_Texture(const _tchar* pTexturePath, _int iMaxFrame);

	static CParticleSplash* Create(IDirect3DDevice9* pDevice, _matrix matWorld,
		const _tchar* pTextruePath = L"../Bin/Resource/Texture/Particle/BloodExp2/BloodExp_%d.png", _int iMaxFrame = 7,
		_float fSize = 0.25f, _int iCount = 10);

private:
	virtual void	Free() override;

private:
	bool m_bIsLeft;
	_float m_fGravity;

	CTexture* m_pTexture;

	_int					m_iPicNum;
	_float					m_fFrame = 0.f;
	_float					m_fFrameSpeed = 2.f;
};
END