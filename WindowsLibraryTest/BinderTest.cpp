//! @file	BinderTest.cpp
//! @brief	newオブジェクトを管理するコンテナクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#include "pch.h"
#include "Binder.hpp"
#include "Freight.hpp"
#include "Exception.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (BinderTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("BinderTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("BinderTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("BinderTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("BinderTest method cleanup.\n");
		}

		class Rectangle
		{
		public:
			Rectangle ()
			{
				SetRect (-1, -1, -1, -1);
			};

			Rectangle (int x, int y, int width, int height)
			{
				SetRect (x, y, width, height);
			};

			~Rectangle ()
			{
				SetRect (-1, -1, -1, -1);
			};

			int GetArea ()
			{
				return _width * _height;
			};

			void GetRect (int& x, int& y, int& width, int& height)
			{
				x = _x;
				y = _y;
				width = _width;
				height = _height;
			}

			void SetRect (int x, int y, int width, int height)
			{
				_x = x;
				_y = y;
				_width = width;
				_height = height;
			};

		private:
			int _x;
			int _y;
			int _width;
			int _height;
		};

		TEST_METHOD (BinderTest1)
		{
			try
			{
				alt::skeleton::Binder<Rectangle> binder;
				Rectangle* rectangle = binder.Get ();
				int area = rectangle->GetArea ();
				Assert::Fail (_T ("RuntimeException not raised.\n"));
			}
			catch (const alt::ex::RuntimeException& ex)
			{
				Logger::WriteMessage (ex.GetReason ());
			}
			catch (...)
			{
				Assert::Fail (_T ("unknown exception raised.\n"));
			}
		}

		TEST_METHOD (BinderTest2)
		{
			Rectangle* rectangle = new Rectangle (1, 1, 10, 20);
			alt::skeleton::Binder<Rectangle> binder;
			binder.Set (rectangle);
			rectangle = nullptr; // no problem. Binder will clean it safely.

			Rectangle* newRectangle = binder.Get ();
			Assert::AreEqual<int> (200, newRectangle->GetArea (), _T ("invalid value GetArea().\n"));
		}

		TEST_METHOD (BinderTest3)
		{
			Rectangle* rectangle = new Rectangle (1, 1, 20, 20);
			alt::skeleton::Binder<Rectangle> binder(rectangle);
			rectangle = nullptr; // no problem. Binder will clean it safely.

			Rectangle* newRectangle = binder.Get ();
			Assert::AreEqual<int> (400, newRectangle->GetArea (), _T ("invalid value GetArea().\n"));
		}
		
		TEST_METHOD (BinderTest4)
		{
			alt::skeleton::Binder<Rectangle> binder;

			Assert::IsTrue (binder.New (), _T ("Biner::New() failed.\n"));
			Rectangle* rectangle = binder.Get ();
			rectangle->SetRect (10, 10, 10, 10);

			int area = binder->GetArea ();
			Assert::AreEqual<int> (100, area, _T ("invalid value GetArea().\n"));
		}

		TEST_METHOD (BinderTest5)
		{
			typedef alt::skeleton::Binder<Rectangle, int, int, int, int> Binder;
			Binder binder;

			Assert::IsTrue (binder.New (10, 10, 10, 10), _T ("Biner::New(10,10,10,10) failed.\n"));

			int area = binder->GetArea ();
			Assert::AreEqual<int> (100, area, _T ("invalid value GetArea().\n"));

			int area2 = binder->GetArea ();
		}
	};
}