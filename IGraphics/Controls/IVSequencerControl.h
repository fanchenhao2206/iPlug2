/*
 ==============================================================================

 This file is by Tim Fan <fanchenhao2206@icloud.com>

 ==============================================================================
*/

#pragma once

/**
 * @file
 * @copydoc IVSequencerControl
 */

#include "IControl.h"

BEGIN_IPLUG_NAMESPACE
BEGIN_IGRAPHICS_NAMESPACE

template <int MAXNC = 16, int MAXNR = 12>
class IVSequencerControl : public IVTrackControlBase
{
public:
  using OnNewValueFunc = std::function<void(int trackIdx, double val)>;
  
  /** Constructs a vector multi slider control that is not linked to parameters
   * @param bounds The control's bounds
   * @param label The label for the vector control, leave empty for no label
   * @param style The styling of this vector control \see IVStyle
   * @param direction The direction of the sliders */
  IVSequencerControl(const IRECT& bounds, const char* label, const IVStyle& style = DEFAULT_STYLE, EDirection dir = EDirection::Vertical)
  : IVTrackControlBase(bounds, label, style, 
      MAXNC /* maxNTracks */, MAXNR /* nSteps */, dir)
  {
    mDrawTrackFrame = true;
    mDrawStepFrame = true;
    mTrackPadding = 1.f;
  }

  void Draw(IGraphics& g) override
  {
    // Fill the whole rect with background (aka transparent)
    DrawBackground(g, mRECT);

    // Draw the widget; calls DrawTrack for each of the nVals tracks,
    // set by SetNVals(maxNTracks) in this child of IVTrackControlBase. 
    // Tracks, are the columns. And each track can have an integer number 
    // of steps, if desired. If left at zero, dragging the slider up and 
    // down moves the value of this track between 0 and 1.
    DrawWidget(g);
    
    if(mStyle.drawFrame)
      // Draw a frame around the widget
      g.DrawRect(GetColor(kFR), mWidgetBounds, &mBlend, mStyle.frameThickness);
  }

  /** Implement this method to respond to a mouse down event on this control. 
   * @param x The X coordinate of the mouse event
   * @param y The Y coordinate of the mouse event
   * @param mod A struct indicating which modifier keys are held for the event */
  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    // 2206: so simple!
    // User has clicked the coordinate (x,y) of the whole screen, which must be within the bounds 
    // of this control; else why are we here?

    // First, get cell of this control corresponding to given (x,y); guaranteed to return a 
    // cell, because this control is all cells, a grid of cells.

    // Modify control based on given (x,y) that mouse clicked on; e.g., if was on, turn off, 
    // was off, turn on.

  }

  /** Implement this method to respond to a mouse drag event on this control. 
   * @param x The X coordinate of the mouse event
   * @param y The Y coordinate of the mouse event
   * @param dX The X delta (difference) since the last event
   * @param dY The Y delta (difference) since the last event
   * @param mod A struct indicating which modifier keys are held for the event */
  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override
  {
    // 2206: so simple!
    // User has dragged their mouse over to the coordinate (x,y). This is interesting; what should 
    // be done to the cell corresponding to this coordinate? One idea, is to remember what caused 
    // this drag event to happen in the first place. In other words, what was the MouseDown event?

    // If the mouse down event was on a cell that was originally off (and so afterwards turned on), 
    // then perhaps the user is interested in turning on all cells they afterwards drag their mouse 
    // across. Otherwise, if they only wanted to turn on the initial cell, then they would click it, 
    // and let go; not continue to hold their mouse down and drag across to other cells too! 

    // Remember the initial mouse down event---call it prevMouseDown or something like that---
    // which can be either:
    //
    // (i) true, when the initial cell was originally off, and so afterwards turned on by the mouse 
    // click, so that as the mouse is dragged past that initial mouse click, every cell it touches 
    // also turns on. Keep in mind, if the mouse is clicked on and then just held down on the initial
    // cell, then technically the initial cell is just repeatedly turned on; this works!
    // 
    // (ii) false, when the initial cell was originally on, and so afterwards turned off by the mouse
    // click, so that as the mouse is dragged past that initial mouse click, every cell it touches
    // also turns off. Keep in mind, if the mouse is clicked on and then just held down on the initial
    // cell, then technically the initial cell is just repeatedly turned off; this works!

  }

  /** override to do something when an individual slider is dragged */
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
