// ----------------------------------------------------------------------------
//! @file	CriticalSection.h
//! @brief	CriticalSection�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	CriticalSection
	//! @brief	CriticalSection�Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC CriticalSection : public WindowsLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY CriticalSection ();

		//! @brief �f�X�g���N�^
		APIENTRY ~CriticalSection ();

		//! @brief �r�������J�n
		VOID APIENTRY Enter ();

		//! @brief �r�������I��
		VOID APIENTRY Leave ();

	private:
		CRITICAL_SECTION _criticalSection;
	};
}