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

#include "XE_MIDI.h"

// -----------------------------------------------------------------------------

BEGIN_MIDI_NAMESPACE

/*! \brief Encode System Exclusive messages.
  SysEx messages are encoded to guarantee transmission of data bytes higher than
  127 without breaking the MIDI protocol. Use this static method to convert the
  data you want to send.
  \param inData The data to encode.
  \param outSysEx The output buffer where to store the encoded message.
  \param inLength The lenght of the input buffer.
  \return The lenght of the encoded output buffer.
  @see decodeSysEx
  Code inspired from Ruin & Wesen's SysEx encoder/decoder - http://ruinwesen.com
*/
unsigned encodeSysEx(const byte* inData, byte* outSysEx, unsigned inLength)
{
  unsigned outLength  = 0;     // Num bytes in output array.
  byte count          = 0;     // Num 7bytes in a block.
  outSysEx[0]         = 0;

  for (unsigned i = 0; i < inLength; ++i)
  {
    const byte data = inData[i];
    const byte msb  = data >> 7;
    const byte body = data & 0x7f;

    outSysEx[0] |= (msb << (6 - count));
    outSysEx[1 + count] = body;

    if (count++ == 6)
    {
      outSysEx   += 8;
      outLength  += 8;
      outSysEx[0] = 0;
      count       = 0;
    }
  }
  return outLength + count + (count != 0 ? 1 : 0);
}

/*! \brief Decode System Exclusive messages.
  SysEx messages are encoded to guarantee transmission of data bytes higher than
  127 without breaking the MIDI protocol. Use this static method to reassemble
  your received message.
  \param inSysEx The SysEx data received from MIDI in.
  \param outData    The output buffer where to store the decrypted message.
  \param inLength The lenght of the input buffer.
  \return The lenght of the output buffer.
  @see encodeSysEx @see getSysExArrayLength
  Code inspired from Ruin & Wesen's SysEx encoder/decoder - http://ruinwesen.com
*/
unsigned decodeSysEx(const byte* inSysEx, byte* outData, unsigned inLength)
{
  unsigned count  = 0;
  byte msbStorage = 0;
  byte byteIndex  = 0;

  for (unsigned i = 0; i < inLength; ++i)
  {
    if ((i % 8) == 0)
    {
      msbStorage = inSysEx[i];
      byteIndex  = 6;
    }
    else
    {
      const byte body = inSysEx[i];
      const byte msb  = ((msbStorage >> byteIndex--) & 1) << 7;
      outData[count++] = msb | body;
    }
  }
  return count;
}

END_MIDI_NAMESPACE
