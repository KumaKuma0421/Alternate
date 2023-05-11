//! @file	DBLibrary.h
//! @brief	DB APIを集約するクラスの基底クラス
//! @note	DllMain()のプロトタイプもここで宣言しています。
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.4

#pragma once

BOOL APIENTRY DllMain (HMODULE hModule, DWORD dwReasonForCall, LPVOID lpvReserved);

namespace alt
{
	//! @class	DBLibrary
	//! @brief	DB APIを集約するクラスの基底クラス
	class DLL_DECLSPEC DBLibrary
	{
	public:
		DBLibrary ();
		virtual ~DBLibrary ();
	};
}