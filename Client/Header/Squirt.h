#pragma once

#include "Monster.h"
#include "GameObject.h"

#include "Layer.h"

class CSquirt : public CMonster
{
	const _float HEIGHT_Y = 1.2f;
	const _float INTERVAL = 1.f;
private:
	explicit CSquirt(LPDIRECT3DDEVICE9 pGraphicDev, int iID);
	explicit CSquirt(const CSquirt& rhs);
	virtual ~CSquirt();

	enum SQUIRTSTATE { SQU_IDLE, SQU_SLIDE, SQU_END };

public:
	void				Create_Dip(CLayer* pLayer);

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject()					 override;
	virtual void	Render_GameObject()						 override;

private:
	virtual HRESULT		Add_Component()						override;
	virtual void		Motion_Change()						override;
	virtual void		Face_Camera()						override;

	void				Sliding(const _float& fTimeDelta);
	void				Check_TargetPos();
	void				Epic_Time();

private:
	_bool				m_bSliding;
	_bool				m_bBounceWall;
	_bool				m_bEpicTime;
	_int				m_iRandNum;
	_int				m_iPicNum;
	_float				m_fFrameSpeed = 1.f;
	_float				m_fAccel;

	_vec3				m_vTargetPos;
	_vec3				m_vDir; // 쓸 필요 없을 듯함
	_vec3				m_vBounceDir;
	_vec3				m_vOriginAngle;

	SQUIRTSTATE			m_ePreState;
	SQUIRTSTATE			m_eCurState;

public:
	static CSquirt* Create(LPDIRECT3DDEVICE9 pGraphicDev, int iID);

private:
	virtual void	Free()									override;
};

