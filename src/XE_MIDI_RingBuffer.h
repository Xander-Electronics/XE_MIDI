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

BEGIN_MIDI_NAMESPACE

template<typename DataType, int Size>
class RingBuffer
{
  public:
    RingBuffer();
    ~RingBuffer();

  public:
    int getLength() const;
    bool isEmpty() const;

  public:
    void write(DataType inData);
    void write(const DataType* inData, int inSize);
    void clear();

  public:
    DataType read();
    void read(DataType* outData, int inSize);

  private:
    DataType mData[Size];
    DataType* mWriteHead;
    DataType* mReadHead;
};

END_MIDI_NAMESPACE

#include "midi_RingBuffer.hpp"
