#pragma once

#include "MapObj.h"
#include "GameObject.h"

#include "ShellNpc.h"
#include "Shell.h"

#include "Layer.h"

class CShellGame : public CMapObj
{
private:
	const float SCALAR_X = -0.9f;
	const float SCALAR_Y = 0.1f;
	const float INTERVALX = 1.5f;

private:
	explicit CShellGame(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShellGame(const CShellGame& rhs);
	virtual ~CShellGame();

public:
	void				Set_Game(_bool bIsGame){ m_bGame = bIsGame; };
	_bool				Get_Game() { return m_bGame; }
	_bool				Get_Game_Reward() { return m_bReward; } // 보상 받을 수 있는지
	void				Set_Game_Reward(_bool bIsReward) { m_bReward = bIsReward; }
	void				Set_CheckCoolTime() { m_bCheckCoolTime = true; }
	_bool				Get_CheckCoolTime() { return m_bCheckCoolTime; }
	vector<CShell*>		Get_ShellVec() { return m_vecShell; }
	CShellNpc*			Get_ShellNpc() { return m_pShellNpc; }
	void				Set_ShellObj_ToStage(CLayer* pLayer);

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject()					 override;
	virtual void	Render_GameObject()						 override;

private:
	virtual HRESULT		Add_Component()						override;

	void				Create_NPC();
	void				Create_Shell();

	void				Game(const _float& fTimeDelta);
	void				Shaking_Shell(const _float& fTimeDelta);
	void				Resetting_ShellPos();
	void				Setting_RewardShell();

private:
	_bool				m_bGame;
	_bool				m_bShellShaking;
	_bool				m_bReward;
	_bool				m_bCheckCoolTime;
	_int				m_iShake_Lev;
	_float				m_fSpeed;

	CShellNpc*			m_pShellNpc;
	vector<CShell*>		m_vecShell;

	_int				m_iSoundTimer = 0;

public:
	static CShellGame* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free()								override;

};