@ECHO OFF

ECHO CCGUICore.BAT:       Compiling GUI\Core

CALL CC gui2dlib
CALL CC GUI__AddSpaceHex
CALL CC GUI__DivideRound
CALL CC GUI__IntersectRect
CALL CC GUI__IntersectRects
CALL CC GUI__ReduceRect
CALL CC GUI_AddBin
CALL CC GUI_AddDec
CALL CC GUI_AddHex
CALL CC GUI_Color2VisColor
CALL CC GUI_DispBin
CALL CC GUI_DispHex
CALL CC GUI_DispStringInRect
CALL CC GUI_DrawBitmap
CALL CC GUI_DrawBitmapExp
CALL CC GUI_DrawBitmapMag
CALL CC GUI_DrawFocusRect
CALL CC GUI_DrawGraph
CALL CC GUI_DrawHLine
CALL CC GUI_DrawPie
CALL CC GUI_DrawPixel
CALL CC GUI_DrawPolyline
CALL CC GUI_DrawRectEx
CALL CC GUI_DrawVLine
CALL CC GUI_ErrorOut
CALL CC GUI_Exec
CALL CC GUI_FillPolygon
CALL CC GUI_FillRect
CALL CC GUI_FillRectEx
CALL CC GUI_GetClientRect
CALL CC GUI_GetColor
CALL CC GUI_GetFontInfo
CALL CC GUI_GetTextExtend
CALL CC GUI_GetVersionString
CALL CC GUI_InvertRect
CALL CC GUI_Log
CALL CC GUI_MergeRect
CALL CC GUI_MOUSE
CALL CC GUI_MoveRect
CALL CC GUI_OnKey
CALL CC GUI_Pen
CALL CC GUI_RectsIntersect
CALL CC GUI_SaveContext
CALL CC GUI_SelLCD
CALL CC GUI_SetColorIndex
CALL CC GUI_SetDecChar
CALL CC GUI_SetDefault
CALL CC GUI_SetLUTEntry
CALL CC GUI_SetOrg
CALL CC GUI_StreamBMP
CALL CC GUI_TOUCH
CALL CC GUI_WaitKey
CALL CC GUI_Warn
CALL CC GUIAlloc
CALL CC guiarc
CALL CC GUIChar
CALL CC GUIChar0
CALL CC GUICharM
CALL CC GUICharP
CALL CC GUICirc
CALL CC GUIColor2Index
CALL CC GUICore
CALL CC GUICurs
CALL CC GUIEncJS
CALL CC GUIIndex2Color
CALL CC GUIPolyE
CALL CC GUIPolyM
CALL CC GUIPolyR
CALL CC GUIStream
CALL CC guitask
CALL CC GUITime
CALL CC guitimer
CALL CC GUIUC0
CALL CC GUIUC1
CALL CC GUIVal
CALL CC GUIValf
CALL CC LCD
CALL CC LCDAA
CALL CC LCDColor
CALL CC LCDGetP
CALL CC LCDInfo
CALL CC LCDInfo0
CALL CC LCDInfo1
CALL CC LCDL0Mag
CALL CC LCDRLE4
CALL CC LCDRLE8
CALL CC LCD_GetColorIndex
CALL CC LCD_GetNumDisplays
CALL CC LCD_GetPixelColor
CALL CC LCD_L0_Generic