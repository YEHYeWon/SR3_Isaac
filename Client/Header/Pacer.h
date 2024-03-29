#pragma once

#include "Monster.h"
#include "GameObject.h"

class CPacer : public CMonster
{
	const _float HEIGHT_Y = 0.4f;
	const _float INTERVAL = 2.f;
private:
	explicit CPacer(LPDIRECT3DDEVICE9 pGraphicDev, int iID);
	explicit CPacer(const CPacer& rhs);
	virtual ~CPacer();

	enum PACERSTATE { PACER_IDLE, PACER_END };

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject()					 override;
	virtual void	Render_GameObject()						 override;

private:
	virtual HRESULT		Add_Component()						override;
	virtual void		Motion_Change()						override;
	virtual void		Face_Camera()						override;

	void				Change_Dir();
	void				Move(const _float& fTimeDelta);
	void				Epic_Time();

private:
	_bool				m_bEpicTime;
	_int				m_iRandNum;
	_int				m_iPicNum;
	_float				m_fFrameSpeed = 1.f;

	_vec3				m_vOriginAngle;

public:
	static CPacer* Create(LPDIRECT3DDEVICE9 pGraphicDev, int iID);

private:
	virtual void	Free()									override;
};

