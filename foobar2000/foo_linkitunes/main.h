#pragma once
#include "../SDK/foobar2000.h"
#include "../helpers/helpers.h"
#include "iTunesHeader.h"
#include "iTunesEvent.h"

class ApplicationLifecycle : public initquit{
public:
	virtual void on_init();
	virtual void on_quit();
	void releaseCOMObjects();
	void initCOMObjects();
	IConnectionPoint* icp;
	IUnknown *pSinkUnk;
	CITunesEventSink* events;
};
class RunOnMainThread : public main_thread_callback{
public:
	virtual void callback_run();
};
static initquit_factory_t < ApplicationLifecycle > lifecycle;
IiTunes *iITunes;
void IventReceivedCallback(long ev);
ApplicationLifecycle *lcm;
void iTunesProcessWatcher(LPVOID pParam);
void WaitUntiliTunesProcessDie(LPVOID pParam);
TCHAR m_Path[MAX_PATH];
char *mPathChar;