//! @file	DBLibrary.h
//! @brief	DB API���W�񂷂�N���X�̊��N���X
//! @note	DllMain()�̃v���g�^�C�v�������Ő錾���Ă��܂��B
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.4

#pragma once

BOOL APIENTRY DllMain (HMODULE hModule, DWORD dwReasonForCall, LPVOID lpvReserved);

namespace alt
{
	//! @class	DBLibrary
	//! @brief	DB API���W�񂷂�N���X�̊��N���X
	class DLL_DECLSPEC DBLibrary
	{
	public:
		DBLibrary ();
		virtual ~DBLibrary ();
	};
}