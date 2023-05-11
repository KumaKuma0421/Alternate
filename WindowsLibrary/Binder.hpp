//! @file	Binder.hpp
//! @brief	new�I�u�W�F�N�g���Ǘ�����R���e�i�N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#pragma once

#include "Exception.h"

namespace alt
{
	namespace skeleton
	{
		//! @class	Binder
		//! @brief	new�I�u�W�F�N�g���Ǘ�����R���e�i�N���X
		template <class T, class... Args>
		class Binder
		{
		public:
			//! @brief �R���X�g���N�^
			Binder<T, Args...> ()
			{
				_value = nullptr;
			};

			//! @brief �R���X�g���N�^
			//! @param t �Ǘ��I�u�W�F�N�g�|�C���^
			Binder<T, Args...> (T* t)
			{
				_value = t;
			}

			//! @brief �f�X�g���N�^
			virtual APIENTRY ~Binder ()
			{
				Delete ();
			};

			//! @brief �Ǘ��I�u�W�F�N�g�̍쐬
			//! @param ...args �R���X�g���N�^����
			//! @return bool
			//! @retval true(����)
			//! @retval false(���s)
			bool APIENTRY New (Args... args)
			{
				_value = new T (args...);
				return _value == nullptr ? false : true;
			};

			//! @brief �Ǘ��I�u�W�F�N�g�̊J��
			VOID APIENTRY Delete ()
			{
				delete _value;
				_value = nullptr;
			};

			//! @brief �Ǘ��I�u�W�F�N�g�̎擾
			//! @return T* �Ǘ��I�u�W�F�N�g�|�C���^
			T* APIENTRY Get () const
			{
				if (_value == nullptr)
				{
					throw(alt::ex::RuntimeException ());
				}

				return _value;
			}

			//! @brief �Ǘ��I�u�W�F�N�g�̎擾
			//! @return T* �Ǘ��I�u�W�F�N�g�̃|�C���^
			T* APIENTRY operator -> () const
			{
				return Get ();
			}

			//! @brief �O������Ǘ��I�u�W�F�N�g��ݒ�
			//! @param t T* �Ǘ��I�u�W�F�N�g
			VOID APIENTRY Set (T* t)
			{
				if (_value != nullptr)
				{
					delete _value;
				}

				_value = t;
			};

		private:
			Binder<T, Args...> (const Binder<T, Args...>&) = delete;
			VOID operator = (const Binder<T, Args...>&) = delete;
			T* _value;
		};
	}
}