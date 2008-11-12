//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Player.h"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlControl;
	TSpeedButton *btnPlayPause;
	TSpeedButton *btnStop;
	TTrackBar *trckbrVol;
	TPanel *pnlVideo;
	TMainMenu *mmMain;
	TMenuItem *mniFile;
	TMenuItem *mniExit;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall pnlVideoDblClick(TObject *Sender);
private:	// User declarations
	TPlayer	m_player;
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
