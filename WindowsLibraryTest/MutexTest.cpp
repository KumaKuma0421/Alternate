//! @file	MutexTest.cpp
//! @brief	�~���[�e�b�N�X�Ɋւ���WindowsAPI���W�񂵂��N���X�̃e�X�g�N���X
//! @author	kumakuma0421@gmail.com
//! @date	2020.3.29

#include "pch.h"
#include "Mutex.h"
#include "Thread.h"
#include "TString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
#define MUTEX_NAME _T("MutexTest")
#define EVENT_NAME _T("EventTest")

	TEST_CLASS (MutexTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("MutexTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("MutexTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("MutexTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("MutexTest method cleanup.\n");
		}

		TEST_METHOD (MutexTest1)
		{
			auto ThreadFunction = [](LPVOID lpvParam)->DWORD
			{
				alt::TString keyword = (LPCTSTR)lpvParam;
				alt::TString msg;
				BOOL ret;
				alt::Mutex mutex;
				ret = mutex.Open (MUTEX_NAME);
				Assert::IsTrue (ret, _T("Mutex::Open() failed.\n"));

				msg.Format (_T ("%s start\n"), keyword.Ctr());
				Logger::WriteMessage (msg.Ctr ());

				for (int i = 0; i < 10; i++)
				{
					DWORD dwRet = mutex.Wait ();
					Assert::AreEqual<DWORD> (WAIT_OBJECT_0, dwRet, _T ("Mutex::Wait() not return WAIT_OBJECT_0.\n"));

					msg.Format (_T ("%s %d.\n"), keyword.Ctr(), i);
					Logger::WriteMessage (msg.Ctr ());

					Sleep (10);

					ret = mutex.Release ();
					Assert::IsTrue (ret, _T("Mutex::Release() failed.\n"));
				}

				msg.Format (_T ("%s stopped.\n"), keyword.Ctr());
				Logger::WriteMessage (msg.Ctr ());

				return 0;
			};

			alt::Mutex mutex;
			alt::Thread MutexThreads[10];
			HANDLE hMutexs[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
			BOOL ret;

			ret = mutex.Create (MUTEX_NAME);
			Assert::IsTrue (ret, _T ("Mutex::Create() failed.\n"));
			
			DWORD dwRet = mutex.Wait ();
			Assert::AreEqual<DWORD> (WAIT_OBJECT_0, dwRet, _T ("Mutex::Wait() not return WAIT_OBJECT_0.\n"));

			for (int i = 0; i < 10; i++)
			{
				alt::TString Keyword;
				Keyword.Format (_T ("Thread_%02d"), i);
				ret = MutexThreads[i].Create (ThreadFunction, (LPVOID)Keyword.Ctr());
				Sleep (10); // ���̃X���[�v�́A�X���b�h�����S�Ƀp�����[�^����荞�ނ��߂ɐݒ肵�Ă��܂��B
				Assert::IsTrue (ret, _T("Thread::Create() failed.\n"));

				hMutexs[i] = MutexThreads[i].GetHandle ();
			}

			ret = mutex.Release ();
			Assert::IsTrue (ret, _T ("Mutex::Release() failed.\n"));

			WaitForMultipleObjects (10, hMutexs, TRUE, INFINITE);
		}
	};
}