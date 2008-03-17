unit ufrmMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, uVGPlayer, TntForms, Menus, TntMenus, ExtCtrls, TntExtCtrls,
  ComCtrls, TntComCtrls, StdCtrls, Buttons, TntButtons, TntDialogs,
  DirectShow9;

type
  TfrmMain = class(TTntForm)
    pnlVideo: TTntPanel;
    pnlControl: TTntPanel;
    trckbrProgress: TTntTrackBar;
    btnPlayPause: TTntBitBtn;
    btnStop: TTntBitBtn;
    trckbrVolume: TTntTrackBar;
    splProgressBegin: TTntSplitter;
    pmVideo: TTntPopupMenu;
    mniVideoOpenFile: TTntMenuItem;
    dlgOpen: TTntOpenDialog;
    mniSP1: TTntMenuItem;
    mniVideoPlayPause: TTntMenuItem;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure pnlControlResize(Sender: TObject);
    procedure mniOpenFile(Sender: TObject);
  private
    FPlayer: TVGPlayer;
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

{$R *.dfm}

const
  SIZE_VOLUME_WIDTH       = 100;
  SIZE_VOLUME_HEIGHT      = 20;
  SIZE_PROGRESS_HEIGHT    = 20;
  SIZE_PROGRESS_MINWIDTH  = 150;

function GetErrorString(hr: HRESULT): WideString;
var
  szMsg: array[0..MAX_ERROR_TEXT_LEN] of WideChar;
begin
  AMGetErrorTextW(hr, szMsg, MAX_ERROR_TEXT_LEN);
  Result := szMsg;
end;

{ TfrmMain }

procedure TfrmMain.FormCreate(Sender: TObject);
begin
  FPlayer := TVGPlayer.Create;
  FPlayer.Init(pnlVideo);
end;

procedure TfrmMain.FormDestroy(Sender: TObject);
begin
  FreeAndNil(FPlayer);
end;

procedure TfrmMain.mniOpenFile(Sender: TObject);
var
  hr: HRESULT;
begin
  dlgOpen.Filter := 'RealMedia Files(*.rm;*.rmvb)|*.rm;*.rmvb';
  if dlgOpen.Execute then
  begin
    hr := FPlayer.RenderFile(dlgOpen.FileName);
    if Failed(hr) then
    begin
      MessageBoxW(Handle, PWideChar(WideFormat('无法播放文件：%s',
        [GetErrorString(hr)])), '错误', MB_OK + MB_ICONSTOP + MB_TOPMOST);
      Exit;
    end;
    FPlayer.Play;
  end;
end;

procedure TfrmMain.pnlControlResize(Sender: TObject);
begin
  with trckbrVolume do
  begin
    Width := SIZE_VOLUME_WIDTH;
    Height := SIZE_VOLUME_HEIGHT;
    Left := (Sender as TTntPanel).ClientWidth - Width;
    Top := ((Sender as TTntPanel).ClientHeight - Height) div 2;
  end;
  with trckbrProgress do
  begin
    Left := splProgressBegin.Left + splProgressBegin.Width;
    Width := trckbrVolume.Left - Left;
    Height := SIZE_PROGRESS_HEIGHT;
    Top := ((Sender as TTntPanel).ClientHeight - Height) div 2;
  end;
end;

end.
