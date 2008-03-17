unit uVGLib;

interface

uses
  Windows, SysUtils, ActiveX, DirectShow9, JclPeImage;

type
  IVGFilterList =  interface(IUnknown)
  ['{CA6D4AE8-CFF5-4479-8FE0-11810F2FBFA5}']
    function Add(pBF: IBaseFilter; pName: PWideChar): HRESULT; stdcall;
    function Clear: HRESULT; stdcall;
    function Delete(nIndex: Cardinal): HRESULT; stdcall;
		function Get(nIndex: Cardinal; out pBF: IBaseFilter; out pName: PWideChar): HRESULT; stdcall;
		function GetCount: Cardinal; stdcall;
  end;

  TSpeakerToChannelMap = array[0..17, 0..17] of DWORD;

  IAudioSwitcherFilter = interface(IUnknown)
  ['{CEDB2890-53AE-4231-91A3-B0AAFCD1DBDE}']
    function GetInputSpeakerConfig(pdwChannelMask: PDWORD): HRESULT; stdcall;
    function GetSpeakerConfig(pfCustomChannelMapping: PBoolean;
      pSpeakerToChannelMap: TSpeakerToChannelMap): HRESULT; stdcall;
    function SetSpeakerConfig(pfCustomChannelMapping: Boolean;
      pSpeakerToChannelMap: TSpeakerToChannelMap): HRESULT; stdcall;
    function GetNumberOfInputChannels: Integer; stdcall;
    function IsDownSamplingTo441Enabled: Boolean; stdcall;
    function EnableDownSamplingTo441(fEnable: Boolean): HRESULT; stdcall;
    function GetAudioTimeShift: TReferenceTime; stdcall;
    function SetAudioTimeShift(rtAudioTimeShift: TReferenceTime): HRESULT; stdcall;
    function GetNormalizeBoost(var fNormalize, fNormalizeRecover: Boolean;
      var boost: Single): HRESULT; stdcall;
    function SetNormalizeBoost(fNormalize, fNormalizeRecover: Boolean;
      boost: Single): HRESULT; stdcall;
  end;

  function VGEnumMatchingFilters(out pList: IVGFilterList; dwMerit: DWORD; bInputNeeded: BOOL;
    clsInMaj, clsInSub: TCLSID; bRender, bOutputNeeded: BOOL;
    clsOutMaj, clsOutSub: TCLSID): HResult; stdcall; external 'VGLib.dll';
  function VGEnumMatchingSource(lpszFile: PWideChar; out pBF: IBaseFilter): HRESULT; stdcall; external 'VGLib.dll';
  function VGCreateAudioSwitcher(out pBF: IBaseFilter; out pName: PWideChar): HRESULT; stdcall; external 'VGLib.dll';

implementation

end.
