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

#include "XE_MIDI_Namespace.h"
#include "XE_MIDI_Defs.h"

BEGIN_MIDI_NAMESPACE

/*! The Message structure contains decoded data of a MIDI message
    read from the serial port with read()
*/
template<unsigned SysExMaxSize>
struct Message
{
  /*! Default constructor
    \n Initializes the attributes with their default values.
  */
  inline Message()
    : channel(0)
    , type(midi::InvalidType)
    , data1(0)
    , data2(0)
    , valid(false)
  {
    memset(sysexArray, 0, sSysExMaxSize * sizeof(DataByte));
  }

  /*! The maximum size for the System Exclusive array.
  */
  static const unsigned sSysExMaxSize = SysExMaxSize;

  /*! The MIDI channel on which the message was recieved.
    \n Value goes from 1 to 16.
  */
  Channel channel;

  /*! The type of the message
    (see the MidiType enum for types reference)
  */
  MidiType type;

  /*! The first data byte.
    \n Value goes from 0 to 127.
  */
  DataByte data1;

  /*! The second data byte.
    If the message is only 2 bytes long, this one is null.
    \n Value goes from 0 to 127.
  */
  DataByte data2;

  /*! System Exclusive dedicated byte array.
    \n Array length is stocked on 16 bits,
    in data1 (LSB) and data2 (MSB)
  */
  DataByte sysexArray[sSysExMaxSize];

  /*! This boolean indicates if the message is valid or not.
    There is no channel consideration here,
    validity means the message respects the MIDI norm.
  */
  bool valid;

  inline unsigned getSysExSize() const
  {
    const unsigned size = unsigned(data2) << 8 | data1;
    return size > sSysExMaxSize ? sSysExMaxSize : size;
  }
};

END_MIDI_NAMESPACE
