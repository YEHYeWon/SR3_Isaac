#pragma once

#include "Base.h"
#include "UI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CFightVs : public Engine::CUI
{
private:
	explicit CFightVs(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFightVs(const CFightVs& rhs);
	virtual ~CFightVs();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

	void	Set_Back() { m_bBack = true; }

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex*		m_pBufferCom;
	Engine::CTransform*	m_pTransformCom;
	Engine::CTexture*	m_pTextureCom;

	_float		m_fMoveTimer = 0.f;
	_float		m_fMoveSpeed = 100.f;
	_float		m_fIncreaseSpeed = 70.f;

	_bool		m_bBack = false;

public:
	static CFightVs*		Create(LPDIRECT3DDEVICE9	pGraphicDev, 
		_float fSizeX, _float fSizeY, 
		_float fPosX, _float fPosY, 
		_float fWinCX = WINCX, _float fWinCY = WINCY);

private:
	virtual void Free() override;
};

