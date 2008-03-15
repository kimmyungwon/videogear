unit ufrmMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, DirectShow9;

type
  TfrmMain = class(TForm)
    procedure FormDblClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

uses uVGLib;

{$R *.dfm}

procedure TfrmMain.FormDblClick(Sender: TObject);
var
  pList: IVGFilterList;
  InputTypes, OutputTypes: array of TGUID;
begin
  VGEnumMatchingFilters(pList, 0, False, MERIT_DO_NOT_USE, True, Length(InputTypes),
    @InputTypes[0], nil, nil, False, True, Length(OutputTypes), @OutputTypes[0],
    nil, nil);
end;

end.
