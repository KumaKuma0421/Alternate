//! @file	ArrayTest.cpp
//! @brief	可変配列に関するクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.2.24

#include "pch.h"
#include "Array.hpp"
#include "HeapMemory.h"
#include "VirtualMemory.h"
#include "Freight.hpp"
#include "Binder.hpp"
#include "TString.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (ArrayTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("ArrayTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("ArrayTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("ArrayTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("ArrayTest method cleanup.\n");
		}

		typedef struct _Freight1
		{
			int _index = 0;
			char _message[MAX_PATH]{ 0 };
			char _guid[40]{ 0 };
			bool isRemain = 0;
		} Freight1;

		typedef std::function<void (Freight1&, int, bool)> MakeFreight1;
		MakeFreight1 makeFreight1 = [](Freight1& freight, int i, bool isRemain)->void
		{
			alt::TString buffer;
			freight._index = i;
			buffer = alt::Utility::GetFormatTime (alt::Utility::GetLocalTime ());
			buffer.ToSJIS (freight._message, MAX_PATH);
			buffer = alt::Utility::MakeGUID ();
			buffer.ToSJIS (freight._guid, 40);
			freight.isRemain = isRemain;
		};

		typedef std::function<void (Freight1&)> ShowFreight1;
		ShowFreight1 showFreight1 = [](Freight1& freight)
		{
			alt::TString output;
			output.Format (_T ("%2d:%S %S isRemain=%d\n"),
						   freight._index, freight._message, freight._guid, freight.isRemain);
			Logger::WriteMessage (output.Ctr ());
		};

		typedef struct _Freight2
		{
			int _index = 0;
			char _message[MAX_PATH]{ 0 };
			alt::TString _guid;
			bool isRemain = 0;
		} Freight2;

		typedef std::function<void (Freight2&, int, bool)> MakeFreight2;
		MakeFreight2 makeFreight2 = [](Freight2& freight, int i, bool isRemain)->void
		{
			freight._index = i;
			alt::TString message =
				alt::Utility::GetFormatTime (alt::Utility::GetLocalTime ());
			message.ToSJIS (freight._message, MAX_PATH);
			freight._guid = alt::Utility::MakeGUID ();
			freight.isRemain = isRemain;
		};

		typedef std::function<void (Freight2&)> ShowFreight2;
		ShowFreight2 showFreight2 = [](Freight2& freight)
		{
			alt::TString output;
			output.Format (_T ("%2d:%S %s isRemain=%d\n"),
						   freight._index, freight._message, freight._guid.Ctr (), freight.isRemain);
			Logger::WriteMessage (output.Ctr ());
		};

		typedef alt::skeleton::Freight<TCHAR> Freight3;

		typedef std::function<void (Freight3&)> MakeFreight3;
		MakeFreight3 makeFreight3 = [](Freight3& freight)->void
		{
			alt::TString message =
				alt::Utility::GetFormatTime (alt::Utility::GetLocalTime ());
			freight.SetData (message.Ptr (), message.Byte ());
		};

		typedef std::function<void (Freight3&)> ShowFreight3;
		ShowFreight3 showFreight3 = [](Freight3& freight)
		{
			alt::TString output;
			output.Format (_T ("%s[%d]\n"), freight.GetData (), freight.GetSize ());
			Logger::WriteMessage (output.Ctr ());
		};

		class Freight4 : public Freight3
		{
		public:
			Freight4 ()
				:Freight3 ()
			{
				_index = -1;
				_isRemain = false;
			};

			Freight4 (const Freight4& base)
				:Freight3 ((Freight3)base)
			{
				_index = base._index;
				_isRemain = base._isRemain;
				_guid = base._guid;
			};

			~Freight4 ()
			{
				_index = -1;
				_isRemain = false;
			};

			void GetData (
				INT& index, LPTSTR& message, size_t& size, bool& isRemain, alt::TString& guid)
			{
				message = Freight3::GetData ();
				size = Freight3::GetSize ();
				index = _index;
				guid = _guid;
				isRemain = _isRemain;
			};

			void SetData (
				const INT index, LPTSTR message, const int size, const bool isRemain, const alt::TString& guid)
			{
				Freight3::SetData (message, size);
				_index = index;
				_guid = guid;
				_isRemain = isRemain;
			};

			void operator = (const Freight4& base)
			{
				Freight3::operator=((Freight3)base);
				_index = base._index;
				_guid = base._guid;
				_isRemain = base._isRemain;
			}

		private:
			int _index;
			alt::TString _guid;
			bool _isRemain;
		};

		typedef std::function<void (Freight4&, int, bool)> MakeFreight4;
		MakeFreight4 makeFreight4 = [](Freight4& freight, int index, bool isRemain)->void
		{
			alt::TString message =
				alt::Utility::GetFormatTime (alt::Utility::GetLocalTime ());
			alt::TString guid = alt::Utility::MakeGUID ();
			freight.SetData (index, message.Ptr (), message.Byte (), isRemain, guid);
		};

		typedef std::function<void (Freight4&)> ShowFreight4;
		ShowFreight4 showFreight4 = [](Freight4& freight)
		{
			int index;
			size_t size;
			LPTSTR lptszMessage = nullptr;
			bool isRemain;
			alt::TString guid;
			freight.GetData (index, lptszMessage, size, isRemain, guid);

			alt::TString output;
			output.Format (_T ("%2d:%s %s isRemain=%d\n"),
						   index, lptszMessage, guid.Ctr (), isRemain);
			Logger::WriteMessage (output.Ctr ());
		};

		TEST_METHOD (VectorFreightTest1)
		{
			alt::TString message;

			message.Format (_T ("sizeof(Freight1)=%d\n"), sizeof (Freight1));
			Logger::WriteMessage (message.Ctr ());
			message.Format (_T ("sizeof(Freight2)=%d\n"), sizeof (Freight2));
			Logger::WriteMessage (message.Ctr ());
			message.Format (_T ("sizeof(Freight3)=%d\n"), sizeof (Freight3));
			Logger::WriteMessage (message.Ctr ());
			message.Format (_T ("sizeof(Freight4)=%d\n"), sizeof (Freight4));
			Logger::WriteMessage (message.Ctr ());

			{
				std::vector<Freight1> vector;
				Freight1 freight;
				makeFreight1 (freight, 128, false);
				vector.push_back (freight);
				auto& val1 = vector.front ();
				showFreight1 (val1);
				Freight1 val2 = vector.front ();
				showFreight1 (val2);
			}

			{
				std::vector<Freight2> vector;
				Freight2 freight;
				makeFreight2 (freight, 128, false);
				vector.push_back (freight);
				auto& val1 = vector.front ();
				showFreight2 (val1);
				Freight2 val2 = vector.front ();
				showFreight2 (val2);
			}

			{
				std::vector<Freight3> vector;
				Freight3 freight;
				makeFreight3 (freight);
				vector.push_back (freight);
				auto& val1 = vector.front ();
				showFreight3 (val1);
				Freight3 val2 = vector.front ();
				showFreight3 (val2);
			}

			{
				std::vector<Freight4> vector;
				Freight4 freight;
				makeFreight4 (freight, 128, false);
				vector.push_back (freight);
				auto& val1 = vector.front ();
				showFreight4 (val1);
				Freight4 val2 = vector.front ();
				showFreight4 (val2);
			}
		}

		TEST_METHOD (ArrayTest1BA)
		{
			typedef alt::skeleton::Array<Freight1> Array;
			Array array1;

			for (int i = 0; i < 10; i++)
			{
				Freight1 freight;
				makeFreight1 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight1 freight;
					makeFreight1 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight1 freight : array3)
					{
						showFreight1 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight1 freight : array2)
				{
					showFreight1 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight1& freight : array1)
			{
				showFreight1 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest2BA)
		{
			typedef alt::skeleton::Array<Freight2> Array;
			Array array1;

			for (int i = 0; i < 10; i++)
			{
				Freight2 freight;
				makeFreight2 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight2 freight;
					makeFreight2 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight2 freight : array3)
					{
						showFreight2 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight2 freight : array2)
				{
					showFreight2 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight2& freight : array1)
			{
				showFreight2 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest3BA)
		{
			typedef alt::skeleton::Array<Freight3> Array;
			Array array1;

			for (int i = 0; i < 10; i++)
			{
				Freight3 freight;
				makeFreight3 (freight);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight3 freight;
					makeFreight3 (freight);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight3 freight : array3)
					{
						showFreight3 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight3 freight : array2)
				{
					showFreight3 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight3& freight : array1)
			{
				showFreight3 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest4BA)
		{
			typedef alt::skeleton::Array<Freight4> Array;
			Array array1;

			for (int i = 0; i < 10; i++)
			{
				Freight4 freight;
				makeFreight4 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight4 freight;
					makeFreight4 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight4 freight : array3)
					{
						showFreight4 (freight);
					}
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight4 freight : array2)
				{
					showFreight4 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight4& freight : array1)
			{
				showFreight4 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest1HO)
		{
			alt::skeleton::Binder<alt::HeapMemory> memory;
			Assert::IsTrue (memory.New ());
			Assert::IsTrue (memory->Open ());

			typedef alt::skeleton::Array<Freight1> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight1 freight;
				makeFreight1 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight1 freight;
					makeFreight1 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight1 freight : array3)
					{
						showFreight1 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight1 freight : array2)
				{
					showFreight1 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight1& freight : array1)
			{
				showFreight1 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest2HO)
		{
			alt::skeleton::Binder<alt::HeapMemory> memory;
			Assert::IsTrue (memory.New ());
			Assert::IsTrue (memory->Open ());

			typedef alt::skeleton::Array<Freight2> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight2 freight;
				makeFreight2 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight2 freight;
					makeFreight2 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight2 freight : array3)
					{
						showFreight2 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight2 freight : array2)
				{
					showFreight2 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight2& freight : array1)
			{
				showFreight2 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest3HO)
		{
			alt::skeleton::Binder<alt::HeapMemory> memory;
			Assert::IsTrue (memory.New ());
			Assert::IsTrue (memory->Open ());

			typedef alt::skeleton::Array<Freight3> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight3 freight;
				makeFreight3 (freight);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight3 freight;
					makeFreight3 (freight);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight3 freight : array3)
					{
						showFreight3 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight3 freight : array2)
				{
					showFreight3 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight3& freight : array1)
			{
				showFreight3 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest4HO)
		{
			alt::skeleton::Binder<alt::HeapMemory> memory;
			Assert::IsTrue (memory.New ());
			Assert::IsTrue (memory->Open ());

			typedef alt::skeleton::Array<Freight4> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight4 freight;
				makeFreight4 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight4 freight;
					makeFreight4 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight4 freight : array3)
					{
						showFreight4 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight4 freight : array2)
				{
					showFreight4 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight4& freight : array1)
			{
				showFreight4 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest1HC)
		{
			alt::skeleton::Binder<alt::HeapMemory> memory;
			Assert::IsTrue (memory.New ());
			Assert::IsTrue (memory->Create ());

			typedef alt::skeleton::Array<Freight1> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight1 freight;
				makeFreight1 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight1 freight;
					makeFreight1 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight1 freight : array3)
					{
						showFreight1 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight1 freight : array2)
				{
					showFreight1 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight1& freight : array1)
			{
				showFreight1 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest2HC)
		{
			alt::skeleton::Binder<alt::HeapMemory> memory;
			Assert::IsTrue (memory.New ());
			Assert::IsTrue (memory->Create ());

			typedef alt::skeleton::Array<Freight2> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight2 freight;
				makeFreight2 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight2 freight;
					makeFreight2 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight2 freight : array3)
					{
						showFreight2 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight2 freight : array2)
				{
					showFreight2 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight2& freight : array1)
			{
				showFreight2 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest3HC)
		{
			alt::skeleton::Binder<alt::HeapMemory> memory;
			Assert::IsTrue (memory.New ());
			Assert::IsTrue (memory->Create ());

			typedef alt::skeleton::Array<Freight3> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight3 freight;
				makeFreight3 (freight);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight3 freight;
					makeFreight3 (freight);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight3 freight : array3)
					{
						showFreight3 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight3 freight : array2)
				{
					showFreight3 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight3& freight : array1)
			{
				showFreight3 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest4HC)
		{
			alt::skeleton::Binder<alt::HeapMemory> memory;
			Assert::IsTrue (memory.New ());
			Assert::IsTrue (memory->Create ());

			typedef alt::skeleton::Array<Freight4> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight4 freight;
				makeFreight4 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight4 freight;
					makeFreight4 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight4 freight : array3)
					{
						showFreight4 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight4 freight : array2)
				{
					showFreight4 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight4& freight : array1)
			{
				showFreight4 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest1V1)
		{
			alt::skeleton::Binder<alt::VirtualMemory1> memory;
			Assert::IsTrue (memory.New ());

			typedef alt::skeleton::Array<Freight1> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight1 freight;
				makeFreight1 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight1 freight;
					makeFreight1 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight1 freight : array3)
					{
						showFreight1 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight1 freight : array2)
				{
					showFreight1 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight1& freight : array1)
			{
				showFreight1 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest2V1)
		{
			alt::skeleton::Binder<alt::VirtualMemory1> memory;
			Assert::IsTrue (memory.New ());

			typedef alt::skeleton::Array<Freight2> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight2 freight;
				makeFreight2 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight2 freight;
					makeFreight2 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight2 freight : array3)
					{
						showFreight2 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight2 freight : array2)
				{
					showFreight2 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight2& freight : array1)
			{
				showFreight2 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest3V1)
		{
			alt::skeleton::Binder<alt::VirtualMemory1> memory;
			Assert::IsTrue (memory.New ());

			typedef alt::skeleton::Array<Freight3> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight3 freight;
				makeFreight3 (freight);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight3 freight;
					makeFreight3 (freight);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight3 freight : array3)
					{
						showFreight3 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight3 freight : array2)
				{
					showFreight3 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight3& freight : array1)
			{
				showFreight3 (freight);
			}

			array1.Clear ();
		}

		TEST_METHOD (ArrayTest4V1)
		{
			alt::skeleton::Binder<alt::VirtualMemory1> memory;
			Assert::IsTrue (memory.New ());

			typedef alt::skeleton::Array<Freight4> Array;
			Array array1 (memory.Get ());

			for (int i = 0; i < 10; i++)
			{
				Freight4 freight;
				makeFreight4 (freight, i, true);
				array1.Add (freight);
			}

			for (int i = 0; i < 5; i++)
			{
				array1.Remove (5);
			}

			Array array2;

			{
				for (int i = 0; i < 10; i++)
				{
					Freight4 freight;
					makeFreight4 (freight, i, true);
					array2.Add (freight);
				}

				for (int i = 0; i < 5; i++)
				{
					array2.Remove (0);
				}

				{
					Array array3 = array1;
					array3 += array2;

					Logger::WriteMessage ("----------------------------------------\n");
					Assert::AreEqual<SIZE_T> (10, array3.Size ());
					for (Freight4 freight : array3)
					{
						showFreight4 (freight);
					}

					array3.Clear ();
				}

				Logger::WriteMessage ("----------------------------------------\n");
				Assert::AreEqual<SIZE_T> (5, array2.Size ());
				for (Freight4 freight : array2)
				{
					showFreight4 (freight);
				}

				array2.Clear ();
			}

			Logger::WriteMessage ("----------------------------------------\n");
			Assert::AreEqual<SIZE_T> (5, array1.Size ());
			for (Freight4& freight : array1)
			{
				showFreight4 (freight);
			}

			array1.Clear ();
		}
	};
}