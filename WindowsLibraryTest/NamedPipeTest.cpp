//! @file	NamedPipeTest.cpp
//! @brief	名前付きパイプに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "NamedPipe.h"
#include "Thread.h"
#include "Array.hpp"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define PIPE_NAME _T ("TEST_PIPE")
const int __ciBufferSize = 120;

namespace WindowsLibraryTest
{
	TEST_CLASS (NamedPipeTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("NamedPipeTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("NamedPipeTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("NamedPipeTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("NamedPipeTest method cleanup.\n");
		}

		TEST_METHOD (NamedPipeStreamTest1)
		{
			auto serverFunc = [](LPVOID lpvParam)->DWORD
			{
				alt::NamedPipe pipe;
				BOOL ret;
				TCHAR tszBuff[__ciBufferSize];
				DWORD dwRet;

				ret = pipe.Create (PIPE_NAME, TRUE);
				Assert::IsTrue (ret, _T ("S)NamedPipe::Create() failed.\n"));

				ret = pipe.Connect ();
				Assert::IsTrue (ret, _T ("S)NamedPipe::Connect() failed.\n"));

				while (true)
				{
					dwRet = pipe.PeekRemain ();
					if (dwRet < 0)
					{
						Assert::Fail (_T ("S)NamedPipe::PeekRemain() failed.\n"));
					}

					if (dwRet == 0)
					{
						Logger::WriteMessage ("S)メッセージが到着していません。\n");
						Sleep (1000);
					}
					else break;
				}
				Assert::AreEqual<DWORD> (6 * sizeof (TCHAR), dwRet, _T ("S)NamedPipe::PeekRemain() invalid size.\n"));

				ZeroMemory (tszBuff, sizeof (tszBuff));
				dwRet = pipe.PeekData (tszBuff, __ciBufferSize);
				tszBuff[__ciBufferSize - 1] = NULL;
				Assert::IsTrue (ret, _T ("S)NamedPipe::PeekData() failed.\n"));
				Assert::AreNotEqual<DWORD> (0, dwRet, _T ("S)NamedPipe::PeekData() invalid size.\n"));
				Assert::AreEqual<int> (0, lstrcmp (_T ("12345"), tszBuff), _T ("S)受信(Peek)内容が想定外です。"));

				ZeroMemory (tszBuff, sizeof (tszBuff));
				dwRet = pipe.Read (tszBuff, __ciBufferSize);
				Assert::AreEqual<DWORD> (6 * sizeof (TCHAR), dwRet, _T("NamedPipe::Read() failed.\n"));
				Assert::AreEqual<int> (0, lstrcmp (_T ("12345"), tszBuff), _T ("S)受信(Read)内容が想定外です。"));

				ret = pipe.Close ();
				Assert::IsTrue (ret, _T ("S)NamedPipe::Close() failed.\n"));

				return 0;
			};

			auto clientFunc = [](LPVOID lpvParam)->DWORD
			{
				alt::NamedPipe pipe;
				BOOL ret;
				TCHAR tszBuf[__ciBufferSize];
				DWORD dwRet;

				ret = pipe.Wait (PIPE_NAME);
				Assert::IsTrue (ret, _T ("C)NamedPipe::Wait() failed.\n"));

				ret = pipe.Open (PIPE_NAME);
				Assert::IsTrue (ret, _T ("C)NamedPipe::Open() failed.\n"));

				wsprintf (tszBuf, _T ("%s"), _T ("12345"));
				dwRet = pipe.Write (tszBuf, 6 * sizeof (TCHAR));
				Assert::AreEqual<DWORD> (6 * sizeof (TCHAR), dwRet, _T ("C)NamedPipe::Write() failed.\n"));

				ret = pipe.Close ();
				Assert::IsTrue (ret, _T ("C)NamedPipe::Close() failed.\n"));

				return 0;
			};

			alt::Thread* serverThread = new alt::Thread ();
			serverThread->Create (serverFunc);

			Sleep (1000);

			alt::Thread* clientThread = new alt::Thread ();
			clientThread->Create (clientFunc);

			clientThread->Wait ();
			serverThread->Wait ();

			delete clientThread;
			delete serverThread;
		}

		class ServerPipe : public alt::skeleton::ICallback
		{
		public:
			ServerPipe (int instanceNo) { _instanceNo = instanceNo; };

			BOOL Create ()
			{
				BOOL ret = _pipe.Create (PIPE_NAME, TRUE);
				Assert::IsTrue (ret, _T ("S)NamedPipe::Create() failed.\n"));

				return ret;
			};

			BOOL Connect ()
			{
				BOOL ret = _pipe.Connect ();
				Assert::IsTrue (ret, _T ("S)NamedPipe::Connect() failed.\n"));

				return ret;
			};

			BOOL Disconnect ()
			{
				BOOL ret = _pipe.Disconnect ();
				Assert::IsTrue (ret, _T ("S)NamedPipe::Disconnect() failed.\n"));

				return ret;
			};

			BOOL MessagePeek (LPTSTR lptszBuff, DWORD dwSize)
			{
				DWORD dwCheck;

				while (true)
				{
					dwCheck = _pipe.PeekRemain ();
					if (dwCheck == 0) Sleep (1000);
					else if (dwSize > 0) break;
					else Assert::Fail (_T ("S)NamedPipe::PeekRemain() failed.\n"));
				}

				dwCheck = _pipe.PeekData (lptszBuff, dwSize);
				Assert::AreNotEqual<DWORD> (0, dwCheck, _T ("S)NamedPipe::Peek() return 0.\n"));

				return (dwCheck > 0);
			};

			BOOL Close ()
			{
				BOOL ret = _pipe.Close ();
				Assert::IsTrue (ret, _T ("S)NamedPipe::Close() failed.\n"));

				return ret;
			};

		private:
			ServerPipe () { _instanceNo = -1; };

			DWORD Invoke (LPVOID lpvParam)
			{
				BYTE byBuffer[__ciBufferSize];
				alt::TString message;

				for (;;)
				{
					ZeroMemory (byBuffer, __ciBufferSize);
					DWORD dwSize = _pipe.Read (byBuffer, __ciBufferSize);
					if (dwSize == (DWORD)-1)
					{
						message.Format (_T (" S%d)Maybe disconnect.\n"), _instanceNo);
						Logger::WriteMessage (message.Ctr ());
						break;
					}
					else
					{
						byBuffer[dwSize] = '\0';
						message = alt::Utility::GetFormatTime (alt::Utility::GetLocalTime ());
						TCHAR tszBuf[__ciBufferSize];
						wsprintf (tszBuf, _T (" S%d)Received:%s %s\n"), _instanceNo, message.Ctr (), (LPCTSTR)byBuffer);
						Logger::WriteMessage (tszBuf);
					}
					Sleep (0);

					message = _T ("NEXT");
					_pipe.Write (message.Ctr (), message.Byte ());
					Sleep (0);
				}

				this->Disconnect ();
				this->Close ();

				return 0;
			};

		private:
			int _instanceNo;
			alt::NamedPipe _pipe;
		};

		class ClientPipe : public alt::skeleton::ICallback
		{
		public:
			ClientPipe (int instanceNo) { _instanceNo = instanceNo; };

			BOOL Wait ()
			{
				BOOL ret = _pipe.Wait (PIPE_NAME, 3000);
				Assert::IsTrue (ret, _T ("C)NamedPipe::Wait() failed.\n"));

				return ret;
			};

			BOOL Open ()
			{
				BOOL ret = _pipe.Open (_T ("TEST_PIPE"));
				Assert::IsTrue (ret, _T ("C)NamedPipe::Open() failed.\n"));

				return ret;
			};

			BOOL Close ()
			{
				BOOL ret = _pipe.Close ();
				Assert::IsTrue (ret, _T ("C)NamedPipe::Close() failed.\n"));

				return ret;
			};

		private:
			ClientPipe () { _instanceNo = -1; };

			DWORD Read (LPVOID lpvBuf, DWORD dwSize)
			{
				return _pipe.Read (lpvBuf, dwSize);
			};

			DWORD Write (LPVOID lpvBuf, DWORD dwSize)
			{
				return _pipe.Write (lpvBuf, dwSize);
			};

			DWORD Invoke (LPVOID lpvParam)
			{
				TCHAR tszBuff[__ciBufferSize];
				alt::TString message;

				for (int i = 0; i < 10; i++)
				{
					message = alt::Utility::GetFormatTime (alt::Utility::GetLocalTime ());
					wsprintf (tszBuff, _T ("C%d)%03d %s"), _instanceNo, i, message.Ctr ());
					DWORD dwWrite = this->Write (tszBuff, lstrlen (tszBuff) * sizeof (TCHAR));
					Sleep (0);

					ZeroMemory (tszBuff, sizeof (tszBuff));
					DWORD dwRead = this->Read (tszBuff, __ciBufferSize);
					tszBuff[dwRead/2] = NULL;
					message.Format (_T ("  C%d)Received %s.\n"), _instanceNo, tszBuff);
					Logger::WriteMessage (message.Ctr());
					Sleep (500);
				}

				this->Close ();

				return 0;
			};

		private:
			int _instanceNo;
			alt::NamedPipe _pipe;
		};

		TEST_METHOD (NamedPipeStreamTest2)
		{
			auto serverFunc = [](LPVOID lpvParam)->DWORD
			{
				alt::skeleton::Array<ServerPipe> pipes;
				alt::skeleton::Array<alt::Thread> container;
				TCHAR tszBuff[10] = { 0 };

				for (int i = 1; i <= 10; i++)
				{
					ServerPipe* pServerPipe = new ServerPipe (i);
					pServerPipe->Create ();
					pServerPipe->Connect ();
					pipes.Add (*pServerPipe);

					pServerPipe->MessagePeek (tszBuff, sizeof (tszBuff));
					if (!_memicmp ("EXIT", tszBuff, 4))
					{
						Logger::WriteMessage (_T ("S)Loop escaped.\n"));
						break;
					}

					alt::Thread* pThread = new alt::Thread (pServerPipe);
					pThread->Create ();
					container.Add (*pThread);
				}

				return 0;
			};

			auto clientFunc = [](LPVOID lpvParam)->DWORD
			{
				int clientNo = *((int*)(lpvParam));
				ClientPipe clientPipe (clientNo);
				clientPipe.Wait ();
				clientPipe.Open ();

				alt::Thread thread (&clientPipe);
				thread.Create ();
				thread.Wait ();

				return 0;
			};

			alt::Thread* serverThread = new alt::Thread ();
			serverThread->Create (serverFunc);

			Sleep (3000);

			alt::Thread* clientThread1 = new alt::Thread ();
			int client1 = 1;

			alt::Thread* clientThread2 = new alt::Thread ();
			int client2 = 2;

			alt::Thread* clientThread3 = new alt::Thread ();
			int client3 = 3;

			clientThread1->Create (clientFunc, &client1);
			Sleep (1000);
			clientThread2->Create (clientFunc, &client2);
			Sleep (1000);
			clientThread3->Create (clientFunc, &client3);

			clientThread1->Wait ();
			clientThread2->Wait ();
			clientThread3->Wait ();

			delete clientThread1;
			delete clientThread2;
			delete clientThread3;

			alt::NamedPipe terminator;
			terminator.Wait (PIPE_NAME);
			terminator.Open (PIPE_NAME);
			terminator.Write ("EXIT", 4);
			Logger::WriteMessage ("'EXIT' send to server pipe.\n");

			serverThread->Wait (3000);
			delete serverThread;
		}
	};
}