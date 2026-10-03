/*
 ==============================================================================

 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers.

 See LICENSE.txt for  more info.

 ==============================================================================
*/

#pragma once

#include "IControl.h"
#include <array>

BEGIN_IPLUG_NAMESPACE
BEGIN_IGRAPHICS_NAMESPACE

template <int NROW, int NCOL>
class IVSequencerControl : public IControl
{
public:
  IVSequencerControl(const IRECT& bounds, int paramIdx = kNoParameter, 
    std::function<void()> updateFn = nullptr)
  : IControl(bounds, paramIdx), mUpdateFn(updateFn)
  {
    mPrevCol = mPrevRow = 0;
    mIgnoreMouse = false;
  }
  
  void Draw(IGraphics& g) override
  {
    g.FillRect(COLOR_WHITE, mRECT);
    g.DrawRect(COLOR_BLACK, mRECT, 0, 4.f);

    // // sketch:
    // char str[64];
    // g.DrawText(DEFAULT_TEXT, str, mRECT);

    // Draw grid: nRow by nCol cells
    float rowSpacing = mRECT.H() / NROW;
    float colSpacing = mRECT.W() / NCOL;

    // Fill in selected cells
    assert(mCells.size() == NCOL);
    for (int i = 0; i < NCOL; i++) {
      int cell = mCells[i];
      if (!cell) continue;

      // Fill in the cell at column i, and row given by cell
      g.FillRect(COLOR_BLACK, IRECT::MakeXYWH(
        mRECT.L + (i * colSpacing), mRECT.T + (cell * rowSpacing),
        colSpacing, rowSpacing));
    }

    // Draw dividing lines
    for (int i = 1; i <= NROW-1; i++) {
      // Draw nRow-1 horizontal lines
      float y = mRECT.T + (i * rowSpacing);

      // Make it easier to track which note
      float thickness = (i % 4 == 0) ? 4.f : 1.f; 
      g.DrawLine(COLOR_BLACK, mRECT.L, y, mRECT.R, y, 0, thickness);
    }
    for (int i = 1; i <= NCOL-1; i++) {
      // Draw nCol-1 vertical lines
      float x = mRECT.L + (i * colSpacing);

      // Emphasize the bar lines
      float thickness = (i % 4 == 0) ? 4.f : 1.f; 
      g.DrawLine(COLOR_BLACK, x, mRECT.B, x, mRECT.T, 0, thickness);
    }
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    // Mouse needs to be in-bounds to edit the sequencer
    if (!mRECT.Contains(x, y)) return;

    // (x,y) are given as absolute coordinates; now that we know it 
    // is a point within mRECT, let's find its relative coordinates
    float relX = x - mRECT.L;
    float relY = y - mRECT.T;

    // Then, it's easy to find which grid this point belongs in
    float rowSpacing = mRECT.H() / NROW;
    float colSpacing = mRECT.W() / NCOL;

    float colIdx = (int)(relX / rowSpacing);
    float rowIdx = (int)(relY / colSpacing);

    // So, turn it on! Or, off it is already on
    if (mCells[colIdx] == rowIdx) {
      // We clicked on (row,col), but that is already on; so turn off
      mCells[colIdx] = 0;
    } else {
      // We clicked on (row,col), which is not already on, so turn it on
      mCells[colIdx] = rowIdx;
    }

    // And re-draw control
    SetDirty(true);

    // Used by OnMouseDrag to know what mode it is in
    mPrevCol = colIdx;
    mPrevRow = rowIdx;
  }

  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override
  {
    // Mouse needs to be in-bounds to edit the sequencer
    if (!mRECT.Contains(x, y)) return;

    // (x,y) are given as absolute coordinates; now that we know it 
    // is a point within mRECT, let's find its relative coordinates
    float relX = x - mRECT.L;
    float relY = y - mRECT.T;

    // Then, it's easy to find which grid this point belongs in
    float rowSpacing = mRECT.H() / NROW;
    float colSpacing = mRECT.W() / NCOL;

    float colIdx = (int)(relX / rowSpacing);
    float rowIdx = (int)(relY / colSpacing);

    // So, turn it on! Or, off, depending on mPrevCol/Row
    if (mCells[mPrevCol]) {
      // User turned ON the cell they clicked, so lets turn ON this cell too
      mCells[colIdx] = rowIdx;
    } else {
      // User turned OFF the cell they clicked, so lets turn OFF this cell too
      if (mCells[colIdx] == rowIdx) {
        // As long as it was previously on...
        mCells[colIdx] = 0;
      }
    }

    // And re-draw control
    SetDirty(true);
  }

  /* .cpp files that extend me (e.g. IMidiSequencer) can override this */
  virtual void OnNewValue()
  {
    if(mUpdateFn)
      mUpdateFn();
  }
  
protected:
  std::function<void()> mUpdateFn;
private:
  std::array<int, NCOL> mCells;
  int mPrevCol, mPrevRow;
};

END_IGRAPHICS_NAMESPACE
END_IPLUG_NAMESPACE
