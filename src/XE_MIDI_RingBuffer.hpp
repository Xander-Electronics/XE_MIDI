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

BEGIN_MIDI_NAMESPACE

template<typename DataType, int Size>
RingBuffer<DataType, Size>::RingBuffer()
  : mWriteHead(mData)
  , mReadHead(mData)
{
  memset(mData, DataType(0), Size * sizeof(DataType));
}

template<typename DataType, int Size>
RingBuffer<DataType, Size>::~RingBuffer()
{
}

// -----------------------------------------------------------------------------

template<typename DataType, int Size>
int RingBuffer<DataType, Size>::getLength() const
{
  if (mReadHead == mWriteHead)
  {
    return 0;
  }
  else if (mWriteHead > mReadHead)
  {
    return int(mWriteHead - mReadHead);
  }
  else
  {
    return int(mWriteHead - mData) + Size - int(mReadHead - mData);
  }
}

template<typename DataType, int Size>
bool RingBuffer<DataType, Size>::isEmpty() const
{
  return mReadHead == mWriteHead;
}

// -----------------------------------------------------------------------------

template<typename DataType, int Size>
void RingBuffer<DataType, Size>::write(DataType inData)
{
  *mWriteHead++ = inData;
  if (mWriteHead >= mData + Size)
  {
    mWriteHead = mData;
  }
}

template<typename DataType, int Size>
void RingBuffer<DataType, Size>::write(const DataType* inData, int inSize)
{
  for (int i = 0; i < inSize; ++i)
  {
    write(inData[i]);
  }
}

template<typename DataType, int Size>
void RingBuffer<DataType, Size>::clear()
{
  memset(mData, DataType(0), Size * sizeof(DataType));
  mReadHead  = mData;
  mWriteHead = mData;
}

// -----------------------------------------------------------------------------

template<typename DataType, int Size>
DataType RingBuffer<DataType, Size>::read()
{
  const DataType data = *mReadHead++;
  if (mReadHead >= mData + Size)
  {
    mReadHead = mData;
  }
  return data;
}

template<typename DataType, int Size>
void RingBuffer<DataType, Size>::read(DataType* outData, int inSize)
{
  for (int i = 0; i < inSize; ++i)
  {
    outData[i] = read();
  }
}

END_MIDI_NAMESPACE
