// ----------------------------------------------------------------------------
//! @file	Exception.h
//! @brief	��O�Ɋւ���N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

#define ALT_REASON_EXCEPTION        _T("��O���������܂����B")
#define ALT_REASON_INVALID_ARGUMENT _T("�����̐ݒ肪�ُ�ł��B")
#define ALT_REASON_RUNTIME_ERROR	_T("���s���ɃG���[���������܂����B")

namespace alt
{
	namespace ex
	{
		//! @class	Exception
		//! @brief	���ƂȂ��O�N���X
		class DLL_DECLSPEC Exception : public WindowsLibrary
		{
		public:
			//! @brief �R���X�g���N�^
			APIENTRY Exception ();

			//! @brief �f�X�g���N�^
			APIENTRY ~Exception ();

			//! @brief �������擾
			//! @return ��������
			LPCTSTR APIENTRY GetReason () const;
		};

		//! @class	InvalidArgmentException
		//! @brief	�����ݒ�G���[��O
		class DLL_DECLSPEC InvalidArgumentException : Exception
		{
		public:
			//! @brief �R���X�g���N�^
			APIENTRY InvalidArgumentException ();

			//! @brief �f�X�g���N�^
			APIENTRY ~InvalidArgumentException ();

			//! @brief �������擾
			//! @return ��������
			LPCTSTR APIENTRY GetReason () const;
		};

		//! @class	RuntimeException
		//! @brief	���s���G���[��O
		class DLL_DECLSPEC RuntimeException : Exception
		{
		public:
			//! @brief �R���X�g���N�^
			APIENTRY RuntimeException ();

			//! @brief �f�X�g���N�^
			APIENTRY ~RuntimeException ();

			//! @brief �������擾
			//! @return ��������
			LPCTSTR APIENTRY GetReason () const;
		};
	}
}