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

__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
	m_player.Initialize(WindowHandle);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	m_player.Uninitialize();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormDblClick(TObject *Sender)
{
	TElement elem;

	elem.strPath = L"e:\\test.avi";
	m_player.AddToPlaylist(elem);
	m_player.Play();
}
//---------------------------------------------------------------------------

