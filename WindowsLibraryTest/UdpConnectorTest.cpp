//! @file	UdpConnectorTest.cpp
//! @brief	UDP通信に関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.22

#include "pch.h"
#include "SocketBuilder.h"
#include "UdpConnector.h"
#include "Thread.h"
#include "TString.h"
#include "Binder.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (UdpConnectorTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("UdpConnectorTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("UdpConnectorTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("UdpConnectorTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("UdpConnectorTest method cleanup.\n");
		}

		class WorkerThread : public alt::Thread, public alt::skeleton::ICallback
		{
		public:
			WorkerThread (alt::UdpConnector* connector, int id)
			{
				_connector = connector;
				_id = id;
				_callback = this;
			};

			~WorkerThread ()
			{
				delete _connector;
			};

			BOOL SetTarget (LPCTSTR lpctszTargetIP, WORD wTargetPort)
			{
				return _connector->SetSockAddr (lpctszTargetIP, wTargetPort);
			};

			DWORD Invoke (LPVOID lpvParam)
			{
				alt::TString message;

				for (int i = 0; i < 10; i++)
				{
					if (_id == 1)
					{
						message.Format (_T ("WorkerThread(%d) %d\n"), _id, i);
						INT iSend = _connector->Send ((LPVOID)message.Ctr (), message.Byte ());
						Assert::AreEqual<int> (iSend, message.Byte (), _T ("send size different.\n"));

						alt::TString buffer (80);
						INT iRecv = _connector->Recv (buffer.Ptr(), 80);
						Logger::WriteMessage (buffer.Ctr ());
					}
					else
					{
						BYTE byBuffer[80]{ 0 };
						ZeroMemory (byBuffer, sizeof(byBuffer));
						INT iRecv =_connector->Recv (byBuffer, sizeof(byBuffer));
						byBuffer[iRecv] = '\0';
						Logger::WriteMessage ((LPCTSTR)byBuffer);

						alt::TString message (_T ("NEXT\n"));
						_connector->Send ((LPVOID)message.Ctr (), message.Byte ());
					}
				}

				return 0;
			}

		private:
			WorkerThread () = delete;

			int _id;
			alt::UdpConnector* _connector;
		};

		TEST_METHOD (UdpConnecorTest1)
		{
			alt::SocketBuilder builder;
			alt::skeleton::Binder<WorkerThread, alt::UdpConnector*, int> connector1;
			alt::skeleton::Binder<WorkerThread, alt::UdpConnector*, int> connector2;
			Assert::AreEqual(0, builder.Startup (), _T("SocketBuilder::Startup() failed.\n"));

			connector1.New (builder.CreateUdpConnector (_T ("127.0.0.1"), 9001), 1);
			Assert::IsTrue(connector1->SetTarget (_T ("127.0.0.1"), 9002), _T("Connector::SetTarget(1) failed.\n"));

			connector2.New (builder.CreateUdpConnector (_T ("127.0.0.1"), 9002), 2);
			Assert::IsTrue(connector2->SetTarget (_T ("127.0.0.1"), 9001), _T("Connector::SetTarget(2) failed.\n"));

			connector1->Create ();
			connector2->Create ();

			connector1->Wait ();
			connector2->Wait ();
		}
	};
}