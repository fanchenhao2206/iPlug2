/*
 ==============================================================================

 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers.

 See LICENSE.txt for  more info.

 ==============================================================================
*/

#pragma once

#include "IControl.h"

BEGIN_IPLUG_NAMESPACE
BEGIN_IGRAPHICS_NAMESPACE

class IVSequencerControl : public IControl
{
public:
  using OnNewValueFunc = std::function<void(int trackIdx, double val)>;

  IVSequencerControl(const IRECT& bounds)
  : IControl(bounds)
  {}
  
  void Draw(IGraphics& g) override
  {
    g.DrawRect(COLOR_BLACK, mRECT);
    g.FillRect(COLOR_WHITE, mRECT);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    /* 2206: */
    // User has clicked the coordinate (x,y) of the whole screen, which must be 
    // within the bounds of this control; else why are we here?

    // First, get cell of this control corresponding to given (x,y); guaranteed 
    // to return a cell, because this control is all cells, a grid of cells.

    // Modify control based on given (x,y) that mouse clicked on; e.g., if was 
    // on, turn off, was off, turn on.
  }

  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override
  {
    /* 2206: */
    // User has dragged their mouse over to the coordinate (x,y). This is 
    // interesting; what should be done to the cell corresponding to this 
    // coordinate? One idea, is to remember what caused this drag event to 
    // happen in the first place. In other words, what was the MouseDown event?

    // If the mouse down event was on a cell that was originally off (and so 
    // afterwards turned on), then perhaps the user is interested in turning on 
    // all cells they afterwards drag their mouse across. Otherwise, if they 
    // only wanted to turn on the initial cell, then they would click it, and 
    // let go; not continue to hold their mouse down and drag across to other 
    // cells too! 

    // Remember the initial mouse down event---call it prevMouseDown or 
    // something like that---which can be either:
    //
    // (i) true, when the initial cell was originally off, and so afterwards 
    // turned on by the mouse click, so that as the mouse is dragged past that 
    // initial mouse click, every cell it touches also turns on. Keep in mind, 
    // if the mouse is clicked on and then just held down on the initial cell, 
    // then technically the initial cell is just repeatedly turned on; this 
    // works!
    // 
    // (ii) false, when the initial cell was originally on, and so afterwards 
    // turned off by the mouse click, so that as the mouse is dragged past that 
    // initial mouse click, every cell it touches also turns off. Keep in mind, 
    // if the mouse is clicked on and then just held down on the initial cell, 
    // then technically the initial cell is just repeatedly turned off; this 
    // works!
  }

  /* .cpp files that extend me (e.g. IMidiSequencer) can override this */
  virtual void OnNewValue(int trackIdx, double val)
  {
    if(mOnNewValueFunc)
      mOnNewValueFunc(trackIdx, val);
  }
  
  void SetOnNewValueFunc(OnNewValueFunc func)
  {
    mOnNewValueFunc = func;
  }
  
protected:
  OnNewValueFunc mOnNewValueFunc = nullptr;
};

END_IGRAPHICS_NAMESPACE
END_IPLUG_NAMESPACE
