#include "stdafx.h"
#include "..\Header\BossFight.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "LoadStage.h"
#include "BlackBackground.h"

CBossFight::CBossFight(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CBossFight::~CBossFight()
{
}

HRESULT CBossFight::Ready_Scene()
{

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);

	return S_OK;
}

Engine::_int CBossFight::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);

	m_fSceneChangeTimer += fTimeDelta;

	if (5.f < m_fSceneChangeTimer)
	{
		// �������� ����
		Engine::CScene* pScene = nullptr;

		// ���� ���������� ť�� ���� ���ʿ��Ͽ� true���� �Ѱ��ֱ�
		pScene = CLoadStage::Create(m_pGraphicDev, m_iStageKey, false);

		NULL_CHECK_RETURN(pScene, -1);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
	}

	return iExit;
}

void CBossFight::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CBossFight::Render_Scene()
{

}

HRESULT CBossFight::Ready_Prototype()
{

	return S_OK;
}

HRESULT CBossFight::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	//����̽�, xũ��, yũ��, x��ǥ, y��ǥ, x��ü ũ��, y��ü ũ�� (��üũ��� default ��������)
	pGameObject = CBlackBackground::Create(m_pGraphicDev, WINCX, WINCY, 0.f, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlackBackground", pGameObject), E_FAIL);

	//���������� ������������ �޾ƿͼ� �ؽ�ó�� �������ش�.
	//if()

	//pGameObject = pBack = CGameEnd::Create(m_pGraphicDev, 300.f, 300.f, 0.f, 0.f, 2, 2);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GameEnd", pGameObject), E_FAIL);

	//pGameObject = CStageToolFly::Create(m_pGraphicDev, 100.f, 100.f, 120.f, -100.f, 2, 4);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//m_pStageToolFly->Set_RenderIndex(2);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Proto_StageToolFly", pGameObject), E_FAIL);


	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CBossFight * CBossFight::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iStageKey)
{
	CBossFight *	pInstance = new CBossFight(pGraphicDev);
	pInstance->m_iStageKey = iStageKey;

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Ending Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CBossFight::Free()
{
	__super::Free();
}