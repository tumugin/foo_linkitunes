#include <windows.h>
#include <string>
using namespace std;
#include "main.h"
#include "StringUtils.h"
#include "prochelper.h"
DECLARE_COMPONENT_VERSION("foo_linkitunes", "1.0.0", "iTunes Client Ver1.0")

//on foobar2000 initialize
void ApplicationLifecycle::on_init(){
	setlocale(LC_ALL, "japanese");
	lcm = this;
	console::info("foo_linkitunes initializing...");
	CoInitialize(NULL);
	//start thread
	//initCOMObjects();
	_beginthread(iTunesProcessWatcher, 0, NULL);
}

//on foobar2000 exit
void ApplicationLifecycle::on_quit(){
	console::info("foo_linkitunes destroying...");
	if (iITunes != NULL){
		iITunes->Release();
		lcm->releaseCOMObjects();
	}
	CoUninitialize();
}

void ApplicationLifecycle::initCOMObjects(){
	HRESULT hRes = ::CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *)&iITunes);
	if (hRes != S_FALSE){
		console::info("Link to iTunes OK.");
		IConnectionPointContainer* icpc;
		iITunes->QueryInterface(IID_IConnectionPointContainer, (void **)&icpc);
		icpc->FindConnectionPoint(DIID__IiTunesEvents, &icp);
		icpc->Release();
		events = new CITunesEventSink();
		events->QueryInterface(IID_IUnknown, (void **)&pSinkUnk);
		DWORD dwAdvise;
		icp->Advise(pSinkUnk, &dwAdvise);
		//ŠÖ”ƒ|ƒCƒ“ƒ^
		events->OnEvent = &IventReceivedCallback;
		console::info("Register to iTunes OK.");
	}
}

void ApplicationLifecycle::releaseCOMObjects(){
	icp->Release();
	pSinkUnk->Release();
	//delete events;
	console::info("destroy com object OK");
}

void IventReceivedCallback(long ev){
	std::string str;
	static_api_ptr_t<playback_control> pc;
	switch (ev)
	{
	case 3:
		console::info("iTunes play stopped");
		pc->pause(true);
		break;
	case 1:
		console::info("iTunes song changed");
		BSTR path;
		IITTrack *track;
		iITunes->get_CurrentTrack(&track);
		if (track == NULL) return;
		IITFileOrCDTrack *filetrack;
		track->QueryInterface(&filetrack);
		filetrack->get_Location(&path);
		//convert
		str = ConvertBSTRToMBS(path);
		console::info(str.c_str());
		
		//release
		track->Release();
		filetrack->Release();
		break;
	case 9:
		console::info("iTunes exit.");
		iITunes->Release();
		lcm->releaseCOMObjects();
		iITunes = NULL;
		_beginthread(WaitUntiliTunesProcessDie, 0, NULL);
		break;
	default:
		string str = "itunes unknown code=";
		str += to_string(ev);
		console::info(str.c_str());
		//console::info(&t);
		break;
	}
}

void iTunesProcessWatcher(LPVOID pParam){
	console::info("waiting for itunes start.");
	while (1){
		const auto list = ::getModuleList();
		for (auto name : list) {
			//char wStrC[50];
			//wcstombs_s(0, wStrC, 20, name.c_str(), _TRUNCATE);
			//console::info(wStrC);
			if (name.compare(_T("iTunes.exe")) == 0){
				//found itunes proc
				console::info("found iTunes.exe");
				service_impl_t<RunOnMainThread>* thread_cb = new service_impl_t<RunOnMainThread>();
				static_api_ptr_t<main_thread_callback_manager> cb_mgr;
				cb_mgr->add_callback(thread_cb);
				return;
			}
		}
		Sleep(1000);
	}
}

void WaitUntiliTunesProcessDie(LPVOID pParam){
	console::info("waiting for itunes die.");
	while (1){
		const auto list = ::getModuleList();
		bool itunesAlive = false;
		for (auto name : list) {
			if (name.compare(_T("iTunes.exe")) == 0){
				itunesAlive = true;
			}
		}
		if (!itunesAlive){
			//dead then start watcher
			console::info("itunes dead.");
			_beginthread(iTunesProcessWatcher, 0, NULL);
			return;
		}
	}
}

void RunOnMainThread::callback_run(){
	lcm->initCOMObjects();
}