//! @file	Freight.hpp
//! @brief	プリミティブな変数を管理するコンテナクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#pragma once

#include "HeapMemory.h"

namespace alt
{
	namespace skeleton
	{
		//! @class	Freight
		//! @brief	プリミティブな変数を管理するコンテナクラス
		template<class T>
		class Freight
		{
		public:
			//! @brief コンストラクタ
			Freight<T> ()
			{
				_memory = new alt::BasicMemory ();
				_lpData = NULL;
				_size = 0;
			};

			//! @brief コンストラクタ
			//! @param memory IMemoryオブジェクト
			Freight<T> (IMemory* memory)
			{
				if (memory == NULL)
					_memory = new alt::BasicMemory ();
				else
					_memory = memory;
				_lpData = NULL;
				_size = 0;
			};

			//! @brief コンストラクタ
			//! @param lpData 格納データ
			//! @param size 格納データサイズ（T単位の個数）
			//! @param memory HeapMemoryManager
			Freight<T> (T* lpData, size_t size, IMemory* memory = NULL)
				:Freight (memory)
			{
				SetData (lpData, size);
			};

			//! @brief コピーコンストラクタ
			//! @param base コピー元オブジェクト
			Freight<T> (const Freight<T>& base)
			{
				if (dynamic_cast<alt::BasicMemory*>(base._memory))
					_memory = new alt::BasicMemory ();
				else
					_memory = base._memory;
				_lpData = NULL;
				_size = 0;
				SetData (base._lpData, base._size);
			};

			//! @brief デストラクタ
			APIENTRY ~Freight ()
			{
				Clear ();
				auto basicMemory = dynamic_cast<alt::BasicMemory*>(_memory);
				if (basicMemory)
				{
					delete basicMemory;
				}
				_memory = NULL;
			};

			//! @brief 値の設定
			//! @param lpData 格納データ
			//! @param size 格納データサイズ（T単位の個数）
			VOID APIENTRY SetData (T* lpData, size_t size)
			{
				New (size);
				// ここで正確に情報をコピーできるのは、
				// charやunsigned char などのプリミティブな
				// 型だけになる。コンストラクタやデストラクタを
				// 持つものは、対応不可。operator = ()も動かない。
				CopyMemory (_lpData, lpData, size * sizeof (T));
				_size = size;
			};

			//! @brief 事前のメモリ取得
			//! @param size 取得サイズ
			VOID APIENTRY New (size_t size)
			{
				Clear ();
				_lpData = static_cast<T*>(_memory->Allocate (sizeof (T) * size));
				_size = size;
			};

			//! @brief 格納データの取得
			//! @return T* 格納データ
			T* APIENTRY GetData () const { return _lpData; };

			//! @brief 格納データのサイズ取得
			//! @return size_t 格納データサイズ（T単位）
			size_t APIENTRY GetSize () const { return _size; }

			//! @brief 格納領域の開放
			VOID APIENTRY Clear ()
			{
				if (_lpData != NULL)
				{
					_memory->Free (_lpData);
					_lpData = NULL;
					_size = 0;
				}
			};

			//! @brief 安全なオブジェクトのコピー
			//! @param base コピー元オブジェクト
			VOID APIENTRY operator = (const Freight<T>& base)
			{
				if (dynamic_cast<alt::BasicMemory*>(base._memory))
					_memory = new alt::BasicMemory ();
				else
					_memory = base._memory;
				SetData (base._lpData, base._size);
			};

		private:
			IMemory* _memory;
			T* _lpData;
			size_t _size;
		};
	}
}