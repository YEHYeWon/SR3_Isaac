#pragma once

#include "Item.h"
#include "GameObject.h"

class CHeartHalf : public CItem
{
	explicit CHeartHalf(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHeartHalf(const CHeartHalf& rhs);
	virtual ~CHeartHalf();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject()					 override;
	virtual void	Render_GameObject()						 override;

public:
	bool			Run_Item_Effect()						override;
	void			Item_Spawn_Action()						override;

private:
	virtual HRESULT		Add_Component()						override;
	virtual void		Motion_Change()						override;


private:
	// 랜덤값으로 한 이미지 지정하면 계속 유지
	_float					m_fPicNum;

public:
	static CHeartHalf* Create(LPDIRECT3DDEVICE9 pGraphicDev, int spawnspot, _vec3 pos, _vec3 look);

private:
	virtual void		Free()									override;
};

