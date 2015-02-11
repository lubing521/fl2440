/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
* File    : GUICurs.C
* Purpose : Cursor routines of the graphics library
*
*
* Version-Date---Author-Explanation
*
* 1.00.00 020517 RS     First release
*
*
* Known problems or limitations with current version
*
*    None.
*
*
* Open issues
*
*    None
*********************************************************************************************************
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Private.h"

/*******************************************************************
*
*                  Statics
*
********************************************************************
*/


static  GUI_RECT _Rect;
static  char _CursorOn;
static  GUI_BITMAP const * _pBitmap;
static  U8   _CursorDeActCnt;
void   (*_cbShowCursor)(void);
void   (*_cbHideCursor)(void);

/*******************************************************************
*
*                  static Show / Hide
*
********************************************************************

Purpose:
  Show / Hide cursor.
*/
static void Hide(void) {
  if (_cbHideCursor)
    _cbHideCursor();
}

static void Show(void) {
  if (_cbShowCursor)
    _cbShowCursor();
}


/*******************************************************************
*
*                  Activate / Deactivate
*
********************************************************************

Purpose:
  Allows activation or deactivation of cursor. Can be used to make
  cursor flash.
*/
void GUI_CURSOR_Activate(void) {
  GUI_LOCK();
  if ((--_CursorDeActCnt) ==0) {
    Show();
  }
  GUI_UNLOCK();
}

void GUI_CURSOR_Deactivate(void) {
  GUI_LOCK();
  if (_CursorDeActCnt-- ==0)
    Hide();
  GUI_UNLOCK();
}


/*******************************************************************
*
*                  GUI_CURSOR_Clear
*
********************************************************************

Purpose:
  Clears cursor.
*/

void GUI_CURSOR_Clear(void) {
  GUI_LOCK();
  Hide();
  _CursorOn = 0;
  /* Set function pointer which window manager can use */
  GUI_CURSOR_pfTempHide   = NULL;
  GUI_CURSOR_pfTempUnhide = NULL;
  GUI_UNLOCK();
}

/*******************************************************************
*
*                  static: SetCursor
*
********************************************************************

Purpose:
  Show cursor.
*/
static void _SetCursor(const GUI_BITMAP* pBM, const GUI_RECT* pRect, void (*cbShow)(void), void (*cbHide)(void)) {
  Hide();  /* Make sure the old cursor (if there was an old one) is history */
  _CursorOn = 1;
  _Rect = *pRect;
  _pBitmap = pBM;
  _cbShowCursor = cbShow;
  _cbHideCursor = cbHide;
  /* Set function pointer which window manager can use */
  GUI_CURSOR_pfTempHide   = GUI_CURSOR_TempHide;
  GUI_CURSOR_pfTempUnhide = GUI_CURSOR_TempUnhide;
  Show();
}

/*******************************************************************
*
*                  Xor Cursor
*
********************************************************************
Purpose:
*/

static void XorDraw(void) {
  GUI_DRAWMODE DMOld;
  DMOld = GUI_SetDrawMode(GUI_DM_XOR);
  GUI_DrawBitmap(_pBitmap, _Rect.x0, _Rect.y0);
  GUI_SetDrawMode(DMOld);
}


void GUI_CURSOR_SetXor(const GUI_BITMAP* pBM, int x, int y) {
  GUI_RECT r;
  GUI_LOCK();
  r.x0 = x;
  r.y0 = y;
  r.x1 = x + pBM->XSize -1;
  r.y1 = y + pBM->YSize -1;
  _SetCursor(pBM, &r, XorDraw, XorDraw);
  GUI_UNLOCK();
}


/*******************************************************************
*
*                  GUI_CURSOR_TempHide
*
********************************************************************
Purpose:
  Hide cursor if a part of the given rectangle is located in the
  rectangle used for the cursor. This routine is called automatically
  by the window manager. This way the window manager can
  automatically make sure that the cursor is always displayed
  correctly.
Params:
  pRect   Rectangle under consideration

*/
void GUI_CURSOR_TempHide(const GUI_RECT* pRect) {
  if (_CursorOn) {
    if (_CursorDeActCnt==0) {
      if ((pRect == NULL) | GUI_RectsIntersect(pRect, &_Rect)) {
        Hide();
      }
    }
  }
}

void GUI_CURSOR_TempUnhide(void) {
}
















