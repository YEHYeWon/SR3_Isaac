#include "stdafx.h"
#include "..\Header\FightPlayerName.h"

#include "Export_Utility.h"

#include "Player.h"

CFightPlayerName::CFightPlayerName(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CUI(pGraphicDev)
{
}

CFightPlayerName::CFightPlayerName(const CFightPlayerName& rhs)
	: Engine::CUI(rhs)
{
}

CFightPlayerName::~CFightPlayerName()
{
}

HRESULT CFightPlayerName::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale.x = m_fSizeX;
	m_pTransformCom->m_vScale.y = m_fSizeY;

	m_vDestPos = _vec3(m_fPosX, m_fPosY, 0.98f);

	m_pTransformCom->Set_Pos(_vec3(-500.f, m_fPosY, 0.98f));

	__super::Ready_GameObject();

	return S_OK;
}

Engine::_int CFightPlayerName::Update_GameObject(const _float& fTimeDelta)
{
	CUI::Update_GameObject(fTimeDelta);

	if (!m_bBack)
	{
		_vec3 vDir = m_vDestPos - m_pTransformCom->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&vDir, &vDir);

		m_fMoveSpeed += m_fIncreaseSpeed;

		if (vDir.x > 0)
			m_pTransformCom->Move_Pos(&vDir, m_fMoveSpeed, fTimeDelta);
	}
	else
	{

		_vec3 vDir(-1.f, 0.f, 0.f);

		m_fMoveSpeed = 1300.f;

		m_pTransformCom->Move_Pos(&vDir, m_fMoveSpeed, fTimeDelta);
		m_pTransformCom->m_vScale.x += 800.f * fTimeDelta;
	}
	
	return 0;
}

void CFightPlayerName::LateUpdate_GameObject()
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	//CGameObject::LateUpdate_GameObject();
	__super::LateUpdate_GameObject();
}

void CFightPlayerName::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture((_int)0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CFightPlayerName::Add_Component()
{
	CComponent*		pComponent = nullptr;
		
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	if (m_bIsaac)
	{
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_AzazelNameTexture"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_AzazelNameTexture", pComponent });
	}
	else
	{
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_IsaacNameTexture"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_IsaacNameTexture", pComponent });
	}

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
		
	return S_OK;
}

CFightPlayerName * CFightPlayerName::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
	_float fSizeX, _float fSizeY, 
	_float fPosX, _float fPosY, 
	_float fWinCX, _float fWinCY)
{
	CFightPlayerName *	pInstance = new CFightPlayerName(pGraphicDev);

	pInstance->Set_WindowSize(fWinCX, fWinCY);
	pInstance->Set_Size(fSizeX, fSizeY);
	pInstance->Set_Pos(fPosX, fPosY, fWinCX, fWinCY);

	pInstance->m_bIsaac = CPlayer::GetInstance()->Get_AzazelMode();

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("EndingHW Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CFightPlayerName::Free()
{	
	__super::Free();
}
