//! @file	FreightMemoryLeakCheck.h
//! @brief	Freightのメモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#pragma once

#include "LeakCheck.h"
#include "Freight.hpp"

namespace LeakCheck
{
	//! @class	FreightMemoryLeakCheck
	//! @brief	Arrayのメモリーリークチェッククラス
	class FreightMemoryLeakCheck : public LeakCheck
	{
	public:
		//! @brief コンストラクタ
		FreightMemoryLeakCheck ();

		//! @brief デストラクタ
		virtual ~FreightMemoryLeakCheck ();

		//! @brief 初期化処理
		virtual BOOL Init ();

		//! @brief メモリーリーク調査
		virtual BOOL DoAction ();
	
	private:
		BOOL DoAction1 ();
		BOOL DoAction2 ();
		BOOL DoAction3 ();
		BOOL DoAction4 ();
		BOOL DoAction5 ();
		BOOL DoAction6 ();
	};
}