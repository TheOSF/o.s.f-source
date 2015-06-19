#pragma once

#include "iextreme.h"
#include <vector>

class CharacterBase;
//***************************************************
//		�L�����N�^�[���� �J�E���^�[�\���N���X
//***************************************************
class CharacterCounterPose
{
public:
	// �J�E���^�[�\���p�����[�^
	struct CounterPoseParams
	{
		int MaxPoseFrame;       // �ő�^�������t���[��
		int LevelUpFrame;        // ���x���A�b�v����t���[��
		float MoveDownSpeed; // ��������
	};

	// �J�E���^�[�\���C�x���g
	class Event
	{
	public:
		virtual~Event(){}
		virtual void Update() = 0;              // �X�V
		virtual void PoseStart() = 0;          // �\���J�n
		virtual void PoseEnd() = 0;            // �\���I��
		virtual void LevelUp(int level) = 0; // ���x���A�b�v
	};

	// �R���X�g���N�^
	CharacterCounterPose(
		const int                               maxLevel,                  // �ő僌�x��
		const CounterPoseParams& counterPoseParams,  // �\���p�����[�^
		CharacterBase*                    pCharacter,                // �����������L�����N�^�[
		Event*                                  pEvent                       // �\���C�x���g
		);

	// �f�X�g���N�^
	~CharacterCounterPose();

	// �X�V
	bool Update();

	// level ���ő僌�x�����ǂ���
	bool IsMaxLevel(int level)const;
	bool IsMaxLevel()const;

	// ���x���擾
	inline int GetLevel()const{ return m_NowLevel; }

private:
	const int                              m_MaxLevel;                  // ���x���A�b�v�p�����[�^
	int                                       m_NowLevel;                  // ���݂̃��x��
	int                                       m_Timer;                        // �^�C�}�[
	int                                       m_TotalFrame;                // �S�t���[��
	const CounterPoseParams  m_CounterPoseParams; // �J�E���^�[�\���p�����[�^
	CharacterBase*                  m_pCharacter;                // �L�����N�^�[
	Event*                                m_pEvent;                      // �J�E���^�[�\���C�x���g
};
