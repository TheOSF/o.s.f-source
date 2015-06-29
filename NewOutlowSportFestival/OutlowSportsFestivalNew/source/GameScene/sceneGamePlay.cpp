#include	"iextreme.h"
#include	"../IexSystem/System.h"
#include	"debug\DebugFunction.h"
#include	"sceneGamePlay.h"
#include	"../Ball/Ball.h"
#include	"../Camera/Camera.h"
#include	"../character/CharacterManager.h"

#include	"../character/Tennis/TennisPlayer.h"
#include	"../character/Tennis/TennisPlayerState.h"
#include	"../character/Soccer/SoccerPlayer.h"
#include	"../character/Soccer/SoccerPlayerState.h"
#include	"../character/Baseball/BaseballPlayer.h"
#include	"../character/Baseball/BaseballPlayerState.h"

// Effekseer
#include "../Library/Effekseer/EffekseerSystem.h"
#include "../Library/Effekseer/EffekseerEffectManager.h"
#include "../Library/Effekseer/EffekseerEffect.h"

// Bullet
#include "../Library/Bullet/BulletSystem.h"

//*****************************************************************************************************************************
//
//	グローバル変数
//
//*****************************************************************************************************************************
static LPIEXMESH pStage;
static LPIEXMESH pTennisBall;

static EffekseerSystem* pEffekseerSystem;
static EffekseerEffectManager* pEffekseerEffectManager;
static EffekseerEffect* pEffekseerEffect;


//*****************************************************************************************************************************
//
//	初期化
//
//*****************************************************************************************************************************


bool sceneGamePlay::Initialize()
{
	//	環境設定
	iexLight::SetAmbient(0x404040);
	iexLight::SetFog(800, 1000, 0);

	Vector3 dir(1.0f, -1.0f, -0.5f);
	dir.Normalize();
	iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);

	DefCamera.m_Position = Vector3(0, 40, -55);
	DefCamera.m_Target = Vector3(0, 4, 0);

	pStage = new iexMesh("DATA\\STAGE\\Stage.IMO");

	//キャラクタ作成
	if(0){
		CharacterBase::PlayerInfo pl;

		pl.chr_type = CharacterType::_Baseball;
		pl.number = (PlayerNum::Value)0;
		pl.player_type = PlayerType::_Player;
		pl.strong_type = StrongType::__ErrorType;

		BaseballPlayer* bp = new BaseballPlayer(pl);
		bp->SetState(new BaseballState_PlayerControll_Move());
	}


	{
		CharacterBase::PlayerInfo pl;

		pl.chr_type = CharacterType::_Tennis;
		pl.number = (PlayerNum::Value)0;
		pl.player_type = PlayerType::_Player;
		pl.strong_type = StrongType::__ErrorType;

		TennisPlayer* tp = new TennisPlayer(pl);
		tp->SetState(new TennisState_PlayerControll_Move());
	}
	{
		CharacterBase::PlayerInfo pl;

		pl.chr_type = CharacterType::_Tennis;
		pl.number = (PlayerNum::Value)1;
		pl.player_type = PlayerType::_Player;
		pl.strong_type = StrongType::__ErrorType;

		TennisPlayer* tp = new TennisPlayer(pl);
		tp->SetState(new TennisState_PlayerControll_Move());
		tp->m_Params.pos.x += 40;
	}

	{
		// Effekseer
		pEffekseerSystem = new EffekseerSystem(iexSystem::Device);
		pEffekseerEffectManager = pEffekseerSystem->CreateManager();

		pEffekseerEffectManager->LoadEffect(0, (const EFK_CHAR*)L"DATA//Effekseer//Sample//test.efk");


		// エフェクト再生
		pEffekseerEffect = pEffekseerEffectManager->PlayEffect(0);
	};

	{// Bullet

		pTennisBall = new IEXMESH("DATA//CHR//Tennis_ball//Tennis_ball.IMO");
		pTennisBall->SetPos(0, 20, 10);
		pTennisBall->SetAngle(0, 0, 0);
		pTennisBall->SetScale(0.03f, 0.03f, 0.03f);

		DefBulletSystem.StartUp();
		DefBulletSystem.InitializeBulletPhysics(btVector3(0, -9.8f, 0), iexSystem::Device);

		// テスト用

		// Box
		DefBulletSystem.AddRigidBox(
			1.0f, 
			RigidBody::ct_dynamic, 
			Vector3(0, 30, 0), 
			Vector3(0.2f, 0, 0), 
			Vector3(1, 1, 1),
			0.2f, 
			1.0f, 
			Vector3(0, 0, 0)
			);

		// Sphere
		DefBulletSystem.AddRigidSphere(
			1.0f,
			RigidBody::ct_dynamic,
			Vector3(10, 30, 0),
			Vector3(0, 0, 0),
			1.0f,
			0.2f,
			1.0f,
			Vector3(0, -10, 0)
			);

		// Mesh
		DefBulletSystem.AddRigidMesh(
			pTennisBall,
			1.0f,
			RigidBody::ct_dynamic,
			0.2f,
			1.0f,
			Vector3(0,0,0)
			);


		// 床
		DefBulletSystem.AddRigidBox(
			0.0f,
			RigidBody::ct_static,
			Vector3(0, -10, 0),
			Vector3(0, 0, 0),
			Vector3(100, 10, 100),
			0.2f,
			0.5f,
			Vector3(0, 0, 0)
			);
	};

	return true;
}

//*****************************************************************************************************************************
//
//		解放
//
//*****************************************************************************************************************************

sceneGamePlay::~sceneGamePlay()
{
	{// Effekseer
		delete pEffekseerEffect;
		delete pEffekseerEffectManager;
		delete pEffekseerSystem;
	};

	{// Bullet
		delete pTennisBall;
		DefBulletSystem.ReleaseBulletPhysics();
		DefBulletSystem.ShutDown(true);
	};

	delete pStage;
	DefGameObjMgr.Release();
	DefCharacterMgr.Release();
	DefRendererMgr.Release();
	DefDamageMgr.Release();
	DefBallMgr.Release();
	DefCamera.Release();
}

//*****************************************************************************************************************************
//
//		更新
//
//*****************************************************************************************************************************
void	sceneGamePlay::Update()
{
	{// Effekseer

		// ビュー行列設定
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				pEffekseerSystem->m_ViewMatrix.Values[j][i] = matView.m[j][i];
			}
		}

		// 射影変換行列設定
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				pEffekseerSystem->m_ProjectionMatrix.Values[j][i] = matProjection.m[j][i];
			}
		}

		// レンダラーの更新
		pEffekseerSystem->UpdateParams();

		// エフェクト更新
		if (pEffekseerEffect->IsDead())
		{
			delete pEffekseerEffect;
			pEffekseerEffect = pEffekseerEffectManager->PlayEffect(0);
		}
		else{
			pEffekseerEffect->m_Params.Angle.Y += 0.02f;
			pEffekseerEffect->Update();
		}

		// マネージャーの更新
		pEffekseerEffectManager->UpdateAllInstances(1.0f);
	};

	{// Bullet Physics


		if (KEY(KEY_SPACE, 0) == 3)
		{
			// Sphere
			DefBulletSystem.AddRigidSphere(
				1.0f,
				RigidBody::ct_dynamic,
				Vector3(10, 30, 0),
				Vector3(0, 0, 0),
				1.0f,
				0.0f,
				1.0f,
				Vector3(0, -10, 0)
				);
		}

		DefBulletSystem.StepSimulation(1.0f / 60.0f);
	};

	DefCamera.Update();

	DefGameObjMgr.Update();
}

//*****************************************************************************************************************************
//
//		描画関連
//
//*****************************************************************************************************************************

void	sceneGamePlay::Render()
{
	DefCamera.Clear();

	pStage->Render();

	

	DefRendererMgr.DeferredRender();
	DefRendererMgr.ForwardRender();

	{// Effekseer
		pEffekseerSystem->BeginRendering();
		pEffekseerEffectManager->RenderAllInstances();
		pEffekseerSystem->EndRendering();
	};

	

	//{// Bullet Physics
	//	DefBulletSystem.DebugDrawWorld();
	//};
}
