//! @file	LinkedListMemoryLeakCheck.h
//! @brief	LinkedList�̃������[���[�N���m�F���܂��B
//! @author	kumakuma0421@gmail.com
//! @date	2022.06.28

#pragma once

#include "LeakCheck.h"
#include "LinkedList.hpp"

namespace LeakCheck
{
	//! @class	LinkedListMemoryLeakCheck
	//! @brief	LinkedList�̃������[���[�N�`�F�b�N�N���X
	class LinkedListMemoryLeakCheck : public LeakCheck
	{
	public:
		//! @brief �R���X�g���N�^
		LinkedListMemoryLeakCheck ();

		//! @brief �f�X�g���N�^
		virtual ~LinkedListMemoryLeakCheck ();

		//! @brief ����������
		virtual BOOL Init ();

		//! @brief �������[���[�N����
		virtual BOOL DoAction ();
	};
}