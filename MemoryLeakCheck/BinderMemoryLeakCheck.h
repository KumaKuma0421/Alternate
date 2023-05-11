//! @file	BinderMemoryLeakCheck.h
//! @brief	Binderのメモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#pragma once

#include "LeakCheck.h"
#include "Binder.hpp"
#include "Freight.hpp"

namespace LeakCheck
{
	//! @class	BinderMemoryLeakCheck
	//! @brief	Binderのメモリーリークチェッククラス
	class BinderMemoryLeakCheck : public LeakCheck
	{
	public:
		//! @brief コンストラクタ
		BinderMemoryLeakCheck ();

		//! @brief デストラクタ
		virtual ~BinderMemoryLeakCheck ();

		//! @brief 初期化処理
		virtual BOOL Init ();

		//! @brief メモリーリーク調査
		virtual BOOL DoAction ();

	private:
		BOOL DoAction1 ();
		BOOL DoAction2 ();
		BOOL DoAction3 ();
		BOOL DoAction4 ();
	};
}