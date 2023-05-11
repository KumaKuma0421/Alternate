#pragma once

#include "IFactoryMethod.h"
#include "FunctionPointers.hpp"
#include "WatchDogTimer.h"
#include "CriticalSection.h"
#include "TString.h"
#include "Event.h"
#include "Thread.h"
#include "Log.h"

#define SECTION_PIPELINE_FACTORY    _T("PipelineFactory")
#define KEYWORD_INTERVAL            _T("Interval")
#define KEYWORD_DUMMY_PROCESS_TIME	_T("DummyProcessTime")

namespace alt
{
	namespace FactoryMethod
	{
		// -- ByteData ------------------------------------------------------

		class DLL_DECLSPEC ByteData : public IData
		{
		public:
			ByteData (int seqNo, LPBYTE lpbyChunkData, size_t chunkSize);
			virtual ~ByteData ();

			LPBYTE GetChunk () { return _lpbyChunkData; };
			size_t GetChunkSize () { return _chunkSize; };

		protected:
			ByteData ();

		private:
			LPBYTE _lpbyChunkData;
			size_t _chunkSize;
		};

		// -- Config ---------------------------------------------------------

		class DLL_DECLSPEC Config : public IConfig
		{
		public:
			Config ();
			virtual ~Config ();

			virtual bool Load (LPCTSTR lpctszConfigPath);
			virtual bool Reload ();

			int GetInterval () { return _interval; };
			int GetDummyProcessTime () { return _dummyProcessTime; };

		private:
			TString _configPath;
			int _interval;
			int _dummyProcessTime;
		};

		// -- ClockElement ---------------------------------------------------

		class ClockElement : public IElement, public skeleton::ICallback
		{
		public:
			ClockElement (TString elementName);
			virtual ~ClockElement ();

			virtual void Update (IData* data) {};
			virtual void Init ();
			virtual void Start (DWORD dwInterval);
			virtual void Stop ();
			virtual DWORD Invoke (LPVOID lpvParam);

		protected:
			ClockElement ();

		private:
			int _seqNo;
			bool _stop;
			WatchDogTimer* _watchdog;
			Thread* _thread;
			DWORD _dwInterval;
			Log _logger;
		};

		// -- SyncElement ---------------------------------------------------

		class SyncElement : public IElement
		{
		public:
			SyncElement (TString elementName);
			virtual ~SyncElement ();

			virtual void Update (IData* data);

		protected:
			SyncElement ();
		};

		// -- QueueElement --------------------------------------------------

		class QueueElement : public IElement, public skeleton::ICallback
		{
		public:
			QueueElement (TString elementName);
			virtual ~QueueElement ();

			virtual void Setup ();
			virtual void Update (IData* data);
			virtual DWORD Invoke (LPVOID lpvParam);

		protected:
			std::queue<IData*> _queue;
			CriticalSection _exclusive;
			Event _event;
			Thread* _thread;
		};

		// -- TerminateElement ----------------------------------------------

		class TerminateElement : public IElement
		{
		public:
			TerminateElement (TString elementName);
			virtual ~TerminateElement ();

			virtual void Update (IData* data);

		protected:
			TerminateElement ();
			Log _logger;
		};

		// -- DummyProduct --------------------------------------------------

		class DummyProduct : public IProduct
		{
		public:
			DummyProduct (int id, TString productName);

			virtual bool Init (IConfig* config);
			virtual bool Start ();
			virtual IData* Process (IData* data);
			virtual bool Stop ();
			virtual bool Exit ();

			virtual void Update (IMessage* message);

		protected:
			DummyProduct ();
			Config* _config;
		};

		// -- Creator -------------------------------------------------------

		class Creator : public ICreator
		{
		public:
			Creator ();
			virtual ~Creator ();

			virtual IProduct* CreateProduct (int id, TString target, TString fullName);
			virtual IElement* CreateElement (TString target, TString fullName);
		};

		// -- ProductManager ------------------------------------------------

		class ProductManager : public IProductManager
		{
		public:
			ProductManager ();
			virtual ~ProductManager ();

			virtual bool Init (IConfig* config);
			virtual bool Start ();
			virtual bool Stop ();
			virtual bool Exit ();

			virtual void Update (IMessage* message);

			virtual void AddProduct (TString name, IProduct* product);
			virtual IProduct* GetProduct (TString name);
			virtual void AddElement (TString name, IElement* element);
			virtual IElement* GetElement (TString name);

		protected:

		private:
			std::map<TString, IProduct*> _products;
			std::map<TString, IElement*> _elements;
			IConfig* _config;
		};

		// -- PipelineFactoryMethod -----------------------------------------

		class DLL_DECLSPEC PipelineFactoryMethod : public IFactoryMethod
		{
		public:
			PipelineFactoryMethod () {};
			virtual ~PipelineFactoryMethod () {};

			virtual IProductManager* Buildup ();
		};
	}
}