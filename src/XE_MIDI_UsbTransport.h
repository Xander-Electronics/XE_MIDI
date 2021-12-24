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
#include "XE_MIDI_RingBuffer.h"
#include <MIDIUSB.h>

BEGIN_MIDI_NAMESPACE

template<unsigned BuffersSize>
class UsbTransport
{
  public:
    inline UsbTransport();
    inline ~UsbTransport();

  public: // Serial / Stream API required for template compatibility
    inline void begin(unsigned inBaudrate);
    inline unsigned available();
    inline byte read();
    inline void write(byte inData);

  private:
    inline bool pollUsbMidi();
    inline void recomposeAndSendTxPackets();
    inline void resetTx();
    static inline byte encodePacketHeader(StatusByte inStatusByte);
    static inline int getPacketLength(const midiEventPacket_t& inPacket);

  private:
    typedef RingBuffer<byte, BuffersSize> Buffer;
    Buffer mTxBuffer;
    Buffer mRxBuffer;

    union TxPacket
    {
      byte mDataArray[4];
      midiEventPacket_t mPacket;
    };
    TxPacket mCurrentTxPacket;
    int mCurrentTxPacketByteIndex;
};

END_MIDI_NAMESPACE

#include "midi_UsbTransport.hpp"
