/*
  ==============================================================================

   This file is part of the JUCE 9 preview.
   Copyright (c) Raw Material Software Limited

   You may use this code under the terms of the AGPLv3
   (see www.gnu.org/licenses).

   For the JUCE 9 preview this file cannot be licensed commercially.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

Uuid::Uuid()
{
    Random r;

    for (size_t i = 0; i < sizeof (uuid); ++i)
        uuid[i] = (uint8) (r.nextInt (256));

    // To make it RFC 4122 compliant, need to force a few bits...
    uuid[6] = (uuid[6] & 0x0f) | 0x40;
    uuid[8] = (uuid[8] & 0x3f) | 0x80;
}

Uuid::Uuid (const Uuid& other) noexcept
{
    std::copy (other.uuid, other.uuid + sizeInBytes, uuid);
}

Uuid& Uuid::operator= (const Uuid& other) noexcept
{
    Uuid { other }.swap (*this);
    return *this;
}

bool Uuid::operator== (const Uuid& other) const noexcept    { return memcmp (uuid, other.uuid, sizeof (uuid)) == 0; }
bool Uuid::operator!= (const Uuid& other) const noexcept    { return ! operator== (other); }

bool Uuid::operator<  (const Uuid& other) const noexcept    { return compare (other) < 0; }
bool Uuid::operator>  (const Uuid& other) const noexcept    { return compare (other) > 0; }
bool Uuid::operator<= (const Uuid& other) const noexcept    { return compare (other) <= 0; }
bool Uuid::operator>= (const Uuid& other) const noexcept    { return compare (other) >= 0; }

int Uuid::compare (Uuid other) const noexcept
{
    for (size_t i = 0; i < sizeof (uuid); ++i)
        if (int diff = uuid[i] - (int) other.uuid[i])
            return diff > 0 ? 1 : -1;

    return 0;
}

Uuid Uuid::null() noexcept
{
    return Uuid ((const uint8*) nullptr);
}

bool Uuid::isNull() const noexcept
{
    for (auto i : uuid)
        if (i != 0)
            return false;

    return true;
}

String Uuid::getHexRegion (int start, int length) const
{
    return String::toHexString (uuid + start, length, 0);
}

String Uuid::toString() const
{
    return getHexRegion (0, 16);
}

String Uuid::toDashedString() const
{
    return getHexRegion (0, 4)
            + "-" + getHexRegion (4, 2)
            + "-" + getHexRegion (6, 2)
            + "-" + getHexRegion (8, 2)
            + "-" + getHexRegion (10, 6);
}

String Uuid::toArrayString() const
{
    String arrayUUID = "{ ";
    for (int i = 0; i < 16; i++)
        arrayUUID += "0x" + getHexRegion(i, 1) + (i < 15 ? ", " : "");
    return arrayUUID + " }";
}

Uuid::Uuid (const String& uuidString)
{
    MemoryBlock mb;
    mb.loadFromHexString (uuidString);
    mb.ensureSize (sizeof (uuid), true);
    mb.copyTo (uuid, 0, sizeof (uuid));
}

Uuid& Uuid::operator= (const String& uuidString)
{
    Uuid { uuidString }.swap (*this);
    return *this;
}

Uuid::Uuid (const uint8* const rawData) noexcept
{
    operator= (rawData);
}

Uuid& Uuid::operator= (const uint8* const rawData) noexcept
{
    if (rawData != nullptr)
        memcpy (uuid, rawData, sizeof (uuid));
    else
        zeromem (uuid, sizeof (uuid));

    return *this;
}

uint32 Uuid::getTimeLow() const noexcept                  { return ByteOrder::bigEndianInt (uuid); }
uint16 Uuid::getTimeMid() const noexcept                  { return ByteOrder::bigEndianShort (uuid + 4); }
uint16 Uuid::getTimeHighAndVersion() const noexcept       { return ByteOrder::bigEndianShort (uuid + 6); }
uint8  Uuid::getClockSeqAndReserved() const noexcept      { return uuid[8]; }
uint8  Uuid::getClockSeqLow() const noexcept              { return uuid[9]; }
uint64 Uuid::getNode() const noexcept                     { return (((uint64) ByteOrder::bigEndianShort (uuid + 10)) << 32) + ByteOrder::bigEndianInt (uuid + 12); }

uint64 Uuid::hash() const noexcept
{
    uint64 result = 0;

    for (auto n : uuid)
        result = ((uint64) 101) * result + n;

    return result;
}

void Uuid::swap (Uuid& other) noexcept
{
    uint8 temp[sizeInBytes];
    std::copy (uuid, uuid + sizeInBytes, temp);
    std::copy (other.uuid, other.uuid + sizeInBytes, uuid);
    std::copy (temp, temp + sizeInBytes, other.uuid);
}

} // namespace juce
