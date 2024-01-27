#pragma once

#include "Camera.h"

#include "Export_Utility.h"

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

	// PLAYERCHASE : 플레이어 따라다님
	// WHOLELAND : 전체 맵 보여줌
	enum DYNAMICCAMERAID
	{
		C_PLAYERCHASE,
		C_WHOLELAND,
		C_END
	};

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;

private:
	void		Key_Input(const _float& fTimeDelta);

	void		Chase_Character();
	void		Whole_Land_Show();
	void		Mouse_Fix();
	void		Mouse_Move();

private:
	_bool		m_bFix = false;
	_bool		m_bCheck = false;
	_bool		m_bChaseInit = true;

	// X,Y,Z의 회전각
	float		m_fAngleX;
	float		m_fAngleY;
	float		m_fAngleZ;

	// 회전스피드
	float		m_fAngleSpeed;
	// 이동 스피드
	float		m_fMoveSpeed;

	// 플레이어와 카메라의 사이 거리와 방향
	// 플레이어에서 해당방향 더해주기
	_vec3		m_vCameraPosDir;
	// 눈과 바라보는 지점의 고정거리
	// player를 따라다닐때 초기화할때의 값
	float		m_fCameraDistance;
	// y로의 길이 (플레이어 추적할때 초기화에 사용)
	float		m_fCameraHeight;

	CTransform* m_pTarget;

	// 카메라 사용용도
	DYNAMICCAMERAID		m_eCurState;

	//필요없을시 삭제
	DYNAMICCAMERAID		m_ePreState;

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);


	void					Set_Target(CTransform* _target)
	{
		m_pTarget = _target;
	}

private:
	virtual void Free();
};

