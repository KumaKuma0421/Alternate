// ----------------------------------------------------------------------------
//! @file	QueryPerformance.h
//! @brief	������\���Ԍv���Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.06

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	QueryPerformance
	//! @brief	������\���Ԍv���N���X
	class DLL_DECLSPEC QueryPerformance : public WindowsLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY QueryPerformance ();

		//! @brief �f�X�g���N�^
		APIENTRY ~QueryPerformance () {};

		//! @brief �v�����J�n���܂��B
		VOID APIENTRY Start ();

		//! @brief �v�����I�����܂��B
		VOID APIENTRY Stop ();

		//! @brief	�v���J�n�`�v���I���܂ł̎��Ԃ��o�͂��܂��B
		//! @note	�P�ʂ̓~���b�ŏo�͂���܂��B
		//! @note	�L���l�͏����_��S�ʂ܂�
		double APIENTRY PastTime () const;

		//! @brief Start()���̃J�E���^�[�l���擾���܂��B
		LARGE_INTEGER APIENTRY GetStartTick () const { return _startTick; };

		//! @brief Stop()���̃J�E���^�[�l���擾���܂��B
		LARGE_INTEGER APIENTRY GetStopTick () const { return _stopTick; };

		//! @brief ���݂̃^�C�}�[����\���擾���܂��B
		LARGE_INTEGER APIENTRY GetFrequency () const { return _frequency; };

	private:
		//! @brief �J�n���^�C�}�[�l
		LARGE_INTEGER _startTick;

		//! @brief �I�����^�C�}�[�l
		LARGE_INTEGER _stopTick;

		//! @brief ���݂̃^�C�}�[����\
		LARGE_INTEGER _frequency;
	};
}