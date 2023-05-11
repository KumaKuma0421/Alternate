//! @file	FreightTest.cpp
//! @brief	プリミティブな変数を管理するコンテナクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#include "pch.h"
#include "Freight.hpp"
#include "VirtualMemory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (FreightTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("FreightTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("FreightTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("FreightTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("FreightTest method cleanup.\n");
		}

		TEST_METHOD (FreightTestB)
		{
			const int arraySize = 10;
			CHAR caData[arraySize]{ 'A','B','C','D','E','F','G','H','I','J' };

			alt::skeleton::Freight<CHAR> freight1;
			freight1.SetData (caData, arraySize);

			ZeroMemory (caData, arraySize); // 影響ないことを確認。

			alt::skeleton::Freight<CHAR> freight2 (freight1);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					freight1.GetData (),
					freight2.GetData (),
					freight1.GetSize ()),
				_T ("copy failed.\n"));

			Assert::AreNotEqual<int> (
				0,
				_memicmp (
					caData,
					freight1.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			alt::skeleton::Freight<CHAR> freight3 (freight1);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					freight1.GetData (),
					freight3.GetData (),
					freight1.GetSize ()),
				_T ("copy failed.\n"));

			Assert::AreNotEqual<int> (
				0,
				_memicmp (
					caData,
					freight3.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			alt::skeleton::Freight<CHAR> freight4 (caData, arraySize);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					caData,
					freight4.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			freight1.Clear (); // freight1はClear()後に消滅
			freight4.Clear (); // freight4はClear()後に消滅
			// freight2はクリアせずに消滅
			// freight3はクリアせずに消滅
		}

		TEST_METHOD (FreightTestHO)
		{
			const int arraySize = 10;
			CHAR caData[arraySize]{ 'A','B','C','D','E','F','G','H','I','J' };

			alt::HeapMemory memory;
			Assert::IsTrue (memory.Open(), _T("HeapMemory::Open() failed.\n"));
			alt::skeleton::Freight<CHAR> freight1(&memory);

			freight1.SetData (caData, arraySize);

			ZeroMemory (caData, arraySize); // 影響ないことを確認。

			alt::skeleton::Freight<CHAR> freight2 (freight1);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					freight1.GetData (),
					freight2.GetData (),
					freight1.GetSize ()),
				_T ("copy failed.\n"));

			Assert::AreNotEqual<int> (
				0,
				_memicmp (
					caData,
					freight1.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			alt::skeleton::Freight<CHAR> freight3 (freight1);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					freight1.GetData (),
					freight3.GetData (),
					freight1.GetSize ()),
				_T ("copy failed.\n"));

			Assert::AreNotEqual<int> (
				0,
				_memicmp (
					caData,
					freight3.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			alt::skeleton::Freight<CHAR> freight4 (caData, arraySize);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					caData,
					freight4.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			freight1.Clear (); // freight1はClear()後に消滅
			freight4.Clear (); // freight4はClear()後に消滅
							   // freight2はクリアせずに消滅
							   // freight3はクリアせずに消滅
		}

		TEST_METHOD (FreightTestHC)
		{
			const int arraySize = 10;
			CHAR caData[arraySize]{ 'A','B','C','D','E','F','G','H','I','J' };

			alt::HeapMemory memory;
			Assert::IsTrue (memory.Create(), _T("HeapMemory::Create() failed.\n"));
			alt::skeleton::Freight<CHAR> freight1(&memory);

			freight1.SetData (caData, arraySize);

			ZeroMemory (caData, arraySize); // 影響ないことを確認。

			alt::skeleton::Freight<CHAR> freight2 (freight1);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					freight1.GetData (),
					freight2.GetData (),
					freight1.GetSize ()),
				_T ("copy failed.\n"));

			Assert::AreNotEqual<int> (
				0,
				_memicmp (
					caData,
					freight1.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			alt::skeleton::Freight<CHAR> freight3 (freight1);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					freight1.GetData (),
					freight3.GetData (),
					freight1.GetSize ()),
				_T ("copy failed.\n"));

			Assert::AreNotEqual<int> (
				0,
				_memicmp (
					caData,
					freight3.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			alt::skeleton::Freight<CHAR> freight4 (caData, arraySize);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					caData,
					freight4.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			freight1.Clear (); // freight1はClear()後に消滅
			freight4.Clear (); // freight4はClear()後に消滅
							   // freight2はクリアせずに消滅
							   // freight3はクリアせずに消滅
		}

		TEST_METHOD (FreightTestV1)
		{
			const int arraySize = 10;
			CHAR caData[arraySize]{ 'A','B','C','D','E','F','G','H','I','J' };

			alt::VirtualMemory1 memory;
			alt::skeleton::Freight<CHAR> freight1(&memory);

			freight1.SetData (caData, arraySize);

			ZeroMemory (caData, arraySize); // 影響ないことを確認。

			alt::skeleton::Freight<CHAR> freight2 (freight1);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					freight1.GetData (),
					freight2.GetData (),
					freight1.GetSize ()),
				_T ("copy failed.\n"));

			Assert::AreNotEqual<int> (
				0,
				_memicmp (
					caData,
					freight1.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			alt::skeleton::Freight<CHAR> freight3 (freight1);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					freight1.GetData (),
					freight3.GetData (),
					freight1.GetSize ()),
				_T ("copy failed.\n"));

			Assert::AreNotEqual<int> (
				0,
				_memicmp (
					caData,
					freight3.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			alt::skeleton::Freight<CHAR> freight4 (caData, arraySize);

			Assert::AreEqual<int> (
				0,
				_memicmp (
					caData,
					freight4.GetData (),
					arraySize),
				_T ("copy failed.\n"));

			freight1.Clear (); // freight1はClear()後に消滅
			freight4.Clear (); // freight4はClear()後に消滅
							   // freight2はクリアせずに消滅
							   // freight3はクリアせずに消滅
		}
	};
}