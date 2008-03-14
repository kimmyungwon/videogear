unit uBaseVideoFilter;

interface

uses
  Windows, ActiveX, SyncObjs, DirectShow9, BaseClass, DSUtil;

type
  TBaseVideoFilter = class(TBCTransformFilter)
  private
    m_win, m_hin, m_arxin, m_aryin: Cardinal;
	  m_wout, m_hout, m_arxout, m_aryout: Cardinal;

    function ReconnectOutput(_w, _h: Integer): HRESULT;
  protected
    m_w, m_h, m_arx, m_ary: Integer;

    function GetDeliveryBuffer(_w, _h: Integer; out _Out: IMediaSample): HRESULT;
    function Transform(_In: IMediaSample): HRESULT; reintroduce; virtual; abstract;
  public
    constructor Create(_Name: string; _Unk: IUnKnown; out _HR: HRESULT; const _Clsid: TGUID; _Buffers: Integer);
    destructor Destroy; override;
    
    function GetPinCount: Integer; override;
    function GetPin(_n: Integer): TBCBasePin; override;
    function Receive(_Sample: IMediaSample): HRESULT; override;
  end;

implementation

uses JclSysUtils;

function GetFilterFromPin(_Pin: IPin): IBaseFilter;
var
  pi: TPinInfo;
begin
  Result := nil;
  if _Pin = nil then
    Exit;
  if (_Pin <> nil) and Succeeded(_Pin.QueryPinInfo(pi)) then
    Result := pi.pFilter;
end;

function GetCLSID(_BF: IBaseFilter): TGUID; overload;
begin
  Result := GUID_NULL;
  if _BF <> nil then
    _BF.GetClassID(Result);
end;

function GetCLSID(_Pin: IPin): TGUID; overload;
begin
  Result := GetCLSID(GetFilterFromPin(_Pin));
end;

{ TBaseVideoFilter }

constructor TBaseVideoFilter.Create(_Name: string; _Unk: IUnKnown; out _HR: HRESULT; const _Clsid: TGUID; _Buffers: Integer);
begin
  inherited Create(_Name, _Unk, _Clsid);

  FInput := TBCTransformInputPin.Create('TBaseVideoInputPin', Self, _HR, 'Video');
  if Failed(_HR) then
    Exit;

  FOutput := TBCTransformOutputPin.Create('TBaseVideoOutputPin', Self, _HR, 'Audio');
  if Failed(_HR) then
    Exit;

  m_wout := 0; m_win := 0; m_w := 0;
	m_hout := 0; m_hin := 0; m_h := 0;
	m_arxout := 0; m_arxin := 0; m_arx := 0;
	m_aryout := 0; m_aryin := 0; m_ary := 0;
end;

destructor TBaseVideoFilter.Destroy;
begin

  inherited;
end;

function TBaseVideoFilter.GetDeliveryBuffer(_w, _h: Integer; out _Out: IMediaSample): HRESULT;
var
  pMT: PAMMediaType;
begin
  Result := ReconnectOutput(_w, _h);
  if Failed(Result) then
    Exit;

  Result := FOutput.GetDeliveryBuffer(_Out, nil, nil, 0);
  if Failed(Result) then
    Exit;

  if Succeeded(_Out.GetMediaType(pMT)) and (pMT <> nil) then
  begin
    FInput.SetMediaType(pMT);
    DeleteMediaType(pMT);
  end;

  _Out.SetDiscontinuity(False);
  _Out.SetSyncPoint(True);

  // FIXME: hell knows why but without this the overlay mixer starts very skippy
	// (don't enable this for other renderers, the old for example will go crazy if you do)
  
  if IsEqualCLSID(CLSID_OverlayMixer, GetCLSID(FOutput.GetConnected)) then
    _Out.SetDiscontinuity(True);

  Result := S_OK;
end;

function TBaseVideoFilter.GetPin(_n: Integer): TBCBasePin;
begin
  case _n of
    0: Result := FInput;
    1: Result := FOutput;
  else
    Result := nil;
  end;
end;

function TBaseVideoFilter.GetPinCount: integer;
begin
  Result := 2;
end;

function TBaseVideoFilter.Receive(_Sample: IMediaSample): HRESULT;
var
  pProps: PAMSample2Properties;
  pMT: PAMMediaType;
begin
  FcsReceive.Lock;

  pProps := FInput.SampleProps;
  if pProps^.dwStreamId <> AM_STREAM_MEDIA then
  begin
    Result := FOutput.Deliver(_Sample);
    Exit;
  end;

  if Succeeded(_Sample.GetMediaType(pMT)) and (pMT <> nil) then
  begin
    FInput.SetMediaType(pMT);
    DeleteMediaType(pMT);
  end;

  Result := Transform(_Sample);
  if Failed(Result) then
    Exit;

  Result := S_OK;
end;

function TBaseVideoFilter.ReconnectOutput(_w, _h: Integer): HRESULT;
var
  pMT: PAMMediaType;
  w_org, h_org: Integer;
  fForceReconnection: Boolean;
  bmi: PBitmapInfoHeader;
  vih: PVideoInfoHeader;
  vih2: PVideoInfoHeader2;
begin
  pMT := FOutput.CurrentMediaType.MediaType;

  w_org := m_w;
  h_org := m_h;

  fForceReconnection := False;
  if (_w <> m_w) or (_h <> m_h) then
  begin
    fForceReconnection := True;
    m_w := _w;
    m_h := _h;
  end;

  if fForceReconnection or (m_w <> m_wout) or (m_h <> m_hout) or (m_arx <> m_arxout) <> (m_ary <> m_aryout) then
  begin
    if IsEqualCLSID(CLSID_VideoRenderer, GetCLSID(FOutput.GetConnected)) then
    begin
      NotifyEvent(EC_ERRORABORT, 0, 0);
      Result := E_FAIL;
      Exit;
    end;

    if IsEqualGUID(FORMAT_VideoInfo, pMT^.formattype) then
    begin
      vih := PVideoInfoHeader(pMT^.pbFormat);
      SetRect(vih^.rcSource, 0, 0, m_w, m_h);
      SetRect(vih^.rcTarget, 0, 0, m_w, m_h);
      bmi := @vih^.bmiHeader;
      bmi^.biXPelsPerMeter := m_w * m_ary;
      bmi^.biYPelsPerMeter := m_h * m_arx;
    end
    else if IsEqualGUID(FORMAT_VideoInfo2, pMT^.formattype) then
    begin
      vih2 := PVideoInfoHeader2(pMT^.pbFormat);
      SetRect(vih2^.rcSource, 0, 0, m_w, m_h);
      SetRect(vih2^.rcTarget, 0, 0, m_w, m_h);
      bmi := @vih2^.bmiHeader;
      vih2^.dwPictAspectRatioX := m_arx;
      vih2^.dwPictAspectRatioY := m_ary;
    end;

    bmi^.biWidth := m_w;
    bmi^.biHeight := m_h;
    bmi^.biSizeImage := m_w * m_h * bmi^.biBitCount shr 3;
  end;
end;

end.
