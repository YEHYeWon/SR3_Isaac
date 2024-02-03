#pragma once

#include "MapObj.h"
#include "GameObject.h"

class CShellNpc : public CMapObj
{
private:
	explicit CShellNpc(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShellNpc(const CShellNpc& rhs);
	virtual ~CShellNpc();

	enum NPCSTATE { NPC_IDLE, NPC_GAMING, NPC_END };

public:
	void			Set_Game_True() { m_eCurState = NPC_GAMING; }
	void			Set_Game_False() { m_eCurState = NPC_IDLE; }

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject()					 override;
	virtual void	Render_GameObject()						 override;

private:
	virtual HRESULT		Add_Component()						override;
	virtual void		Motion_Change()						override;

private:

	NPCSTATE				m_ePreState;
	NPCSTATE				m_eCurState;

public:
	static CShellNpc* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free()								override;
};
