#include "LacrossePlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"

//*************************************************************
//
//		ラクロスの操作クラス
//
//*************************************************************


//***************************************************
//		プレイヤー操作の移動クラス
//     LacrosseState_PlayerControllMove
//***************************************************

// 移動時のイベントクラス作成
CharacterUsualMove::MoveEvent* LacrosseState_PlayerControllMove::CreateMoveEvent(LacrossePlayer* t)
{
	class LacrosseMoveEvent : public CharacterUsualMove::MoveEvent
	{
		LacrossePlayer* m_pLacrosse;
	public:
		LacrosseMoveEvent(LacrossePlayer* pLacrosse) :
			m_pLacrosse(pLacrosse){}

		// 更新
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pLacrosse->m_Renderer.Update(1);
		}

		// 走り出した時
		void RunStart()
		{
			m_pLacrosse->m_Renderer.SetMotion(lacrosse_player::mt_Run);
		}

		// 止まった時
		void StandStart()
		{
			m_pLacrosse->m_Renderer.SetMotion(lacrosse_player::mt_Stand);
		}
	};

	return new LacrosseMoveEvent(t);
}


// ステート開始
void LacrosseState_PlayerControllMove::Enter(LacrossePlayer* t)
{
	// 移動パラメータ設定
	CharacterUsualMove::Params p;
	p.Acceleration = 0.2f;
	p.MaxSpeed = 0.23f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.2f;

	// 移動クラス作成
	m_pMoveClass = new CharacterUsualMove(t, p, this->CreateMoveEvent(t));

}


// ステート実行
void LacrosseState_PlayerControllMove::Execute(LacrossePlayer* t)
{
	Vector2 st = controller::GetStickValue(controller::stick::left, t->m_PlayerInfo.number);

	m_pMoveClass->SetStickValue(st);
	m_pMoveClass->Update();

	chr_func::CreateTransMatrix(t, 0.05f, &t->m_Renderer.m_TransMatrix);

	if (controller::GetTRG(controller::button::shikaku, t->m_PlayerInfo.number))
	{
		t->SetState(new LacrosseState_PlayerControllAttackClose());
		return;
	}
}


// ステート終了
void LacrosseState_PlayerControllMove::Exit(LacrossePlayer* t)
{
	delete m_pMoveClass;
}



//***************************************************
//		プレイヤー操作の近距離攻撃クラス
//***************************************************


// ステート開始
void LacrosseState_PlayerControllAttackClose::Enter(LacrossePlayer* t)
{
	m_pAttackClass = CreateAttackClass(t);
}


// ステート実行
void LacrosseState_PlayerControllAttackClose::Execute(LacrossePlayer* t)
{
	// スティックの値セット
	m_pAttackClass->SetStickValue(
		controller::GetStickValue(controller::stick::left, t->m_PlayerInfo.number));
	
	// 更新
	m_pAttackClass->Update();

	// 転送行列更新
	chr_func::CreateTransMatrix(t, 0.05f, &t->m_Renderer.m_TransMatrix);
}


// ステート終了
void LacrosseState_PlayerControllAttackClose::Exit(LacrossePlayer* t)
{
	delete m_pAttackClass;
}


// 近接攻撃クラス作成
LacrosseAttackClose* LacrosseState_PlayerControllAttackClose::CreateAttackClass(LacrossePlayer* t)
{
	class AttackCloseEvent : public LacrosseAttackClose::Event
	{
		LacrossePlayer* m_pLacrosse;
	public:
		// コンストラクタ
		AttackCloseEvent(LacrossePlayer* t) :
			m_pLacrosse(t){}

		// 更新
		void Update()override
		{
			m_pLacrosse->m_Renderer.Update(1.0f);
		}

		// 攻撃開始
		void AttackStart()override
		{
			m_pLacrosse->m_Renderer.SetMotion(lacrosse_player::mt_AttackClose_1);
		}

		// 攻撃終了
		void AttackEnd()override
		{
			// 通常移動へ
			m_pLacrosse->SetState(new LacrosseState_PlayerControllMove());
		}

	};
	return new LacrosseAttackClose(t, new AttackCloseEvent(t));
}

