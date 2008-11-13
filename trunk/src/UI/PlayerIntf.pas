unit PlayerIntf;

interface

uses
  Windows;

type
  IPlayer = interface(IUnknown)
    ['{588BC08D-103A-4FCE-B23D-E4DC8B2EC3C6}']
    	function Initialize(hVidWnd: HWND; hMsgWnd: HWND): HRESULT; stdcall;
      function Uninitialize: HRESULT; stdcall;
      function OpenUrl(lpcwstrUrl: LPCWSTR): HRESULT; stdcall;
      function Play: HRESULT; stdcall;
      function Pause: HRESULT; stdcall;
      function Stop: HRESULT; stdcall;
      function get_State: ULONG; stdcall;

      property State: ULONG read get_State;
  end;


const
  CORE_DLL = 'VGearCore.dll';

  function VGCreatePlayer(out ppvObj): HRESULT; stdcall; external CORE_DLL;

implementation


end.
