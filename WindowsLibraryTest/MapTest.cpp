//! @file	MapTest.cpp
//! @brief	マップに関するクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.8.16

#include "pch.h"
#include "Map.hpp"
#include "TString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (MapTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("MapTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("MapTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("MapTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("MapTest method cleanup.\n");
		}

		TEST_METHOD (MapTest0)
		{
			alt::skeleton::Map<int> map;

			//for (int i = 5; i > 0; i--)
			//{
			//	map.Add (i);
			//}

			//for (int i = 6; i <= 10; i++)
			//{
			//	map.Add (i);
			//}

			for (int i = 0; i < 30; i++)
			{
				map.Add (i);
			}
			for (int i = 0; i < map.Size (); i++)
			{
				alt::TString output;
				auto value = map.GetNode(i);
				CHAR szMD5[40];
				ZeroMemory (szMD5, 40);
				CopyMemory (szMD5, value->_byMD5, 32);
				output.Format (_T ("A)MD5:%S value:%d\n"),
							   szMD5, *(value->_value));
				Logger::WriteMessage (output.Ctr ());
			}
			Logger::WriteMessage ("----------------\n");

			for (int i = 30; i >= 0; i--)
			{
				map.Remove (i);
			}
			for (int i = 30; i >= 0; i--)
			{
				map.Add (i);
			}
			for (int i = 0; i < map.Size (); i++)
			{
				alt::TString output;
				auto value = map.GetNode(i);
				CHAR szMD5[40];
				ZeroMemory (szMD5, 40);
				CopyMemory (szMD5, value->_byMD5, 32);
				output.Format (_T ("a)MD5:%S value:%d\n"),
							   szMD5, *(value->_value));
				Logger::WriteMessage (output.Ctr ());
			}
			Logger::WriteMessage ("----------------\n");

			//for (int i = 0; i < map.Size (); i++)
			//{
			//	alt::TString output;
			//	auto value = map[i];
			//	output.Format (_T ("DATA:%d\n"), *value);
			//	Logger::WriteMessage (output.Ctr ());
			//}

			//for (int i = 1; i <= 10; i++)
			//{
			//	map.Remove (i);
			//}

			//Assert::AreEqual<SIZE_T> (0, map.Size ());

			//for (int i = 5; i > 0; i--)
			//{
			//	map.Add (i);
			//}

			//for (int i = 6; i <= 10; i++)
			//{
			//	map.Add (i);
			//}

			//for (int i = 3; i >= 1; i--)
			//{
			//	map.Remove (i);
			//}

			//for (int i = 8; i <= 10; i++)
			//{
			//	map.Remove (i);
			//}

			//Assert::AreEqual<SIZE_T> (4, map.Size ());

			//for (int i = 0; i < map.Size (); i++)
			//{
			//	alt::TString output;
			//	auto value = map[i];
			//	output.Format (_T ("DATA:%d\n"), *value);
			//	Logger::WriteMessage (output.Ctr ());
			//}

			//map.Clear ();
			//Assert::AreEqual<SIZE_T> (0, map.Size ());
		}

		TEST_METHOD (MapTest1)
		{
			alt::skeleton::Map<int> map;

			for (int i = 1; i <= 10; i++)
			{
				map.Add (i);
			}

			int i = 1;
			for (auto itr : map)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			i = 1;
			for (auto& itr : map)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			for (int i = 0; i < 5; i++)
			{
				map.Remove (i);
			}

			i = 5;
			for (auto itr : map)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			i = 5;
			for (auto& itr : map)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			while (map.Size ())
			{
				map.Remove (*(map[0]));
			}

			Assert::AreEqual<SIZE_T> (0, map.Size (), _T ("invalid size.\n"));

			for (int i = 4; i >= 0; i--)
			{
				map.Add (i);
			}

			for (int i = 5; i < 10; i++)
			{
				map.Add (i);
			}

			map.Clear ();

			for (int i = 0; i < 10; i++)
			{
				map.Add (i);
			}

			i = 0;
			for (auto itr : map)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			i = 0;
			for (auto& itr : map)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			int iTarget = 0;
			for (int i = 0; i < map.Size (); i++)
			{
				int* piVal = map.Get (i);
				Assert::AreEqual<int> (i, *piVal);
				if (*piVal == 2) iTarget = *piVal;
			}

			map.Remove (iTarget);
			Assert::AreEqual<SIZE_T> (9, map.Size ());
		}

		TEST_METHOD (MapTest2)
		{
			int index;
			alt::TString data;
			alt::skeleton::Map<alt::TString> map1;

			for (int i = 0; i < 10; i++)
			{
				data.Format (_T ("DATA No.%02d\n"), i);
				map1.Add (data);
			}

			alt::skeleton::Map<alt::TString> map2 (map1);

			index = 0;
			for (auto& item2 : map2)
			{
				const auto& item1 = map1[index++];
				Assert::IsTrue (0 == lstrcmp ((*item1).Ctr (), item2.Ctr ()));
			}

			map1.Clear ();

			index = 0;
			for (auto itr = map2.begin (); itr != map2.end (); ++itr)
			{
				data.Format (_T ("DATA No.%02d\n"), index++);
				Logger::WriteMessage (data.Ctr ());
				Assert::IsTrue (0 == lstrcmp (data.Ctr (), (*itr).Ctr ()));
			}

			index = 0;
			for (auto& itr = map2.begin (); itr != map2.end (); ++itr)
			{
				data.Format (_T ("DATA No.%02d\n"), index++);
				Logger::WriteMessage (data.Ctr ());
				auto& v = *itr;
				Assert::IsTrue (0 == lstrcmp (data.Ctr (), (*itr).Ctr ()));
			}

			index = 0;
			for (alt::TString itr : map2)
			{
				data.Format (_T ("DATA No.%02d\n"), index++);
				Logger::WriteMessage (data.Ctr ());
				Assert::IsTrue (0 == lstrcmp (data.Ctr (), itr.Ctr ()));
			}

			index = 0;
			for (alt::TString& itr : map2)
			{
				data.Format (_T ("DATA No.%02d\n"), index++);
				Logger::WriteMessage (data.Ctr ());
				Assert::IsTrue (0 == lstrcmp (data.Ctr (), itr.Ctr ()));
			}
		}
	};
}