//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HookUtils.h"
#include "FormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------

void __stdcall MySleep(DWORD)
{
    ShowMessage(L"MySleep!");
}

__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
	//m_player.Initialize(WindowHandle);

	void (__stdcall *RealSleep)(DWORD) = Sleep;

	HookAPI((void*&)RealSleep, MySleep);
	Sleep(200);
	RealSleep(2000);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	//m_player.Uninitialize();
}
//---------------------------------------------------------------------------

