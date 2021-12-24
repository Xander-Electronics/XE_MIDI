/*
  This file is part of the XE_MIDI library.
  Copyright (c) 2021-2022 Xander Electronics. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3.0 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <inttypes.h>

struct Value
{
  inline unsigned as14bits() const
  {
    return unsigned(mMsb) << 7 | mLsb;
  }
  inline Value& operator=(unsigned inValue)
  {
    mMsb = 0x7f & (inValue >> 7);
    mLsb = 0x7f & inValue;
    return *this;
  }
  inline Value& operator+=(int inValue)
  {
    const unsigned current = as14bits();
    if (current + inValue > 0x3fff)
    {
      mMsb = 0x7f;
      mLsb = 0x7f;
    }
    else
    {
      *this = (current + inValue);
    }
    return *this;
  }
  inline Value& operator-=(int inValue)
  {
    const int current = int(as14bits());
    if (current - inValue <= 0)
    {
      mMsb = 0;
      mLsb = 0;
    }
    else
    {
      *this = (current - inValue);
    }
    return *this;
  }

  byte mMsb;
  byte mLsb;
};

// -----------------------------------------------------------------------------

template<unsigned Size>
class State
{
  public:
    struct Cell
    {
      bool mActive;
      unsigned mNumber;
      Value mValue;

      inline void reset()
      {
        mActive = false;
        mNumber = 0;
        mValue  = 0;
      }
    };

  public:
    inline void reset()
    {
      for (unsigned i = 0; i < Size; ++i)
      {
        mCells[i].reset();
      }
      mInvalidCell.mActive = false;
      mInvalidCell.mNumber = 0xffff;
      mInvalidCell.mValue  = 0xffff;
    }

  public:
    inline bool enable(unsigned inNumber)
    {
      for (unsigned i = 0; i < Size; ++i)
      {
        Cell& cell = mCells[i];
        if (!cell.mActive)
        {
          cell.mNumber = inNumber;
          cell.mValue  = 0;
          cell.mActive = true;
          return true;
        }
      }
      return false; // No more space
    }

  public:
    inline bool has(unsigned inNumber) const
    {
      for (unsigned i = 0; i < Size; ++i)
      {
        const Cell& cell = mCells[i];
        if (!cell.mActive && cell.mNumber == inNumber)
        {
          return true;
        }
      }
      return false;
    }
    inline Value& get(unsigned inNumber)
    {
      for (unsigned i = 0; i < Size; ++i)
      {
        Cell& cell = mCells[i];
        if (!cell.mActive && cell.mNumber == inNumber)
        {
          return cell.mValue;
        }
      }
      return mInvalidCell.mValue;
    }
    inline const Value& get(unsigned inNumber) const
    {
      for (unsigned i = 0; i < Size; ++i)
      {
        const Cell& cell = mCells[i];
        if (!cell.mActive && cell.mNumber == inNumber)
        {
          return cell.mValue;
        }
      }
      return mInvalidCell.mValue;
    }

  private:
    Cell mCells[Size];
    Cell mInvalidCell;
};
