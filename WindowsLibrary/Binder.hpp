//! @file	Binder.hpp
//! @brief	newオブジェクトを管理するコンテナクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#pragma once

#include "Exception.h"

namespace alt
{
	namespace skeleton
	{
		//! @class	Binder
		//! @brief	newオブジェクトを管理するコンテナクラス
		template <class T, class... Args>
		class Binder
		{
		public:
			//! @brief コンストラクタ
			Binder<T, Args...> ()
			{
				_value = nullptr;
			};

			//! @brief コンストラクタ
			//! @param t 管理オブジェクトポインタ
			Binder<T, Args...> (T* t)
			{
				_value = t;
			}

			//! @brief デストラクタ
			virtual APIENTRY ~Binder ()
			{
				Delete ();
			};

			//! @brief 管理オブジェクトの作成
			//! @param ...args コンストラクタ引数
			//! @return bool
			//! @retval true(成功)
			//! @retval false(失敗)
			bool APIENTRY New (Args... args)
			{
				_value = new T (args...);
				return _value == nullptr ? false : true;
			};

			//! @brief 管理オブジェクトの開放
			VOID APIENTRY Delete ()
			{
				delete _value;
				_value = nullptr;
			};

			//! @brief 管理オブジェクトの取得
			//! @return T* 管理オブジェクトポインタ
			T* APIENTRY Get () const
			{
				if (_value == nullptr)
				{
					throw(alt::ex::RuntimeException ());
				}

				return _value;
			}

			//! @brief 管理オブジェクトの取得
			//! @return T* 管理オブジェクトのポインタ
			T* APIENTRY operator -> () const
			{
				return Get ();
			}

			//! @brief 外部から管理オブジェクトを設定
			//! @param t T* 管理オブジェクト
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