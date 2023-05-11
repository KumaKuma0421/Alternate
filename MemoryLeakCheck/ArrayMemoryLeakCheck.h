//! @file	ArrayMemoryLeakCheck.h
//! @brief	Array�̃������[���[�N���m�F���܂��B
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.01

#pragma once

#include "LeakCheck.h"
#include "Array.hpp"

namespace LeakCheck
{
	//! @class	ArrayMemoryLeakCheck
	//! @brief	Array�̃������[���[�N�`�F�b�N�N���X
	class ArrayMemoryLeakCheck : public LeakCheck
	{
	public:
		//! @brief �R���X�g���N�^
		ArrayMemoryLeakCheck ();

		//! @brief �f�X�g���N�^
		virtual ~ArrayMemoryLeakCheck ();

		//! @brief ����������
		virtual BOOL Init ();

		//! @brief �������[���[�N����
		virtual BOOL DoAction ();
	};
}