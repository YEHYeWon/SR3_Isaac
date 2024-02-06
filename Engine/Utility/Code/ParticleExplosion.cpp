#include "..\..\Header\ParticleSystem.h"
#include "ParticleExplosion.h"

CParticleExplosion::CParticleExplosion(_vec3* vOrigin, int numParticles)
{
	m_vOrigin = *vOrigin;
	m_fSize = 0.7f;
	m_VbSize = 2048;
	m_VbOffset = 0;
	m_VbBatchSize = 512;

	m_vMinVelocity = _vec3(-0.5f, 0.0f, -1.0f);
	m_vMinVelocity = _vec3(0.5f, 1.0f, 1.0f);

	for (int i = 0; i < numParticles; i++)
		Add_Particle();
}

CParticleExplosion::~CParticleExplosion()
{
	Free();
}

void CParticleExplosion::Free()
{
	Engine::Safe_Release(m_pVb);
	Engine::Safe_Release(m_pTex);
}

bool CParticleExplosion::Ready_Particle(IDirect3DDevice9* pDevice)
{
	m_pGraphicDev = pDevice;

	HRESULT hr = 0;

	hr = pDevice->CreateVertexBuffer(
		m_VbSize * sizeof(Particle),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		D3DFVF_XYZ | D3DFVF_DIFFUSE,
		D3DPOOL_DEFAULT, // D3DPOOL_MANAGED can't be used with D3DUSAGE_DYNAMIC 
		&m_pVb,
		0);

	FAILED_CHECK_RETURN(hr, E_FAIL);

	wstring wstr = L"../Bin/Resource/Texture/Particle/explosion.png";

	hr = D3DXCreateTextureFromFile(
		pDevice,
		wstr.c_str(),
		&m_pTex);

	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}

void CParticleExplosion::Reset_Partice(Attribute* attribute)
{
	attribute->_bIsAlive = true;
	attribute->_vPosition = m_vOrigin;

	GetRandomVector(
		&attribute->_vVelocity,
		&m_vMinVelocity,
		&m_vMinVelocity);

	// normalize to make spherical
	D3DXVec3Normalize(
		&attribute->_vVelocity,
		&attribute->_vVelocity);

	attribute->_vVelocity *= 2.f;

	attribute->_color = D3DXCOLOR(
		1.f,
		1.f,
		1.f,
		1.0f);

	//attribute->_color = D3DXCOLOR(
	//	GetRandomFloat(0.0f, 1.0f),
	//	GetRandomFloat(0.0f, 1.0f),
	//	GetRandomFloat(0.0f, 1.0f),
	//	1.0f);

	attribute->_fAge = 0.0f;
	attribute->_fLifeTime = 1.0f; // lives for 2 seconds
}

void CParticleExplosion::Update_Particle(_float fTimeDelat)
{
	list<Attribute>::iterator i;

	for (i = m_ParticlesList.begin(); i != m_ParticlesList.end(); i++)
	{
		// only update living particles
		if (i->_bIsAlive)
		{
			i->_vPosition += i->_vVelocity * fTimeDelat;

			i->_fAge += fTimeDelat;

			if (i->_fAge > i->_fLifeTime) // kill 
				i->_bIsAlive = false;
		}
	}
}