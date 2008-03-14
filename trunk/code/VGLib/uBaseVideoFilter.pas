unit uBaseVideoFilter;

interface

uses
  Windows, SyncObjs, DirectShow9, BaseClass, DSUtil;

type
  TBaseVideoFilter = class(TBCTransformFilter)
  private
    m_win, m_hin, m_arxin, m_aryin: Cardinal;
	  m_wout, m_hout, m_arxout, m_aryout: Cardinal;
  protected
    m_w, m_h, m_arx, m_ary: Integer;
  public
    constructor Create(_Name: string; _Unk: IUnKnown; out _HR: HRESULT; const _Clsid: TGUID; _Buffers: Integer);
    destructor Destroy; override;
    
    function GetPinCount: Integer; override;
    function GetPin(_n: Integer): TBCBasePin; override;
    function Receive(_Sample: IMediaSample): HRESULT; override;
  end;

implementation

uses JclSysUtils;

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
  
  Result := Transform()
  

	if(FAILED(hr = Transform(pIn)))
		return hr;

	return S_OK;
  
end;

end.
