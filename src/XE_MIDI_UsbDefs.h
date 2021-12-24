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

#include "XE_MIDI_Defs.h"

BEGIN_MIDI_NAMESPACE

struct CodeIndexNumbers
{
  enum
  {
    reserved                = 0x00,
    misc                    = reserved,

    cableEvent              = 0x01,
    systemCommon2Bytes      = 0x02,
    systemCommon3Bytes      = 0x03,

    sysExStart              = 0x04,
    sysExContinue           = sysExStart,

    systemCommon1Byte       = 0x05,
    sysExEnds1Byte          = systemCommon1Byte,

    sysExEnds2Bytes         = 0x06,
    sysExEnds3Bytes         = 0x07,
    noteOff                 = 0x08,
    noteOn                  = 0x09,
    polyPressure            = 0x0A,
    controlChange           = 0x0B,
    programChange           = 0x0C,
    channelPressure         = 0x0D,
    pitchBend               = 0x0E,
    singleByte              = 0x0F,
  };

  static inline byte getSize(byte inCodeIndexNumber)
  {
    switch (inCodeIndexNumber)
    {
      case noteOn:
      case noteOff:
      case controlChange:
      case pitchBend:
      case polyPressure:
      case systemCommon3Bytes:
      case sysExEnds3Bytes:
      case sysExStart:
        return 3;

      case programChange:
      case channelPressure:
      case systemCommon2Bytes:
      case sysExEnds2Bytes:
        return 2;

      case systemCommon1Byte:
      case singleByte:
        return 1;

      default:
        break;
    }
    return 0; // Can be any length (1, 2 or 3).
  }
};

// -----------------------------------------------------------------------------

struct UsbMidiEventPacket
{
  public:
    inline UsbMidiEventPacket()
    {
      memset(mData, 0, 4 * sizeof(byte));
    }

  public:
    inline void setHeader(byte inCableNumber, byte inCodeIndexNumber)
    {
      const byte msb = (0x0f & inCableNumber) << 4;
      const byte lsb = (0x0f & inCodeIndexNumber);
      mData[0] = msb | lsb;
    }
    inline void setMidiData(const byte* inData)
    {
      mData[1] = *inData++;
      mData[2] = *inData++;
      mData[3] = *inData;
    }
    inline byte getCableNumber() const
    {
      return mData[0] >> 4;
    }
    inline byte getCodeIndexNumber() const
    {
      return mData[0] & 0x0f;
    }
    inline const byte* getMidiData() const
    {
      return mData + 1;
    }
    inline byte* getMidiData()
    {
      return mData + 1;
    }
    inline UsbMidiEventPacket& operator=(const byte* inData)
    {
      mData[0] = *inData++;
      setMidiData(inData);
      return *this;
    }

  public:
    byte mData[4];
};

END_MIDI_NAMESPACE
