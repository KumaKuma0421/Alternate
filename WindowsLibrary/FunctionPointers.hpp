// ----------------------------------------------------------------------------
//! @file	FunctionPointers.hpp
//! @brief	�R�[���o�b�N�@�\���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	namespace skeleton
	{
		//! @brief CreateThread()�ɓn���X���b�h���s�֐��̃e���v���[�g
		typedef DWORD (*Invoker)(LPVOID me);

		//! @class	ICallback
		//! @brief	Thread�N���X�̃X���b�h���s�֐���ݒ肵�܂��B
		//! @note	Invoker()�͒����Invoke()���Ăяo���܂��B
		//! 		Invoke()�͏������z�֐��Ȃ̂ŁA�X���b�h���s���������@�\�́A
		//!			�p����Ŏ������Ă��������B
		class DLL_DECLSPEC ICallback : public WindowsLibrary
		{
		public:
			//! @brief �R�[���o�b�N�֐��v���[�X�z���_
			//! @param lpvParam �R�[���o�b�N�֐��ɓn�����p�����[�^
			//! @return DWORD
			//! @note ���[�U�[���ŃR�[���o�b�N�֐��̎��̂��L�ڂ��܂��B
			virtual DWORD APIENTRY Invoke (LPVOID lpvParam) = 0;

			//! @brief �R�[���o�b�N�֐��Ăяo���p�C���^�[�t�F�[�X
			//! @param me �Ăяo�����̃A�h���X
			//! @return Invoke()�̖߂�l
			static DWORD APIENTRY Invoker (LPVOID me)
			{
				ICallback* function = (ICallback*)me;
				return function->Invoke (me);
			};
		};

		//! @class	ITimeup
		//! @brief	WatchDogTimer�N���X�̃R�[���o�b�N�֐���ݒ肵�܂��B
		//! @note	TimeupCall()�͒����Timeup()���Ăяo���܂��B
		//! 		Timeup()�͏������z�֐��Ȃ̂ŁA�R�[���o�b�N��Ɏ��s����@�\�́A
		//! 		�p����Ŏ������Ă��������B
		class DLL_DECLSPEC ITimeup : public WindowsLibrary
		{
		public:
			//! @brief �^�C���A�b�v�֐��v���[�X�z���_
			//! @param me �Ăяo�����̃A�h���X
			//! @param dwHiTime �^�C�}�[�l�i��ʁj
			//! @param dwLoTime �^�C�}�[�l�i���ʁj
			virtual VOID APIENTRY Timeup (
				LPVOID me, DWORD dwHiTime, DWORD dwLoTime) = 0;

			//! @brief �^�C���A�b�v�֐��̌Ăяo���p�C���^�[�t�F�[�X
			//! @param me �Ăяo�����̃A�h���X
			//! @param dwHiTime �^�C�}�[�l�i��ʁj
			//! @param dwLoTime �^�C�}�[�l�i���ʁj
			//! @return VOID
			static VOID APIENTRY TimeupCall (
				LPVOID me, DWORD dwHiTime, DWORD dwLoTime)
			{
				ITimeup* function = (ITimeup*)me;
				function->Timeup (me, dwHiTime, dwLoTime);
			}
		};
	}
}