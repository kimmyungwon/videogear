unit ufrmMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, uVGPlayer, TntForms, Menus, TntMenus, ExtCtrls, TntExtCtrls,
  ComCtrls, TntComCtrls, StdCtrls, Buttons, TntButtons, TntDialogs,
  DirectShow9, WideStrings, CommDlg;

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
    mniSP2: TTntMenuItem;
    mniFilters: TTntMenuItem;
    tmrMain: TTimer;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure pnlControlResize(Sender: TObject);
    procedure mniOpenFile(Sender: TObject);
    procedure btnPlayPauseClick(Sender: TObject);
    procedure pmVideoPopup(Sender: TObject);
    procedure tmrMainTimer(Sender: TObject);
    procedure trckbrVolumeChange(Sender: TObject);
  private
    FPlayer: TVGPlayer;
  protected
    procedure PlayerStatusChanges(APlayer: TVGPlayer; AOldStatus, ANewStatus: TVGPlayerStatus);
    procedure SetVideoPanelSize(ANewWidth, ANewHeight: Integer);
  public
    procedure ZoomVideo(AScale: Extended);
  end;

var
  frmMain: TfrmMain;

implementation

{$R *.dfm}

const
  SIZE_CONTROL_HEIGHT     = 32;
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

procedure TfrmMain.btnPlayPauseClick(Sender: TObject);
begin
  FPlayer.PlayOrPause;
end;

procedure TfrmMain.FormCreate(Sender: TObject);
begin
  FPlayer := TVGPlayer.Create;
  FPlayer.OnStatusChanged := PlayerStatusChanges;
  FPlayer.Init(pnlVideo);
end;

procedure TfrmMain.FormDestroy(Sender: TObject);
begin
  FreeAndNil(FPlayer);
end;

procedure TfrmMain.mniOpenFile(Sender: TObject);
var
  hr: HRESULT;
  ofn: TOpenFilenameW;
  szFile: array[0..MAX_PATH] of WideChar;
begin
  FillChar(ofn, SizeOf(TOpenFilenameW), 0);
  FillChar(szFile, SizeOf(szFile), 0);
  ofn.lStructSize := SizeOf(TOpenFilenameW);
  ofn.hWndOwner := Handle;
  ofn.lpstrFilter := 'RealMedia Files(*.rm;*.rmvb)' + #0 + '*.rm;*.rmvb' + #0;
  ofn.lpstrFile := szFile;
  ofn.nMaxFile := MAX_PATH;
  ofn.Flags := OFN_EXPLORER or OFN_FILEMUSTEXIST;
  if GetOpenFileNameW(ofn) then
  begin
    hr := FPlayer.RenderFile(szFile);
    if Failed(hr) then
    begin
      MessageBoxW(Handle, PWideChar(WideFormat('无法播放文件：%s',
        [GetErrorString(hr)])), '错误', MB_OK + MB_ICONSTOP + MB_TOPMOST);
      Exit;
    end;
    FPlayer.Play;
  end;
end;

procedure TfrmMain.PlayerStatusChanges(APlayer: TVGPlayer; AOldStatus, ANewStatus: TVGPlayerStatus);
begin
  case ANewStatus of
    vpsUninitialized: ;
    vpsInitialized: begin
      trckbrProgress.Position := trckbrProgress.Min;
      trckbrProgress.Enabled := False;
    end;
    vpsStopped: begin
      if AOldStatus = vpsInitialized then // 刚刚渲染完毕
      begin
        ZoomVideo(1.0);
        tmrMain.Enabled := False;
        trckbrProgress.Enabled := True;
        trckbrProgress.Max := APlayer.Duration;
        trckbrProgress.Position := 0;
        APlayer.Volume := trckbrVolume.Position;
      end;
    end;
    vpsPlaying: tmrMain.Enabled := True;
    vpsPaused: tmrMain.Enabled := False;
  end;
end;

procedure TfrmMain.pmVideoPopup(Sender: TObject);
var
  lstFilter: TWideStringList;
  newItem: TTntMenuItem;
  I: Integer;
begin
  if FPlayer.Status in [vpsPlaying, vpsPaused] then
  begin
    mniFilters.Enabled := True;
    mniFilters.Clear;
    lstFilter := FPlayer.GetFilterList;
    if lstFilter <> nil then
    begin
      for I := 0 to lstFilter.Count - 1 do
      begin
        newItem := TTntMenuItem.Create(Sender as TTntPopupMenu);
        newItem.Caption := lstFilter[I];
        mniFilters.Add(newItem);
      end;
      lstFilter.Free;
    end;
  end
  else
  begin
    mniFilters.Enabled := False;
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

procedure TfrmMain.SetVideoPanelSize(ANewWidth, ANewHeight: Integer);
begin
  ClientWidth := ANewWidth;
  ClientHeight := ANewHeight + SIZE_CONTROL_HEIGHT;
end;

procedure TfrmMain.tmrMainTimer(Sender: TObject);
begin
  trckbrProgress.Position := FPlayer.Position;
end;

procedure TfrmMain.trckbrVolumeChange(Sender: TObject);
begin
  if FPlayer.Status in [vpsStopped, vpsPlaying, vpsPaused] then
  begin
    FPlayer.Volume := (Sender as TTntTrackBar).Position;
  end;
end;

procedure TfrmMain.ZoomVideo(AScale: Extended);
begin
  SetVideoPanelSize(Round(FPlayer.VideoWidth * AScale),
    Round(FPlayer.VideoHeight * AScale));
end;

end.
