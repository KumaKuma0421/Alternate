//! @file	TcpConnectorTest.cpp
//! @brief	TCP通信に関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.03

#include "pch.h"
#include "SocketBuilder.h"
#include "TcpConnector.h"
#include "Thread.h"
#include "TString.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (TcpConnectorTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("TcpConnectorTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("TcpConnectorTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("TcpConnectorTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("TcpConnectorTest method cleanup.\n");
		}

		TEST_METHOD (TcpConnecorTest)
		{
			alt::Thread* serverThread = new alt::Thread ();
			auto serverFunction = [](LPVOID lpvParam)->DWORD
			{
				Logger::WriteMessage ("[Server]Now start server thread.\n");

				alt::SocketBuilder builder;
				Assert::IsTrue(builder.Startup (), _T("SocketBuilder::Startup() failed.\n"));
				Assert::IsTrue(builder.Prepare (50000), _T("SocketBuilder::Prepare() failed.\n"));

				do
				{
					alt::TcpConnector* connector = builder.Wait ();
					Assert::IsNotNull(connector, _T("SocketBuilder::Wait() failed.\n"));

					alt::Thread* workerThread = new alt::Thread ();
					auto connectorFunction = [](LPVOID lpvParam)->DWORD
					{
						alt::TcpConnector* connector = (alt::TcpConnector*)lpvParam;
						INT iPacketSize = -1;
						BYTE byBuf[256] = { 0 };
						alt::TString message;

						Logger::WriteMessage ("[Worker]Now start worker thread.\n");

						INT ret;
						do
						{
							ret = connector->Recv ((LPVOID)&iPacketSize, sizeof (INT));
							if (ret == 0)
							{
								Logger::WriteMessage ("[Worker]TcpConnector::Recv() return 0.\n");
								break;
							}

							ret = connector->Recv (byBuf, iPacketSize);
							if (ret == 0)
							{
								Logger::WriteMessage ("[Worker]TcpConnector::Recv() return 0.\n");
								break;
							}

							message = (LPTSTR)byBuf;
							Logger::WriteMessage (message.Ctr());
						} while (1);
						
						Logger::WriteMessage ("[Worker]Now finish worker thread.\n");
						return 0;
					};

					workerThread->Create (connectorFunction, connector);
					workerThread->Wait ();
					Assert::AreEqual(0, connector->Close (), _T("TcpConnector::Close() failed.\n"));
					Logger::WriteMessage ("[Server]TcpConnector::Close()\n");
					delete workerThread;
				} while (false);

				Logger::WriteMessage ("[Server]Now finish server thread.\n");
				return 0;
			};

			alt::Thread* clientThread = new alt::Thread ();
			auto clientFunction = [](LPVOID lpvParam)->DWORD
			{
				Logger::WriteMessage ("[Client]Now start client thread.\n");

				alt::SocketBuilder builder;
				Assert::IsTrue(builder.Startup (), _T("SocketBuilder::Startup() failed.\n"));
				alt::TcpConnector* connector = builder.CreateTcpConnector (_T ("127.0.0.1"), 50000, 1, 10);
				Assert::IsNotNull (connector, _T("SocketBuilder::CreateTcpConnector() failed.\n"));

				alt::TString message;
				INT iPacketSize;
				INT ret;
				CONST INT iLoop = 10;

				for (int i = 1; i <= iLoop; i++)
				{
					alt::TString guid = alt::Utility::MakeGUID ();
					alt::TString time = alt::Utility::GetFormatTime (alt::Utility::GetLocalTime ());
					message.Format (_T("[%2d]%s %s\n"), i, time.Ctr(), guid.Ctr());
					iPacketSize = message.Byte();

					ret = connector->Send ((LPVOID)&iPacketSize, sizeof (INT));
					Assert::AreNotEqual (SOCKET_ERROR, ret, _T("TcpConnector::Send() failed.\n"));

					ret = connector->Send ((LPVOID)message.Ctr(), iPacketSize);
					Assert::AreNotEqual (SOCKET_ERROR, ret, _T("TcpConnector::Send() failed.\n"));
				}

				Assert::AreEqual(0, connector->Close (), _T("TcpConnector::Close() failed.\n"));

				Logger::WriteMessage ("[Client]Now finish client thread.\n");
				return 0;
			};

			serverThread->Create (serverFunction);
			Sleep (1000);
			clientThread->Create (clientFunction);
			Sleep (1000);

			clientThread->Wait ();
			Logger::WriteMessage ("Client thread finished.\n");
			serverThread->Wait ();
			Logger::WriteMessage ("Server thread finished.\n");

			delete clientThread;
			delete serverThread;
			Logger::WriteMessage ("Finished.\n");
		}

		TEST_METHOD (TcpClientTest)
		{
			alt::SocketBuilder builder;
			BOOL ret;

			ret = builder.Startup ();
			Assert::IsTrue(ret, _T("SocketBuilder::Startup() failed.\n"));
			
			alt::TString buffer (80);
			ret = builder.GetHostByName (_T ("www.yahoo.co.jp"), buffer.Ptr ());
			Assert::IsTrue(ret, _T("SocketBuilder::GetHostByName() failed.\n"));

			alt::TcpConnector* connector = builder.CreateTcpConnector (buffer.Ctr(), 80, 1, 3);
			Assert::IsNotNull(connector, _T("SocketBuilder::CreateTcpConnector() failed.\n"));

			INT iRet;
			alt::TString sendMessage = _T ("GET / HTTP1.1");
			alt::TString recvMessage (4096);
			alt::TString readable (4096);
			iRet = connector->Send ((LPVOID)sendMessage.Ctr (), sendMessage.Len ());
			do
			{
				iRet = connector->Recv ((LPVOID)recvMessage.Ptr (), 4096);
				readable.FromSJIS ((LPCSTR)recvMessage.Ctr ());
				Logger::WriteMessage (readable.Ctr ());
				if (iRet == 0) break;
			} while (true);
		}

		TEST_METHOD (GetHostByNameTest)
		{
			alt::SocketBuilder builder;
			builder.Startup ();

			alt::TString buff1 (80);
			BOOL ret = builder.GetHostByName (_T("localhost"), buff1.Ptr());
			Assert::IsTrue (ret, _T("SocketBuilder::GetHostByName() failed.\n"));
			Assert::AreEqual (_T ("127.0.0.1"), buff1.Ptr (), _T("SocketBuilder::GetHostByName() invalid response.\n"));
			buff1 += _T ("\n");
			Logger::WriteMessage (buff1.Ctr ());

			alt::TString buff2 (80);
			ret = builder.GetHostByName (_T ("jjy.nict.go.jp"), buff2.Ptr ());
			Assert::IsTrue (ret, _T("SocketBuilder::GetHostByName() failed.\n"));
			Assert::AreEqual (_T ("133.243.3.26"), buff2.Ptr (), _T("SocketBuilder::GetHostByName() invalid response.\n"));
			buff2 += _T ("\n");
			Logger::WriteMessage (buff2.Ctr ());
		}
	};
}