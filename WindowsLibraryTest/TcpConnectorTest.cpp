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
			alt::SocketBuilder builder;
			Assert::AreEqual(0, builder.Startup (), _T("SocketBuilder::Startup() failed.\n"));

			alt::Thread* serverThread = new alt::Thread ();
			auto serverFunction = [](LPVOID lpvParam)->DWORD
			{
				Logger::WriteMessage ("[Server]Now start server thread.\n");

				alt::SocketBuilder* pBuilder = (alt::SocketBuilder*)lpvParam;
				Assert::IsTrue(pBuilder->Prepare (50000), _T("[Server]SocketBuilder::Prepare() failed.\n"));

				do
				{
					alt::TcpConnector* connector = pBuilder->Wait ();
					Assert::IsNotNull(connector, _T("[Server]SocketBuilder::Wait() failed.\n"));

					alt::TString message;
					LPCTSTR lpctszIPAddress = connector->GetAcceptedAddress ();
					USHORT wPort = connector->GetAcceptedPortNo ();
					message.Format (_T ("[Server]SocketBuilder::Wait() success. from %s:%d\n"), lpctszIPAddress, wPort);
					Logger::WriteMessage (message.Ctr());

					alt::Thread* workerThread = new alt::Thread ();
					auto connectorFunction = [](LPVOID lpvParam)->DWORD
					{
						alt::TcpConnector* connector = (alt::TcpConnector*)lpvParam;
						INT iPacketSize = -1;
						BYTE byBuf[256] = { 0 };
						alt::TString message;

						Logger::WriteMessage ("[ServerWorker]Now start worker thread.\n");

						INT ret;
						do
						{
							ret = connector->Recv ((LPVOID)&iPacketSize, sizeof (INT));
							if (ret == 0)
							{
								DWORD dwError = connector->GetErrNo ();
								message.Format (_T ("[ServerWorker]TcpConnector::Recv(1) return 0. [%d]\n"), dwError);
								Logger::WriteMessage (message.Ctr());
								break;
							}

							ret = connector->Recv (byBuf, iPacketSize);
							if (ret == 0)
							{
								DWORD dwError = connector->GetErrNo ();
								message.Format (_T ("[ServerWorker]TcpConnector::Recv(2) return 0. [%d]\n"), dwError);
								Logger::WriteMessage (message.Ctr());
								break;
							}

							message = (LPTSTR)byBuf;
							Logger::WriteMessage (message.Ctr());
						} while (1);
						
						Logger::WriteMessage ("[ServerWorker]Now finish worker thread.\n");
						return 0;
					};

					Assert::IsTrue(workerThread->Create (connectorFunction, connector), _T("[Server]Thread::Create() failed.\n"));
					Assert::AreEqual (WAIT_OBJECT_0, workerThread->Wait (), _T ("[Server]Thread::Wait() is not WAIT_OBJECT_0.\n"));
					Assert::AreEqual(0, connector->Close (), _T("[Server]TcpConnector::Close() failed.\n"));
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
				Assert::AreEqual(0, builder.Startup (),_T("[Client]SocketBuilder::Startup() failed.\n"));
				alt::TcpConnector* connector = builder.CreateTcpConnector (_T ("127.0.0.1"), 50000, 1, 10);
				Assert::IsNotNull (connector, _T("[Client]SocketBuilder::CreateTcpConnector() failed.\n"));
				
				alt::TString message;
				LPCTSTR lpctszIPAddress = connector->GetAcceptedAddress ();
				USHORT wPort = connector->GetAcceptedPortNo ();
				message.Format (_T ("[Client]SocketBuilder::CreateTcpConnector() success. from %s:%d\n"), lpctszIPAddress, wPort);
				Logger::WriteMessage (message.Ctr());

				INT iPacketSize;
				INT ret;
				CONST INT iLoop = 10;
				alt::TString debugString;

				for (int i = 1; i <= iLoop; i++)
				{
					alt::TString guid = alt::Utility::MakeGUID ();
					alt::TString time = alt::Utility::GetFormatTime (alt::Utility::GetLocalTime ());
					message.Format (_T("[%2d]%s %s\n"), i, time.Ctr(), guid.Ctr());
					iPacketSize = message.Byte();

					ret = connector->Send ((LPVOID)&iPacketSize, sizeof (INT));
					Assert::AreNotEqual (SOCKET_ERROR, ret, _T("[Client]TcpConnector::Send() failed.\n"));

					ret = connector->Send ((LPVOID)message.Ctr(), iPacketSize);
					Assert::AreNotEqual (SOCKET_ERROR, ret, _T("[Client]TcpConnector::Send() failed.\n"));

					Sleep (100);
				}

				Assert::AreEqual(0, connector->Close (), _T("[Client]TcpConnector::Close() failed.\n"));

				Logger::WriteMessage ("[Client]Now finish client thread.\n");
				return 0;
			};

			Assert::IsTrue(serverThread->Create (serverFunction, &builder), _T("serverThread.Create() failed.\n"));
			Sleep (1000);
			Assert::IsTrue(clientThread->Create (clientFunction, &builder), _T("clientThread.Create() failed.\n"));
			Sleep (1000);

			Assert::AreEqual(WAIT_OBJECT_0, clientThread->Wait (), _T("clientThread.Wait() failed.\n"));
			Logger::WriteMessage ("Client thread finished.\n");
			Assert::AreEqual(WAIT_OBJECT_0, serverThread->Wait (), _T("serverThread.Wait() failed.\n"));
			Logger::WriteMessage ("Server thread finished.\n");

			delete clientThread;
			delete serverThread;
			Logger::WriteMessage ("Finished.\n");
		}

		TEST_METHOD (TcpClientTest)
		{
			alt::SocketBuilder builder;
			BOOL ret;

			Assert::AreEqual(0, builder.Startup (), _T("SocketBuilder::Startup() failed.\n"));
			
			alt::TString buffer (80);
			ret = builder.GetHostByName (_T ("www.yahoo.co.jp"), buffer.Ptr ());
			Assert::IsTrue(ret, _T("SocketBuilder::GetHostByName() failed.\n"));

			alt::TcpConnector* connector = builder.CreateTcpConnector (buffer.Ctr(), 80, 1, 3);
			Assert::IsNotNull(connector, _T("SocketBuilder::CreateTcpConnector() failed.\n"));

			alt::TString sendMessage = _T ("GET / HTTP/1.1\n");
			sendMessage += _T ("Host: www.yahoo.co.jp\n");
			sendMessage += _T ("Connection: keep - alive\n");
			sendMessage += _T ("Upgrade - Insecure - Requests: 1\n");
			sendMessage += _T ("User - Agent: Mozilla / 5.0 (Macintosh; Intel Mac OS X 10_13_6) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 67.0.3396.99 Safari / 537.36\n");
			sendMessage += _T ("Accept: text / html, application / xhtml + xml, application / xml; q = 0.9, image / webp, image / apng, */*;q=0.8\n");
			sendMessage += _T ("Accept - Language: ja, en - US; q = 0.9, en; q = 0.8\n");
			
			INT iRet;
			iRet = connector->Send ((LPVOID)sendMessage.Ctr (), sendMessage.Len ());
			do
			{
				alt::TString recvMessage (4096);
				alt::TString readable (4096);
				iRet = connector->Recv ((LPVOID)recvMessage.Ptr (), 4096);
				readable.FromMultiByte ((LPCSTR)recvMessage.Ctr (), CP_UTF8);
				Logger::WriteMessage (readable.Ctr ());
				if (iRet == 0) break;
			} while (true);
		}

		TEST_METHOD (TcpClientTestNotExistHost)
		{
			alt::SocketBuilder builder;

			Assert::AreEqual(0, builder.Startup (), _T("SocketBuilder::Startup() failed.\n"));

			alt::TcpConnector* connector = builder.CreateTcpConnector (_T("127.0.0.1"), 80, 1, 3);
			Assert::IsNull(connector, _T("SocketBuilder::CreateTcpConnector() failed.\n"));
		}

		TEST_METHOD (GetHostByNameTest)
		{
			alt::SocketBuilder builder;
			Assert::AreEqual(0, builder.Startup (), _T("SocketBuilder::Startup() failed.\n"));

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

		TEST_METHOD (TcpWaitBlockCancelTest)
		{
			alt::SocketBuilder builder;
			Assert::AreEqual(0, builder.Startup (), _T("SocketBuilder::Startup() failed.\n"));

			alt::Thread* serverThread = new alt::Thread ();
			auto serverFunction = [](LPVOID lpvParam)->DWORD
			{
				Logger::WriteMessage ("[Server]Now start server thread.\n");

				alt::SocketBuilder* pBuilder = (alt::SocketBuilder*)lpvParam;
				Assert::IsTrue(pBuilder->Prepare (50000), _T("SocketBuilder::Prepare() failed.\n"));

				Logger::WriteMessage ("[Server]Now waiting.\n");
				alt::TcpConnector* connector = pBuilder->Wait ();
				Assert::IsNull(connector, _T("SocketBuilder::Wait() failed.\n"));

				Logger::WriteMessage ("[Server]Now finish server thread.\n");
				return 0;
			};

			Assert::IsTrue(serverThread->Create (serverFunction, &builder), _T("Thread::Create() failed.\n"));
			Sleep (1000);

			alt::Thread* clientThread = new alt::Thread ();
			auto clientFunction = [](LPVOID lpvParam)->DWORD
			{
				Logger::WriteMessage ("[Client]Now start client thread.\n");
				
				alt::SocketBuilder* pBuilder = (alt::SocketBuilder*)lpvParam;

				Sleep (5000);
				Logger::WriteMessage ("[Client]fire cancel action.\n");
				Assert::IsTrue(pBuilder->CancelWait ());

				Logger::WriteMessage ("[Client]Now finish client thread.\n");
				return 0;
			};

			Assert::IsTrue (clientThread->Create (clientFunction, &builder), _T ("Thread::Create() failed.\n"));
			Sleep (1000);

			Assert::AreEqual(WAIT_OBJECT_0, serverThread->Wait (60000), _T("Thread::Wait() failed.\n"));
			Logger::WriteMessage ("Server thread finished.\n");

			delete serverThread;
			Logger::WriteMessage ("Finished.\n");
		}

		TEST_METHOD (TcpRecvBlockCancelTest)
		{
			alt::Thread* serverThread = new alt::Thread ();
			alt::TcpConnector* connector = nullptr;

			auto serverFunction = [](LPVOID lpvParam)->DWORD
			{
				Logger::WriteMessage ("[Server]Now start server thread.\n");

				alt::TcpConnector* connector = (alt::TcpConnector*)lpvParam;

				alt::SocketBuilder builder;
				Assert::AreEqual(0, builder.Startup (), _T("SocketBuilder::Startup() failed.\n"));
				Assert::IsTrue(builder.Prepare (50000), _T("SocketBuilder::Prepare() failed.\n"));
				connector = builder.Wait ();
				Assert::IsNotNull(connector, _T("SocketBuilder::Wait() failed.\n"));
				
				BYTE byBuf[1024]{ 0 };

				Logger::WriteMessage ("[Server]Now waiting.\n");
				INT ret = connector->Recv (byBuf, sizeof (byBuf));
				Logger::WriteMessage ("[Server]Now unblocked.\n");

				Logger::WriteMessage ("[Server]Now finish server thread.\n");
				return 0;
			};

			alt::Thread* clientThread = new alt::Thread ();
			auto clientFunction = [](LPVOID lpvParam)->DWORD
			{
				Logger::WriteMessage ("[Client]Now start client thread.\n");

				alt::TcpConnector* counter = (alt::TcpConnector*)lpvParam;

				alt::SocketBuilder builder;
				Assert::AreEqual(0, builder.Startup (),_T("[Client]SocketBuilder::Startup() failed.\n"));
				alt::TcpConnector* connector = builder.CreateTcpConnector (_T ("127.0.0.1"), 50000, 1, 10);
				Assert::IsNotNull (connector, _T("[Client]SocketBuilder::CreateTcpConnector() failed.\n"));

				alt::TString message;
				LPCTSTR lpctszIPAddress = connector->GetAcceptedAddress ();
				USHORT wPort = connector->GetAcceptedPortNo ();
				message.Format (_T ("[Client]SocketBuilder::CreateTcpConnector() success. from %s:%d\n"), lpctszIPAddress, wPort);
				Logger::WriteMessage (message.Ctr());

				Sleep (5000); // 5 seconds.
				Logger::WriteMessage ("[Client]Now force unblocked by CancelRecv().\n");
				counter->CancelRecv ();

				Assert::AreEqual(0, connector->Close (), _T("[Client]TcpConnector::Close() failed.\n"));

				Logger::WriteMessage ("[Client]Now finish client thread.\n");
				return 0;
			};

			Assert::IsTrue(serverThread->Create (serverFunction, &connector), _T("serverThread.Create() failed.\n"));
			Sleep (1000);
			Assert::IsTrue(clientThread->Create (clientFunction, &connector), _T("clientThread.Create() failed.\n"));
			Sleep (1000);

			Assert::AreEqual(WAIT_OBJECT_0, clientThread->Wait (), _T("clientThread.Wait() failed.\n"));
			Logger::WriteMessage ("Client thread finished.\n");
			Assert::AreEqual(WAIT_OBJECT_0, serverThread->Wait (), _T("serverThread.Wait() failed.\n"));
			Logger::WriteMessage ("Server thread finished.\n");

			delete clientThread;
			delete serverThread;
			Logger::WriteMessage ("Finished.\n");
		}
	};
}