#ifndef __BALL_H__
#define __BALL_H__

#include <map>
#include <list>
#include "iextreme.h"

//*****************************************************
//		�ʃx�[�X�N���X
//*****************************************************

class BallBase
{
public:
	//���̋@�\�̃^�C�v
	enum Type
	{
		_DontWork,				//�`��͂��Ă��邪�A�����蔻��̂Ȃ��{�[��
		_Usual,					//�ʏ�̃{�[��
		_CantCounter,			//�J�E���^�[�ł��Ȃ��ʏ�̃{�[��
		_CanVolleySmash,		//�o���[�L�����N�^�ŃX�}�b�V�����邱�Ƃ��ł����Ԃ̃{�[��
		_TennisSpecialAtk,		//�e�j�X
	};

	//�p�����[�^�\����
	struct Params
	{
		Type		type;
		Vector3		pos;
		Vector3		move;
	};

	Params	m_Params;

	//�R���X�g���N�^�E�f�X�g���N�^�Ŏ����I�Ƀ}�l�[�W���ɓo�^�E�폜���s��
	BallBase();
	virtual ~BallBase();
};


//*****************************************************
//	�{�[���}�l�[�W��(�V���O���g������)
//*****************************************************
class BallManager
{
public:
	//�o�^�E�폜��BallBase�̂݉\�ɂ��邽��
	friend class BallBase;

	//�C���X�^���X�̃Q�b�^�E���
	static BallManager& GetInstance();
	static void Release();

private:
	typedef std::map<BallBase*, BallBase*> BallMap;

	static BallManager*		m_pInstance;
	BallMap					m_BallMap;

	BallManager();
	~BallManager();

	bool AddBall(BallBase* pBall);
	bool EraceBall(BallBase* pBall);
};

#define DefBallMgr (BallManager::GetInstance())

#endif