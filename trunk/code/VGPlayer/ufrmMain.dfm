object frmMain: TfrmMain
  Left = 0
  Top = 0
  Caption = 'VGPlayer'
  ClientHeight = 272
  ClientWidth = 320
  Color = clBtnFace
  Constraints.MinWidth = 320
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object pnlVideo: TTntPanel
    Left = 0
    Top = 0
    Width = 320
    Height = 240
    Align = alClient
    BevelOuter = bvNone
    Color = clBlack
    ParentBackground = False
    PopupMenu = pmVideo
    TabOrder = 0
  end
  object pnlControl: TTntPanel
    Left = 0
    Top = 240
    Width = 320
    Height = 32
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    OnResize = pnlControlResize
    DesignSize = (
      320
      32)
    object splProgressBegin: TTntSplitter
      Left = 64
      Top = 0
      Height = 32
      ResizeStyle = rsNone
      ExplicitLeft = 384
      ExplicitTop = 8
      ExplicitHeight = 100
    end
    object trckbrProgress: TTntTrackBar
      Left = 66
      Top = 6
      Width = 150
      Height = 20
      Anchors = []
      ShowSelRange = False
      TabOrder = 0
      ThumbLength = 15
      TickStyle = tsNone
    end
    object btnPlayPause: TTntBitBtn
      Left = 0
      Top = 0
      Width = 32
      Height = 32
      Align = alLeft
      Caption = 'P'
      TabOrder = 1
      OnClick = btnPlayPauseClick
    end
    object btnStop: TTntBitBtn
      Left = 32
      Top = 0
      Width = 32
      Height = 32
      Align = alLeft
      Caption = 'S'
      TabOrder = 2
    end
    object trckbrVolume: TTntTrackBar
      Left = 217
      Top = 6
      Width = 100
      Height = 25
      ShowSelRange = False
      TabOrder = 3
      ThumbLength = 15
      TickMarks = tmBoth
      TickStyle = tsNone
    end
  end
  object pmVideo: TTntPopupMenu
    AutoHotkeys = maManual
    OnPopup = pmVideoPopup
    Left = 8
    Top = 8
    object mniVideoOpenFile: TTntMenuItem
      Caption = #25171#24320#25991#20214'...'
      OnClick = mniOpenFile
    end
    object mniSP1: TTntMenuItem
      Caption = '-'
    end
    object mniVideoPlayPause: TTntMenuItem
      Caption = #25773#25918'/'#26242#20572
    end
    object mniSP2: TTntMenuItem
      Caption = '-'
    end
    object mniFilters: TTntMenuItem
      Caption = #28388#38236
    end
  end
  object dlgOpen: TTntOpenDialog
    Options = [ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 40
    Top = 8
  end
end
