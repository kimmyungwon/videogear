object frmMain: TfrmMain
  Left = 0
  Top = 0
  Caption = 'VideoGear'
  ClientHeight = 294
  ClientWidth = 480
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = mmMain
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object pnlControl: TPanel
    Left = 0
    Top = 270
    Width = 480
    Height = 24
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitTop = 274
    object btnPlayPause: TSpeedButton
      Left = 24
      Top = 0
      Width = 24
      Height = 24
      Align = alLeft
      Caption = 'S'
      Flat = True
    end
    object btnStop: TSpeedButton
      Left = 0
      Top = 0
      Width = 24
      Height = 24
      Align = alLeft
      Caption = 'P'
      Flat = True
    end
    object trckbrVol: TTrackBar
      Left = 400
      Top = 0
      Width = 80
      Height = 24
      Margins.Left = 0
      Margins.Top = 4
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alRight
      Max = 100
      PageSize = 10
      Position = 100
      ShowSelRange = False
      TabOrder = 0
      TickMarks = tmBoth
      TickStyle = tsNone
      ExplicitLeft = 295
      ExplicitTop = 6
      ExplicitHeight = 20
    end
  end
  object pnlVideo: TPanel
    Left = 0
    Top = 0
    Width = 480
    Height = 270
    Align = alClient
    BevelOuter = bvNone
    Caption = 'pnlVideo'
    Color = clBlack
    ParentBackground = False
    TabOrder = 1
    OnDblClick = pnlVideoDblClick
    ExplicitLeft = 144
    ExplicitTop = 64
    ExplicitWidth = 185
    ExplicitHeight = 41
  end
  object mmMain: TMainMenu
    AutoHotkeys = maManual
    Left = 16
    Top = 8
    object mniFile: TMenuItem
      Caption = #25991#20214
      object mniExit: TMenuItem
        Caption = #36864#20986
      end
    end
  end
end
