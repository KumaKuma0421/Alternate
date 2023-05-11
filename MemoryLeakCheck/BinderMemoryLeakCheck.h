//! @file	BinderMemoryLeakCheck.h
//! @brief	Binder�̃������[���[�N���m�F���܂��B
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#pragma once

#include "LeakCheck.h"
#include "Binder.hpp"
#include "Freight.hpp"

namespace LeakCheck
{
	//! @class	BinderMemoryLeakCheck
	//! @brief	Binder�̃������[���[�N�`�F�b�N�N���X
	class BinderMemoryLeakCheck : public LeakCheck
	{
	public:
		//! @brief �R���X�g���N�^
		BinderMemoryLeakCheck ();

		//! @brief �f�X�g���N�^
		virtual ~BinderMemoryLeakCheck ();

		//! @brief ����������
		virtual BOOL Init ();

		//! @brief �������[���[�N����
		virtual BOOL DoAction ();

	private:
		BOOL DoAction1 ();
		BOOL DoAction2 ();
		BOOL DoAction3 ();
		BOOL DoAction4 ();
	};
}