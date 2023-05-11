//! @file	LinkedListTest.cpp
//! @brief	連結リストに関するクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.28

#include "pch.h"
#include "LinkedList.hpp"
#include "TString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (LinkedListTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("LinkedListTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("LinkedListTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("LinkedListTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("LinkedListTest method cleanup.\n");
		}

		TEST_METHOD (LinkedListTest0)
		{
			alt::skeleton::LinkedList<int> linkedList;

			for (int i = 5; i > 0; i--)
			{
				linkedList.AddFirst (i);
			}

			for (int i = 6; i <= 10; i++)
			{
				linkedList.AddLast (i);
			}

			for (int i = 0; i < linkedList.Size (); i++)
			{
				alt::TString output;
				auto value = linkedList[i];
				output.Format (_T ("DATA:%d\n"), *value);
				Logger::WriteMessage (output.Ctr ());
			}

			int notHit = 0;
			linkedList.Remove (notHit);

			for (int i = 1; i <= 10; i++)
			{
				if (i % 2 == 0) linkedList.RemoveFirst ();
				else linkedList.RemoveLast ();
			}

			Assert::AreEqual<SIZE_T> (0, linkedList.Size ());

			for (int i = 5; i > 0; i--)
			{
				linkedList.AddFirst (i);
			}

			for (int i = 6; i <= 10; i++)
			{
				linkedList.AddLast (i);
			}

			for (int i = 3; i >= 1; i--)
			{
				linkedList.Remove (i);
			}

			for (int i = 8; i <= 10; i++)
			{
				linkedList.Remove (i);
			}

			Assert::AreEqual<SIZE_T> (4, linkedList.Size ());

			for (int i = 0; i < linkedList.Size (); i++)
			{
				alt::TString output;
				auto value = linkedList[i];
				output.Format (_T ("DATA:%d\n"), *value);
				Logger::WriteMessage (output.Ctr ());
			}

			linkedList.Clear ();
			Assert::AreEqual<SIZE_T> (0, linkedList.Size ());
		}

		TEST_METHOD (LinkedListTest1)
		{
			alt::skeleton::LinkedList<int> linkedList;

			for (int i = 6; i <= 10; i++)
			{
				linkedList.AddLast (i);
			}

			for (int i = 5; i >= 1; i--)
			{
				linkedList.AddFirst (i);
			}

			int i = 1;
			for (auto itr : linkedList)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			i = 1;
			for (auto& itr : linkedList)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			for (int i = 0; i < 5; i++)
			{
				linkedList.RemoveFirst ();
			}

			i = 6;
			for (auto itr : linkedList)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			i = 6;
			for (auto& itr : linkedList)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			while (linkedList.Size ())
			{
				linkedList.RemoveLast ();
			}

			Assert::AreEqual<SIZE_T> (0, linkedList.Size (), _T ("invalid size.\n"));

			for (int i = 4; i >= 0; i--)
			{
				linkedList.AddFirst (i);
			}

			for (int i = 5; i < 10; i++)
			{
				linkedList.AddLast (i);
			}

			linkedList.Clear ();

			for (int i = 4; i >= 0; i--)
			{
				linkedList.AddFirst (i);
			}

			for (int i = 5; i < 10; i++)
			{
				linkedList.AddLast (i);
			}

			i = 0;
			for (auto itr : linkedList)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			i = 0;
			for (auto& itr : linkedList)
			{
				Assert::AreEqual<int> (i++, itr, _T ("invalid value get.\n"));
			}

			int iTarget = 0;
			for (int i = 0; i < linkedList.Size (); i++)
			{
				int* piVal = linkedList.Get (i);
				Assert::AreEqual<int> (i, *piVal);
				if (*piVal == 2) iTarget = *piVal;
			}

			linkedList.Remove (iTarget);
			Assert::AreEqual<SIZE_T> (9, linkedList.Size ());
		}

		TEST_METHOD (LinkedListTest2)
		{
			alt::skeleton::LinkedList<alt::TString> linkedList;
			alt::TString message;

			for (int i = 1; i <= 10; i++)
			{
				message.Format (_T ("This line is No.%d.\n"), i);
				linkedList.Enqueue (message);
			}

			for (auto item : linkedList)
			{
				Logger::WriteMessage (item.Ctr ());
			}

			for (auto& item : linkedList)
			{
				Logger::WriteMessage (item.Ctr ());
			}

			SIZE_T currentSize = linkedList.Size ();
			for (int i = 1; i <= currentSize; i++)
			{
				alt::TString response;
				linkedList.Dequeue (response);
				Logger::WriteMessage (response.Ctr ());
				message.Format (_T ("This line is No.%d.\n"), i);
				Assert::IsTrue (message == response);
			}
		}

		TEST_METHOD (LinkedListTest3)
		{
			int index;
			alt::TString data;
			alt::skeleton::LinkedList<alt::TString> linkedList1;

			for (int i = 0; i < 10; i++)
			{
				data.Format (_T ("DATA No.%02d\n"), i);
				linkedList1.AddLast (data);
			}

			alt::skeleton::LinkedList<alt::TString> linkedList2 (linkedList1);

			index = 0;
			for (auto& item2 : linkedList2)
			{
				const auto& item1 = linkedList1[index++];
				Assert::IsTrue (0 == lstrcmp ((*item1).Ctr (), item2.Ctr ()));
			}

			linkedList1.Clear ();

			index = 0;
			for (auto itr = linkedList2.begin (); itr != linkedList2.end (); ++itr)
			{
				data.Format (_T ("DATA No.%02d\n"), index++);
				Logger::WriteMessage (data.Ctr ());
				Assert::IsTrue (0 == lstrcmp (data.Ctr (), (*itr).Ctr ()));
			}

			index = 0;
			for (auto& itr = linkedList2.begin (); itr != linkedList2.end (); ++itr)
			{
				data.Format (_T ("DATA No.%02d\n"), index++);
				Logger::WriteMessage (data.Ctr ());
				auto& v = *itr;
				Assert::IsTrue (0 == lstrcmp (data.Ctr (), (*itr).Ctr ()));
			}

			index = 0;
			for (alt::TString itr : linkedList2)
			{
				data.Format (_T ("DATA No.%02d\n"), index++);
				Logger::WriteMessage (data.Ctr ());
				Assert::IsTrue (0 == lstrcmp (data.Ctr (), itr.Ctr ()));
			}

			index = 0;
			for (alt::TString& itr : linkedList2)
			{
				data.Format (_T ("DATA No.%02d\n"), index++);
				Logger::WriteMessage (data.Ctr ());
				Assert::IsTrue (0 == lstrcmp (data.Ctr (), itr.Ctr ()));
			}
		}
	};
}