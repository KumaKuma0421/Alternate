#include "pch.h"
#include "PipelineFactoryMethod.h"

using namespace alt;
using namespace alt::FactoryMethod;

#define MAX_PRODUCT 10
#define CONTAINER_WEIGHT (1024 * 1024 * 10) // 10MB

typedef struct _container
{
	DWORD ProcessStartTick[MAX_PRODUCT];
	double ProcessTick[MAX_PRODUCT];
	BYTE byDummy[CONTAINER_WEIGHT];
} Container, * pContainer;

// -- ByteData ------------------------------------------------------

ByteData::ByteData ()
{
	_lpbyChunkData = nullptr;
	_chunkSize = 0;
}

ByteData::ByteData (int seqNo, LPBYTE lpbyChunkData, size_t chunkSize)
	:IData (seqNo)
{
	_lpbyChunkData = new BYTE[chunkSize];
	CopyMemory (_lpbyChunkData, lpbyChunkData, chunkSize);
	_chunkSize = chunkSize;
}

ByteData::~ByteData ()
{
	if (_lpbyChunkData != nullptr)
	{
		delete[] _lpbyChunkData;
	}
}

// -- Config =------------------------------------------------------

Config::Config ()
	:IConfig ()
{
	_interval = -1;
	_dummyProcessTime = -1;
}

Config::~Config ()
{

}

bool Config::Load (LPCTSTR lpctszConfigPath)
{
	_configPath = lpctszConfigPath;

	_interval = ::GetPrivateProfileInt (
		SECTION_PIPELINE_FACTORY,
		KEYWORD_INTERVAL,
		-1,
		_configPath.Ctr ());
	if (_interval == -1)
	{
		return false;
	}

	_dummyProcessTime = ::GetPrivateProfileInt (
		SECTION_PIPELINE_FACTORY,
		KEYWORD_DUMMY_PROCESS_TIME,
		-1,
		_configPath.Ctr ());
	if (_dummyProcessTime == -1)
	{
		return false;
	}

	return true;
}

bool Config::Reload ()
{
	int _interval = ::GetPrivateProfileInt (
		SECTION_PIPELINE_FACTORY,
		KEYWORD_INTERVAL,
		0,
		_configPath.Ctr ());
	if (_interval == 0)
	{
		return false;
	}

	int _dummyProcessTime = ::GetPrivateProfileInt (
		SECTION_PIPELINE_FACTORY,
		KEYWORD_DUMMY_PROCESS_TIME,
		-1,
		_configPath.Ctr ());
	if (_interval == -1)
	{
		return false;
	}

	return true;
}

// -- ClockElement -------------------------------------------------

ClockElement::ClockElement (TString elementName)
	:IElement (elementName)
{
	_seqNo = 0;
	_stop = false;
	_watchdog = nullptr;
	_thread = nullptr;
	_dwInterval = 0;
	_logger.Init (LogLevel::Trace, elementName.Ctr (), 10, 1024 * 1024 * 10);
}

ClockElement::~ClockElement ()
{

}

ClockElement::ClockElement ()
{
	_seqNo = 0;
	_stop = false;
	_watchdog = nullptr;
	_thread = nullptr;
	_dwInterval = 0;
}

void ClockElement::Init ()
{
	_logger.Info (_T ("ClockElement::Init()"));

	_watchdog = new WatchDogTimer (nullptr);
	_watchdog->InitTimer (this->GetName ().Ctr ());
	_thread = new Thread (this);
}

void ClockElement::Start (DWORD dwInterval)
{
	_logger.Info (_T ("ClockElement::Start()"));
	OutputDebugString (_T ("ClockElement::Start() IN.\n"));

	_stop = false;
	_dwInterval = dwInterval;
	_thread->Create ();

	OutputDebugString (_T ("ClockElement::Start() OUT.\n"));
}

void ClockElement::Stop ()
{
	_logger.Info (_T ("ClockElement::Stop()"));
	OutputDebugString (_T ("ClockElement::Stop() IN.\n"));

	_watchdog->StopTimer ();
	_stop = true;

	OutputDebugString (_T ("ClockElement::Stop() OUT.\n"));
}

DWORD ClockElement::Invoke (LPVOID lpvParam)
{
	_logger.Info (_T ("ClockElement::Invoke()"));
	OutputDebugString (_T ("ClockElement::Invoke() IN.\n"));

	pContainer container = new Container;
	QueryPerformance Q;
	double dPastTick = 0;
	TString msg;

	_watchdog->StartTimer (_dwInterval);

	while (!_stop)
	{
		Q.Start ();
		DWORD dwReason = _watchdog->WaitTimer ();

		if (dwReason == WAIT_OBJECT_0)
		{
			ByteData* byteData = new ByteData (_seqNo++, (LPBYTE)container, sizeof (Container));
			SYSTEMTIME& localTime = byteData->GetCreateTime ();

			GetLocalTime (&localTime);
			Notify (byteData);

			Q.Stop ();
			dPastTick = Q.PastTime () * 1000;

			msg.Format (_T ("ClockTime %3u Past=%5u Diff=%7d %02d:%02d:%02d.%03d"),
					   byteData->GetSeqNo (), static_cast<UINT>(dPastTick),
					   static_cast<INT>(dPastTick - (double)_dwInterval * 1000),
					   localTime.wHour, localTime.wMinute, localTime.wSecond,
					   localTime.wMilliseconds);
			OutputDebugString (msg.Ctr ());
			_logger.Info (msg.Ctr ());
		}
	}

	delete container;

	OutputDebugString (_T ("ClockElement::Invoke() OUT.\n"));
	return 0;
}

// -- SyncElement -------------------------------------------------

SyncElement::SyncElement (TString elementName)
	:IElement (elementName)
{
	_product = nullptr;
}

SyncElement::~SyncElement ()
{

}

SyncElement::SyncElement ()
	:IElement ()
{
	_product = nullptr;
}

void SyncElement::Update (IData* data)
{
	IData* result = _product->Process (data);
	this->Notify (result);
}

// -- QueueElement ------------------------------------------------

QueueElement::QueueElement (TString elementName)
	:IElement (elementName)
{
	_product = nullptr;
	_thread = nullptr;
}

QueueElement::~QueueElement ()
{
	delete _thread;
}

void QueueElement::Setup ()
{
	_event.Create (this->GetName ().Ptr (), TRUE, FALSE);
	_thread = new Thread (this);
	_thread->Create ();
}

void QueueElement::Update (IData* data)
{
	_exclusive.Enter ();
	_queue.push (data);
	_event.Set (); //! @todo 本当は戻り値チェック
	_exclusive.Leave ();
}

DWORD QueueElement::Invoke (LPVOID lpvParam)
{
	Event event;
	event.Open (this->GetName ().Ctr ());

	for (;;)
	{
		event.Wait ();
		_exclusive.Enter ();

		if (!_queue.empty ())
		{
			IData* data = _queue.front ();
			_queue.pop ();
			IData* result = _product->Process (data);
			this->Notify (result);
		}

		event.Reset ();
		_exclusive.Leave ();
	}
}

// -- TerminateElement ----------------------------------------------

TerminateElement::TerminateElement (TString elementName)
	:IElement (elementName)
{
	_logger.Init (LogLevel::Trace, elementName.Ctr (), 10, 1024 * 1024 * 10);
}

TerminateElement::TerminateElement ()
{

}

TerminateElement::~TerminateElement ()
{}

//
// @sa https://ameblo.jp/sgl00044/entry-12463939983.html
//
void TerminateElement::Update (IData* data)
{
	OutputDebugString (_T ("TerminateElement::Update()"));

	ByteData* byteData = dynamic_cast<ByteData*>(data);
	pContainer container = (pContainer)(byteData->GetChunk ());

	TString message;

	for (int i = 0; i < MAX_PRODUCT; i++)
	{
		message.Format (_T ("Product(%u) Seq(%02u) Start(%u) ProcessTick(%u)"),
					   i, byteData->GetSeqNo (), container->ProcessStartTick[i],
					   static_cast<UINT>(container->ProcessTick[i]));

		OutputDebugString (message.Ctr ());
		_logger.Info (message.Ctr ());
	}

	delete data;
}

// -- DummyProduct --------------------------------------------------

DummyProduct::DummyProduct (int id, TString productName)
	:IProduct (id, productName)
{
	TString msg = _T ("DummyProduct::DummyProduct(");
	msg += productName;
	msg += _T (")\n");
	OutputDebugString (msg.Ctr ());

	_config = nullptr;
}

DummyProduct::DummyProduct ()
	:IProduct ()
{
	_config = nullptr;
}

bool DummyProduct::Init (IConfig* config)
{
	TString msg = _T ("DummyProduct::Init(");
	msg += _productName;
	msg += _T (")\n");
	OutputDebugString (msg.Ctr ());

	_config = dynamic_cast<Config*>(config);

	return true;
}

bool DummyProduct::Start ()
{
	TString msg = _T ("DummyProduct::Start(");
	msg += _productName;
	msg += _T (")\n");
	OutputDebugString (msg.Ctr ());

	return true;
}

IData* DummyProduct::Process (IData* data)
{
	QueryPerformance Q;
	Q.Start ();

	TString msg = _T ("DummyProduct::Process(");
	msg += _productName;
	msg += _T (")\n");
	OutputDebugString (msg.Ctr ());

	ByteData* byteData = dynamic_cast<ByteData*>(data);
	Container* container = (pContainer)(byteData->GetChunk ());

	Sleep (_config->GetDummyProcessTime ());

	if (data->GetSeqNo () % 10 == 0)
	{
		IMessage message;
		message.SetParam (0, this->GetID (), 0); //! @todo 暫定
		this->Notify (&message);
	}

	Q.Stop ();
	container->ProcessTick[_id] = Q.PastTime ();
	container->ProcessStartTick[_id] = Q.GetStartTick ().LowPart;
	return data;
}

bool DummyProduct::Stop ()
{
	TString msg = _T ("DummyProduct::Stop(");
	msg += _productName;
	msg += _T (")\n");
	OutputDebugString (msg.Ctr ());

	return true;
}

bool DummyProduct::Exit ()
{
	TString msg = _T ("DummyProduct::Exit(");
	msg += _productName;
	msg += _T (")\n");
	OutputDebugString (msg.Ctr ());

	return true;
}

void DummyProduct::Update (IMessage* message)
{
	TString msg = _T ("DummyProduct::Update(");
	msg += _productName;
	msg += _T (")\n");
	OutputDebugString (msg.Ctr ());

	TString msgString;
	msgString.Format (_T ("Response From(%d) To(%d) Command(%d)\n"),
					 message->GetFrom (), message->GetTo (), message->GetCommand ());
	OutputDebugString (msgString.Ctr ());
}

// -- Creator -------------------------------------------------------

Creator::Creator ()
{

}

Creator::~Creator ()
{

}

IProduct* Creator::CreateProduct (int id, TString target, TString fullName)
{
	IProduct* product;

	if (target.Substring (0, 5) == _T ("Dummy"))
	{
		product = new DummyProduct (id, fullName);
	}
	else
	{
		throw _T ("Invalid argument "), target.Ctr ();
	}

	return product;
}

IElement* Creator::CreateElement (TString target, TString fullName)
{
	IElement* element;

	if (target == _T ("Clock"))
	{
		element = new ClockElement (fullName);
	}
	else if (target == _T ("Sync"))
	{
		element = new SyncElement (fullName);
	}
	else if (target == _T ("Queue"))
	{
		element = new QueueElement (fullName);
	}
	else if (target == _T ("Terminate"))
	{
		element = new TerminateElement (fullName);
	}
	else
	{
		throw _T ("Invalid argument "), target.Ctr ();
	}

	return element;
}

// -- ProductManager ------------------------------------------------

ProductManager::ProductManager ()
{
	_config = nullptr;
}

ProductManager::~ProductManager ()
{

}

bool ProductManager::Init (IConfig* config)
{
	_config = config;

	for (auto product : _products)
	{
		product.second->Init (config);
	}

	ClockElement* clockElement = dynamic_cast<ClockElement*>(this->GetElement (_T ("Clock")));
	clockElement->Init ();

	return true;
}

bool ProductManager::Start ()
{
	bool ret;

	for (auto product : _products)
	{
		ret = product.second->Start ();
		if (ret == false)
		{
			return false;
		}
	}

	ClockElement* clockElement = dynamic_cast<ClockElement*>(this->GetElement (_T ("Clock")));
	clockElement->Start (dynamic_cast<Config*>(_config)->GetInterval ());

	return true;
}

bool ProductManager::Stop ()
{
	ClockElement* clockElement = dynamic_cast<ClockElement*>(this->GetElement (_T ("Clock")));
	clockElement->Stop ();

	for (auto product : _products)
	{
		product.second->Stop ();
	}

	return true;
}

bool ProductManager::Exit ()
{
	for (auto product : _products)
	{
		product.second->Exit ();
	}

	return true;
}

void ProductManager::Update (IMessage* message)
{
	TString msgString;
	msgString.Format (_T ("Response From(%d) To(%d) Command(%d)\n"),
					 message->GetFrom (), message->GetTo (), message->GetCommand ());
	OutputDebugString (msgString.Ctr ());
}

void ProductManager::AddProduct (TString name, IProduct* product)
{
	auto pair = std::pair<TString, IProduct*> (name, product);
	_products.insert (pair);
}

IProduct* ProductManager::GetProduct (TString name)
{
	return _products[name];
}

void ProductManager::AddElement (TString name, IElement* element)
{
	auto pair = std::pair<TString, IElement*> (name, element);
	_elements.insert (pair);
}

IElement* ProductManager::GetElement (TString name)
{
	return _elements[name];
}

// -- PipelineFactoryMethod -----------------------------------------

IProductManager* PipelineFactoryMethod::Buildup ()
{
	auto manager = new ProductManager ();
	auto creator = new Creator ();

	TString prefixNames[] = { _T ("TEST1_"), _T ("TEST2_"), _T ("TEST3_"), _T ("TEST4_"), _T ("TEST5_") };
	TString objectNames[] = { _T ("Dummy1"), _T ("Dummy2") };
	// MAX_PRODUCTになるように調整(現在10)

	IElement* previousElement = creator->CreateElement (_T ("Clock"), _T ("ClockGenerator"));
	manager->AddElement (_T ("Clock"), previousElement);

	TString fullName;
	int id = 0;

	for (TString prefixName : prefixNames)
	{
		for (TString objectName : objectNames)
		{
			fullName = prefixName + objectName;

			IProduct* product = creator->CreateProduct (id++, objectName, fullName);
			manager->AddProduct (fullName, product);
			manager->Add (product);
			product->Add (manager);

			IElement* element = creator->CreateElement (_T ("Queue"), fullName);
			manager->AddElement (fullName, element);

			element->SetProduct (product);
			element->Setup ();

			previousElement->Add (element);
			previousElement = element;
		}

	}

	IElement* terminateElement = creator->CreateElement (_T ("Terminate"), _T ("Terminater"));
	previousElement->Add (terminateElement);
	manager->AddElement (_T ("Terminate"), terminateElement);

	return manager;
}