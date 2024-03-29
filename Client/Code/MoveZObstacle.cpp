#include "stdafx.h"
#include "..\Header\MoveZObstacle.h"

#include "Export_Utility.h"

CMoveZObstacle::CMoveZObstacle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMapObj(pGraphicDev),
	m_fDistanceUp(0.f), m_fDistanceDown(0.f),
	m_bTurn(false)
{
}

CMoveZObstacle::CMoveZObstacle(const CMoveZObstacle& rhs)
	: CMapObj(rhs)
{

}

CMoveZObstacle::~CMoveZObstacle()
{
}

HRESULT CMoveZObstacle::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 1.f, 1.f, 1.f };
	m_bTurn = false;

	m_fSpeed = 3.f;
	m_fDropSpeed = 60.f;

	m_vPos = m_pTransformCom->m_vInfo[INFO_POS];

	m_eObjType = OBSTACLE_Z;

	return S_OK;
}

Engine::_int CMoveZObstacle::Update_GameObject(const _float& fTimeDelta)
{
	__super::Compute_ViewZ(&m_pTransformCom->m_vInfo[INFO_POS]);
	
	CGameObject::Update_GameObject(fTimeDelta);

	_vec3 vDir = _vec3(0.f, -1.f, 0.f);

	if (!m_bArrived && 1 < m_pTransformCom->m_vInfo[INFO_POS].y)
	{
		m_pTransformCom->Move_Pos(&vDir, m_fDropSpeed, fTimeDelta);
	}
	else if (!m_bArrived)
	{
		m_bArrived = true;
		m_pTransformCom->m_vInfo[INFO_POS].y = 1;

		_matrix	mat = *(m_pTransformCom->Get_WorldMatrix());

		mat._42 -= 1.f;

		_float fRandNumX = (rand() % 5 + 1) * 0.1f;
		_float fRandNumZ = (rand() % 5 + 1) * 0.1f;
		mat._41 += fRandNumX;
		mat._43 += fRandNumZ;

		CParticleMgr::GetInstance()->Create_Explosion(m_pGraphicDev, mat, 1.f, 30, 4.f);
	}

	if (m_bArrived)
	{
		if (0.5f < m_fDorpTimer)
		{
			Move(fTimeDelta);
			Check_Wall_Collision();
		}
		else
		{
			m_fDorpTimer += fTimeDelta;
		}
	}

	return 0;
}

void CMoveZObstacle::LateUpdate_GameObject()
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	__super::LateUpdate_GameObject();
}

void CMoveZObstacle::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CMoveZObstacle::Set_Cute_Texture(const _tchar* pTextureTag)
{
	return E_NOTIMPL;
}

void CMoveZObstacle::Set_Pos(_vec3 vPos)
{
	m_vPos = vPos;
}

HRESULT CMoveZObstacle::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransformCom->m_vInfo[INFO_POS] = { 10, 50, 10 };

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ObstacleTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ObstacleTexture", pComponent });

	return S_OK;
}

void CMoveZObstacle::Move(const _float& fTimeDelta)
{
	_float fUpDst = m_vPos.x - m_fDistanceUp;
	_float fDownDst = m_vPos.x + m_fDistanceDown;

	//�����̸� �������� �̵�
	if (!m_bTurn)
	{
		m_pTransformCom->Set_Pos_Z(-m_fSpeed * fTimeDelta);

		if (m_pTransformCom->m_vInfo[INFO_POS].z <= fUpDst)
			m_bTurn = true;
	}
	else
	{
		m_pTransformCom->Set_Pos_Z(m_fSpeed * fTimeDelta);

		if (m_pTransformCom->m_vInfo[INFO_POS].z >= fDownDst)
			m_bTurn = false;
	}
}

void CMoveZObstacle::Check_Wall_Collision()
{
	_vec3 vPos, vScale;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Scale(&vScale);

	if (vPos.z - vScale.z * 2.f <= 0 && !m_bTurn)
		m_bTurn = true;

	if (vPos.z + vScale.z * 2.f >= VTXCNTZ && m_bTurn)
		m_bTurn = false;

}

CMoveZObstacle * CMoveZObstacle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMoveZObstacle *	pInstance = new CMoveZObstacle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("MoveZObstacle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMoveZObstacle::Free()
{
	__super::Free();
}


