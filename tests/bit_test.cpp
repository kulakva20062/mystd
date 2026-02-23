#include <gtest/gtest.h>
#include "../lib/bit.hpp"
#include <cstdint>
#include <cstddef>

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

using namespace mystd;

// ============================================================================
// GetBit — 20 tests
// ============================================================================

TEST(BitTest, GetBit_Bit0OfZero) {
    uint8_t v = 0;
    ASSERT_FALSE(GetBit(&v, 0));
}

TEST(BitTest, GetBit_Bit0OfOne) {
    uint8_t v = 1;
    ASSERT_TRUE(GetBit(&v, 0));
}

TEST(BitTest, GetBit_Bit7Of0x80) {
    uint8_t v = 0x80;
    ASSERT_TRUE(GetBit(&v, 7));
}

TEST(BitTest, GetBit_Bit7Of0x7F) {
    uint8_t v = 0x7F;
    ASSERT_FALSE(GetBit(&v, 7));
}

TEST(BitTest, GetBit_AllBitsOf0xFF) {
    uint8_t v = 0xFF;
    for (size_t i = 0; i < 8; ++i)
        ASSERT_TRUE(GetBit(&v, i)) << "bit " << i;
}

TEST(BitTest, GetBit_Bit8Of0x0100) {
    uint16_t v = 0x0100;
    ASSERT_TRUE(GetBit(&v, 8));
}

TEST(BitTest, GetBit_Bit15Of0x8000) {
    uint16_t v = 0x8000;
    ASSERT_TRUE(GetBit(&v, 15));
}

TEST(BitTest, GetBit_AllBitsOfZero) {
    uint16_t v = 0;
    for (size_t i = 0; i < 16; ++i)
        ASSERT_FALSE(GetBit(&v, i)) << "bit " << i;
}

TEST(BitTest, GetBit_Bits0And7Of0x81) {
    uint8_t v = 0x81;
    ASSERT_TRUE(GetBit(&v, 0));
    ASSERT_TRUE(GetBit(&v, 7));
}

TEST(BitTest, GetBit_Bit4Of0x10) {
    uint8_t v = 0x10;
    ASSERT_TRUE(GetBit(&v, 4));
}

TEST(BitTest, GetBit_ByteBoundaryBits7And8) {
    uint16_t v = 0x0180;  // bit 7 and bit 8 set
    ASSERT_TRUE(GetBit(&v, 7));
    ASSERT_TRUE(GetBit(&v, 8));
}

TEST(BitTest, GetBit_Bit31Of0x80000000) {
    uint32_t v = 0x80000000u;
    ASSERT_TRUE(GetBit(&v, 31));
}

TEST(BitTest, GetBit_Bit16Of0x00010000) {
    uint32_t v = 0x00010000u;
    ASSERT_TRUE(GetBit(&v, 16));
}

TEST(BitTest, GetBit_Bit63OfUint64) {
    uint64_t v = 1ULL << 63;
    ASSERT_TRUE(GetBit(&v, 63));
}

TEST(BitTest, GetBit_Bit32OfUint64) {
    uint64_t v = 1ULL << 32;
    ASSERT_TRUE(GetBit(&v, 32));
}

TEST(BitTest, GetBit_AlternatingBits) {
    uint8_t v = 0x55;  // 01010101
    ASSERT_TRUE(GetBit(&v, 0));
    ASSERT_FALSE(GetBit(&v, 1));
    ASSERT_TRUE(GetBit(&v, 2));
    ASSERT_FALSE(GetBit(&v, 3));
}

TEST(BitTest, GetBit_Uint8MaxAllTrue) {
    uint8_t v = 0xFF;
    for (size_t i = 0; i < 8; ++i)
        ASSERT_TRUE(GetBit(&v, i));
}

TEST(BitTest, GetBit_SignedInt) {
    int32_t v = -1;  // all bits set
    for (size_t i = 0; i < 32; ++i)
        ASSERT_TRUE(GetBit(&v, i));
}

TEST(BitTest, GetBit_MiddleOfByte) {
    uint8_t v = 0x38;  // bits 3, 4, 5 set
    ASSERT_FALSE(GetBit(&v, 2));
    ASSERT_TRUE(GetBit(&v, 3));
    ASSERT_TRUE(GetBit(&v, 4));
    ASSERT_TRUE(GetBit(&v, 5));
    ASSERT_FALSE(GetBit(&v, 6));
}

TEST(BitTest, GetBit_Bits123Of0x0E) {
    uint8_t v = 0x0E;  // bits 1, 2, 3 set
    ASSERT_FALSE(GetBit(&v, 0));
    ASSERT_TRUE(GetBit(&v, 1));
    ASSERT_TRUE(GetBit(&v, 2));
    ASSERT_TRUE(GetBit(&v, 3));
    ASSERT_FALSE(GetBit(&v, 4));
}

// ============================================================================
// SetBit — 20 tests
// ============================================================================

TEST(BitTest, SetBit_SetBit0To1OnZero) {
    uint8_t v = 0;
    SetBit(&v, 0, true);
    ASSERT_EQ(v, 1u);
}

TEST(BitTest, SetBit_SetBit0To0OnOne) {
    uint8_t v = 1;
    SetBit(&v, 0, false);
    ASSERT_EQ(v, 0u);
}

TEST(BitTest, SetBit_SetBits012_VerifyWithGetBit) {
    uint8_t v = 0;
    SetBit(&v, 0, true);
    SetBit(&v, 1, true);
    SetBit(&v, 2, true);
    ASSERT_TRUE(GetBit(&v, 0));
    ASSERT_TRUE(GetBit(&v, 1));
    ASSERT_TRUE(GetBit(&v, 2));
    ASSERT_EQ(v, 7u);
}

TEST(BitTest, SetBit_SetBit7) {
    uint8_t v = 0;
    SetBit(&v, 7, true);
    ASSERT_EQ(v, 0x80u);
}

TEST(BitTest, SetBit_SetBit8OnUint16) {
    uint16_t v = 0;
    SetBit(&v, 8, true);
    ASSERT_EQ(v, 0x0100u);
}

TEST(BitTest, SetBit_RoundtripWithGetBit) {
    uint32_t v = 0x12345678u;
    SetBit(&v, 10, true);
    ASSERT_TRUE(GetBit(&v, 10));
    SetBit(&v, 10, false);
    ASSERT_FALSE(GetBit(&v, 10));
}

TEST(BitTest, SetBit_Set1Then0_Rollback) {
    uint8_t v = 0;
    SetBit(&v, 3, true);
    ASSERT_EQ(v, 8u);
    SetBit(&v, 3, false);
    ASSERT_EQ(v, 0u);
}

TEST(BitTest, SetBit_SetBits07815) {
    uint16_t v = 0;
    SetBit(&v, 0, true);
    SetBit(&v, 7, true);
    SetBit(&v, 8, true);
    SetBit(&v, 15, true);
    ASSERT_EQ(v, 0x8181u);
}

TEST(BitTest, SetBit_OtherBitsUnchanged) {
    uint8_t v = 0x42;  // bits 1 and 6
    SetBit(&v, 4, true);
    ASSERT_TRUE(GetBit(&v, 1));
    ASSERT_TRUE(GetBit(&v, 4));
    ASSERT_TRUE(GetBit(&v, 6));
}

TEST(BitTest, SetBit_AllBitsUint8OneByOne) {
    uint8_t v = 0;
    for (size_t i = 0; i < 8; ++i)
        SetBit(&v, i, true);
    ASSERT_EQ(v, 0xFFu);
}

TEST(BitTest, SetBit_Uint32Bits01631) {
    uint32_t v = 0;
    SetBit(&v, 0, true);
    SetBit(&v, 16, true);
    SetBit(&v, 31, true);
    ASSERT_EQ(v, 0x80010001u);
}

TEST(BitTest, SetBit_Uint64Bits03263) {
    uint64_t v = 0;
    SetBit(&v, 0, true);
    SetBit(&v, 32, true);
    SetBit(&v, 63, true);
    ASSERT_EQ(v, (1ULL << 63) | (1ULL << 32) | 1ULL);
}

TEST(BitTest, SetBit_ClearOneBitIn0xFF) {
    uint8_t v = 0xFF;
    SetBit(&v, 4, false);
    ASSERT_EQ(v, 0xEFu);
}

TEST(BitTest, SetBit_SetOneBitInZero) {
    uint8_t v = 0;
    SetBit(&v, 5, true);
    ASSERT_EQ(v, 32u);
}

TEST(BitTest, SetBit_BoundaryIndices7And8) {
    uint16_t v = 0;
    SetBit(&v, 7, true);
    SetBit(&v, 8, true);
    ASSERT_TRUE(GetBit(&v, 7));
    ASSERT_TRUE(GetBit(&v, 8));
    ASSERT_EQ(v, 0x0180u);
}

TEST(BitTest, SetBit_MiddleOfByteBit4) {
    uint8_t v = 0;
    SetBit(&v, 4, true);
    ASSERT_EQ(v, 0x10u);
}

TEST(BitTest, SetBit_TwoBytesBits7And8) {
    uint16_t v = 0;
    SetBit(&v, 7, true);
    SetBit(&v, 8, true);
    ASSERT_EQ(v, 0x0180u);
}

TEST(BitTest, SetBit_DifferentTypes) {
    uint8_t v8 = 0;
    uint16_t v16 = 0;
    uint32_t v32 = 0;
    SetBit(&v8, 0, true);
    SetBit(&v16, 8, true);
    SetBit(&v32, 16, true);
    ASSERT_EQ(v8, 1u);
    ASSERT_EQ(v16, 0x0100u);
    ASSERT_EQ(v32, 0x00010000u);
}

TEST(BitTest, SetBit_SequentialSet1ThenSet0) {
    uint8_t v = 0xAA;
    SetBit(&v, 0, true);
    SetBit(&v, 0, false);
    ASSERT_EQ(v, 0xAAu);
}

TEST(BitTest, SetBit_MultipleInDifferentBytes) {
    uint32_t v = 0;
    SetBit(&v, 0, true);
    SetBit(&v, 8, true);
    SetBit(&v, 16, true);
    SetBit(&v, 24, true);
    ASSERT_EQ(v, 0x01010101u);
}

// ============================================================================
// ToggleBit — 20 tests
// ============================================================================

TEST(BitTest, ToggleBit_ToggleBit0From0) {
    uint8_t v = 0;
    ToggleBit(&v, 0);
    ASSERT_EQ(v, 1u);
}

TEST(BitTest, ToggleBit_ToggleTwiceReturnsOriginal) {
    uint8_t v = 0x42;
    ToggleBit(&v, 3);
    ToggleBit(&v, 3);
    ASSERT_EQ(v, 0x42u);
}

TEST(BitTest, ToggleBit_ToggleBits012) {
    uint8_t v = 0;
    ToggleBit(&v, 0);
    ToggleBit(&v, 1);
    ToggleBit(&v, 2);
    ASSERT_EQ(v, 7u);
}

TEST(BitTest, ToggleBit_ToggleBit7) {
    uint8_t v = 0;
    ToggleBit(&v, 7);
    ASSERT_EQ(v, 0x80u);
}

TEST(BitTest, ToggleBit_ToggleBit8) {
    uint16_t v = 0;
    ToggleBit(&v, 8);
    ASSERT_EQ(v, 0x0100u);
}

TEST(BitTest, ToggleBit_ToggleAllBits0To0xFF) {
    uint8_t v = 0;
    for (size_t i = 0; i < 8; ++i)
        ToggleBit(&v, i);
    ASSERT_EQ(v, 0xFFu);
}

TEST(BitTest, ToggleBit_Toggle0xFFTo0) {
    uint8_t v = 0xFF;
    for (size_t i = 0; i < 8; ++i)
        ToggleBit(&v, i);
    ASSERT_EQ(v, 0u);
}

TEST(BitTest, ToggleBit_BoundaryBits7And8) {
    uint16_t v = 0;
    ToggleBit(&v, 7);
    ToggleBit(&v, 8);
    ASSERT_TRUE(GetBit(&v, 7));
    ASSERT_TRUE(GetBit(&v, 8));
}

TEST(BitTest, ToggleBit_VerifyWithGetBit) {
    uint8_t v = 0x33;
    ToggleBit(&v, 2);
    ASSERT_TRUE(GetBit(&v, 2));
    ToggleBit(&v, 2);
    ASSERT_FALSE(GetBit(&v, 2));
}

TEST(BitTest, ToggleBit_SameBitFiveTimes) {
    uint8_t v = 0;
    for (int i = 0; i < 5; ++i)
        ToggleBit(&v, 4);
    ASSERT_EQ(v, 0x10u);  // odd number of toggles = 1
}

TEST(BitTest, ToggleBit_Uint16Bits08And15) {
    uint16_t v = 0;
    ToggleBit(&v, 0);
    ToggleBit(&v, 8);
    ToggleBit(&v, 15);
    ASSERT_EQ(v, 0x8101u);
}

TEST(BitTest, ToggleBit_Uint32Bits0And31) {
    uint32_t v = 0;
    ToggleBit(&v, 0);
    ToggleBit(&v, 31);
    ASSERT_EQ(v, 0x80000001u);
}

TEST(BitTest, ToggleBit_Uint64Bit63) {
    uint64_t v = 0;
    ToggleBit(&v, 63);
    ASSERT_EQ(v, 1ULL << 63);
}

TEST(BitTest, ToggleBit_AlternatingBits) {
    uint8_t v = 0x0F;
    ToggleBit(&v, 0);
    ToggleBit(&v, 1);
    ToggleBit(&v, 2);
    ToggleBit(&v, 3);
    ASSERT_EQ(v, 0u);
}

TEST(BitTest, ToggleBit_DifferentBytes) {
    uint32_t v = 0;
    ToggleBit(&v, 0);
    ToggleBit(&v, 8);
    ToggleBit(&v, 16);
    ToggleBit(&v, 24);
    ASSERT_EQ(v, 0x01010101u);
}

TEST(BitTest, ToggleBit_MiddleBit) {
    uint8_t v = 0;
    ToggleBit(&v, 4);
    ASSERT_EQ(v, 0x10u);
}

TEST(BitTest, ToggleBit_0x55To0xAA) {
    uint8_t v = 0x55; //01010101
    for (size_t i = 0; i < 8; ++i)
        ToggleBit(&v, i); //10101010
    ASSERT_EQ(v, 0xAAu);
}

TEST(BitTest, ToggleBit_AfterSetBit) {
    uint8_t v = 0;
    SetBit(&v, 3, true);
    ToggleBit(&v, 3);
    ASSERT_FALSE(GetBit(&v, 3));
}

TEST(BitTest, ToggleBit_ConsecutiveBits) {
    uint8_t v = 0;
    ToggleBit(&v, 2);
    ToggleBit(&v, 3);
    ToggleBit(&v, 4);
    ASSERT_EQ(v, 28u);
}

TEST(BitTest, ToggleBit_Bit16Uint32) {
    uint32_t v = 0;
    ToggleBit(&v, 16);
    ASSERT_EQ(v, 0x00010000u);
}

// ============================================================================
// SetLineBits — 20 tests
// ============================================================================

TEST(BitTest, SetLineBits_FullByte0to8Set1) {
    uint8_t v = 0;
    SetLineBits(&v, 0, 8, true);
    ASSERT_EQ(v, 0xFFu);
}

TEST(BitTest, SetLineBits_FullByte0to8Set0) {
    uint8_t v = 0xFF;
    SetLineBits(&v, 0, 8, false);
    ASSERT_EQ(v, 0u);
}

TEST(BitTest, SetLineBits_TwoBytes0to16) {
    uint16_t v = 0;
    SetLineBits(&v, 0, 16, true);
    ASSERT_EQ(v, 0xFFFFu);
}

TEST(BitTest, SetLineBits_PartialBytes4to12) {
    uint16_t v = 0;
    SetLineBits(&v, 4, 12, true);
    ASSERT_EQ(v, 0x0FF0u);
}

TEST(BitTest, SetLineBits_SingleBit0to1) {
    uint8_t v = 0;
    SetLineBits(&v, 0, 1, true);
    ASSERT_EQ(v, 1u);
}

TEST(BitTest, SetLineBits_BitOnByteBoundary7to8) {
    uint8_t v = 0;
    SetLineBits(&v, 7, 8, true);
    ASSERT_EQ(v, 0x80u);
}

TEST(BitTest, SetLineBits_FirstBitOfSecondByte8to9) {
    uint16_t v = 0;
    SetLineBits(&v, 8, 9, true);
    ASSERT_EQ(v, 0x0100u);
}

TEST(BitTest, SetLineBits_HalfByte0to4) {
    uint8_t v = 0;
    SetLineBits(&v, 0, 4, true);
    ASSERT_EQ(v, 0x0Fu);
}

TEST(BitTest, SetLineBits_SecondHalf4to8) {
    uint8_t v = 0;
    SetLineBits(&v, 4, 8, true);
    ASSERT_EQ(v, 0xF0u);
}

TEST(BitTest, SetLineBits_FullUint320to32) {
    uint32_t v = 0;
    SetLineBits(&v, 0, 32, true);
    ASSERT_EQ(v, 0xFFFFFFFFu);
}

TEST(BitTest, SetLineBits_MiddleOfUint6416to48) {
    uint64_t v = 0;
    SetLineBits(&v, 16, 48, true);
    ASSERT_EQ(v, 0x0000FFFFFFFF0000ULL);
}

TEST(BitTest, SetLineBits_FullUint640to64) {
    uint64_t v = 0;
    SetLineBits(&v, 0, 64, true);
    ASSERT_EQ(v, 0xFFFFFFFFFFFFFFFFULL);
}

TEST(BitTest, SetLineBits_SingleBitInMiddle3to4) {
    uint8_t v = 0;
    SetLineBits(&v, 3, 4, true);
    ASSERT_EQ(v, 8u);
}

TEST(BitTest, SetLineBits_MiddleOfByte1to7) {
    uint8_t v = 0;
    SetLineBits(&v, 1, 7, true);
    ASSERT_EQ(v, 0x7Eu);
}

TEST(BitTest, SetLineBits_ThreeBytes0to24) {
    uint32_t v = 0;
    SetLineBits(&v, 0, 24, true);
    ASSERT_EQ(v, 0x00FFFFFFu);
}

TEST(BitTest, SetLineBits_OtherBitsUnchanged) {
    uint16_t v = 0xFF00;  // high byte set
    SetLineBits(&v, 4, 8, false);  // clear bits 4-7
    ASSERT_EQ(v, 0xFF00u);
}

TEST(BitTest, SetLineBits_VerifyWithGetBit) {
    uint16_t v = 0;
    SetLineBits(&v, 2, 6, true);
    for (size_t i = 2; i < 6; ++i)
        ASSERT_TRUE(GetBit(&v, i)) << "bit " << i;
    ASSERT_FALSE(GetBit(&v, 1));
    ASSERT_FALSE(GetBit(&v, 6));
}

TEST(BitTest, SetLineBits_FullSecondByte8to16) {
    uint16_t v = 0;
    SetLineBits(&v, 8, 16, true);
    ASSERT_EQ(v, 0xFF00u);
}

TEST(BitTest, SetLineBits_TwoEdgeChunks) {
    uint8_t v = 0;
    SetLineBits(&v, 0, 2, true);
    SetLineBits(&v, 6, 8, true);
    ASSERT_EQ(v, 0xC3u);  // bits 0,1,6,7
}

TEST(BitTest, SetLineBits_OverlapWithExistingBits) {
    uint8_t v = 0xFF;
    SetLineBits(&v, 2, 6, false);
    ASSERT_EQ(v, 0xC3u);  // only bits 0,1,6,7 remain
}

#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

// В Big-Endian бит 0 = MSB первого байта в памяти.
// Маска для бита i внутри байта: 1 << (CHAR_BIT - 1 - i%CHAR_BIT).
// Примеры для uint8_t:
//   bit 0 → mask 0x80,  bit 4 → mask 0x08,  bit 7 → mask 0x01

using namespace mystd;

// ============================================================================
// GetBit — 20 tests
// ============================================================================

TEST(BitTest, GetBit_Bit0OfZero) {
    uint8_t v = 0;
    ASSERT_FALSE(GetBit(&v, 0));
}

TEST(BitTest, GetBit_Bit0Of0x80) {
    uint8_t v = 0x80;  // bit 0 = MSB = 1
    ASSERT_TRUE(GetBit(&v, 0));
}

TEST(BitTest, GetBit_Bit7Of0x01) {
    uint8_t v = 0x01;  // bit 7 = LSB = 1
    ASSERT_TRUE(GetBit(&v, 7));
}

TEST(BitTest, GetBit_Bit0Of0x7F) {
    uint8_t v = 0x7F;  // bit 0 = MSB = 0
    ASSERT_FALSE(GetBit(&v, 0));
}

TEST(BitTest, GetBit_AllBitsOf0xFF) {
    uint8_t v = 0xFF;
    for (size_t i = 0; i < 8; ++i)
        ASSERT_TRUE(GetBit(&v, i)) << "bit " << i;
}

TEST(BitTest, GetBit_Bit8Of0x0080) {
    uint8_t mem[2] = {0x00, 0x80};  // explicit BE memory; bit 8 = MSB of byte 1
    ASSERT_TRUE(GetBit(mem, 8));
}

TEST(BitTest, GetBit_Bit15Of0x0001) {
    uint8_t mem[2] = {0x00, 0x01};  // explicit BE memory; bit 15 = LSB of byte 1
    ASSERT_TRUE(GetBit(mem, 15));
}

TEST(BitTest, GetBit_AllBitsOfZero) {
    uint16_t v = 0;
    for (size_t i = 0; i < 16; ++i)
        ASSERT_FALSE(GetBit(&v, i)) << "bit " << i;
}

TEST(BitTest, GetBit_Bits0And7Of0x81) {
    uint8_t v = 0x81;  // MSB and LSB both set
    ASSERT_TRUE(GetBit(&v, 0));
    ASSERT_TRUE(GetBit(&v, 7));
}

TEST(BitTest, GetBit_Bit4Of0x08) {
    uint8_t v = 0x08;  // bit 4 from MSB → mask 0x08
    ASSERT_TRUE(GetBit(&v, 4));
}

TEST(BitTest, GetBit_ByteBoundaryBits7And8) {
    uint8_t mem[2] = {0x01, 0x80};  // bit7=LSB byte0=0x01, bit8=MSB byte1=0x80
    ASSERT_TRUE(GetBit(mem, 7));
    ASSERT_TRUE(GetBit(mem, 8));
}

TEST(BitTest, GetBit_Bit31Of0x00000001) {
    uint8_t mem[4] = {0x00, 0x00, 0x00, 0x01};  // bit 31 = LSB of byte 3
    ASSERT_TRUE(GetBit(mem, 31));
}

TEST(BitTest, GetBit_Bit16Of0x00008000) {
    uint8_t mem[4] = {0x00, 0x00, 0x80, 0x00};  // bit 16 = MSB of byte 2
    ASSERT_TRUE(GetBit(mem, 16));
}

TEST(BitTest, GetBit_Bit63OfUint64) {
    uint8_t mem[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};  // bit 63 = LSB of byte 7
    ASSERT_TRUE(GetBit(mem, 63));
}

TEST(BitTest, GetBit_Bit32OfUint64) {
    uint8_t mem[8] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00};  // bit 32 = MSB of byte 4
    ASSERT_TRUE(GetBit(mem, 32));
}

TEST(BitTest, GetBit_AlternatingBits) {
    uint8_t v = 0x55;  // 01010101
    ASSERT_FALSE(GetBit(&v, 0));  // MSB = 0
    ASSERT_TRUE(GetBit(&v, 1));
    ASSERT_FALSE(GetBit(&v, 2));
    ASSERT_TRUE(GetBit(&v, 3));
}

TEST(BitTest, GetBit_Uint8MaxAllTrue) {
    uint8_t v = 0xFF;
    for (size_t i = 0; i < 8; ++i)
        ASSERT_TRUE(GetBit(&v, i));
}

TEST(BitTest, GetBit_SignedInt) {
    int32_t v = -1;  // all bits set
    for (size_t i = 0; i < 32; ++i)
        ASSERT_TRUE(GetBit(&v, i));
}

TEST(BitTest, GetBit_MiddleOfByte) {
    uint8_t v = 0x38;  // 00111000: bits 2,3,4 set in MSB-first numbering
    ASSERT_FALSE(GetBit(&v, 1));
    ASSERT_TRUE(GetBit(&v, 2));
    ASSERT_TRUE(GetBit(&v, 3));
    ASSERT_TRUE(GetBit(&v, 4));
    ASSERT_FALSE(GetBit(&v, 5));
}

TEST(BitTest, GetBit_Bits456Of0x0E) {
    uint8_t v = 0x0E;  // 00001110: bits 4,5,6 set in MSB-first numbering
    ASSERT_FALSE(GetBit(&v, 3));
    ASSERT_TRUE(GetBit(&v, 4));
    ASSERT_TRUE(GetBit(&v, 5));
    ASSERT_TRUE(GetBit(&v, 6));
    ASSERT_FALSE(GetBit(&v, 7));
}

// ============================================================================
// SetBit — 20 tests
// ============================================================================

TEST(BitTest, SetBit_SetBit0To1OnZero) {
    uint8_t v = 0;
    SetBit(&v, 0, true);
    ASSERT_EQ(v, 0x80u);  // MSB set
}

TEST(BitTest, SetBit_SetBit0To0On0x80) {
    uint8_t v = 0x80;
    SetBit(&v, 0, false);
    ASSERT_EQ(v, 0u);
}

TEST(BitTest, SetBit_SetBits012_VerifyWithGetBit) {
    uint8_t v = 0;
    SetBit(&v, 0, true);
    SetBit(&v, 1, true);
    SetBit(&v, 2, true);
    ASSERT_TRUE(GetBit(&v, 0));
    ASSERT_TRUE(GetBit(&v, 1));
    ASSERT_TRUE(GetBit(&v, 2));
    ASSERT_EQ(v, 0xE0u);  // top 3 bits set
}

TEST(BitTest, SetBit_SetBit7) {
    uint8_t v = 0;
    SetBit(&v, 7, true);
    ASSERT_EQ(v, 0x01u);  // LSB set
}

TEST(BitTest, SetBit_SetBit8OnUint16) {
    uint8_t mem[2] = {};
    SetBit(mem, 8, true);
    ASSERT_EQ(mem[0], 0x00u);
    ASSERT_EQ(mem[1], 0x80u);  // MSB of byte 1
}

TEST(BitTest, SetBit_RoundtripWithGetBit) {
    uint32_t v = 0x12345678u;
    SetBit(&v, 10, true);
    ASSERT_TRUE(GetBit(&v, 10));
    SetBit(&v, 10, false);
    ASSERT_FALSE(GetBit(&v, 10));
}

TEST(BitTest, SetBit_Set1Then0_Rollback) {
    uint8_t v = 0;
    SetBit(&v, 3, true);
    ASSERT_EQ(v, 0x10u);  // bit 3 from MSB → mask 0x10
    SetBit(&v, 3, false);
    ASSERT_EQ(v, 0u);
}

TEST(BitTest, SetBit_SetBits07815) {
    uint16_t v = 0;
    SetBit(&v, 0, true);
    SetBit(&v, 7, true);
    SetBit(&v, 8, true);
    SetBit(&v, 15, true);
    ASSERT_EQ(v, 0x8181u);
}

TEST(BitTest, SetBit_OtherBitsUnchanged) {
    uint8_t v = 0x42;  // 01000010: bits 1 and 6 set in MSB-first
    SetBit(&v, 4, true);
    ASSERT_TRUE(GetBit(&v, 1));
    ASSERT_TRUE(GetBit(&v, 4));
    ASSERT_TRUE(GetBit(&v, 6));
}

TEST(BitTest, SetBit_AllBitsUint8OneByOne) {
    uint8_t v = 0;
    for (size_t i = 0; i < 8; ++i)
        SetBit(&v, i, true);
    ASSERT_EQ(v, 0xFFu);
}

TEST(BitTest, SetBit_Uint32Bits01631) {
    uint8_t mem[4] = {};
    SetBit(mem, 0, true);   // byte 0 MSB
    SetBit(mem, 16, true);  // byte 2 MSB
    SetBit(mem, 31, true);  // byte 3 LSB
    ASSERT_EQ(mem[0], 0x80u);
    ASSERT_EQ(mem[1], 0x00u);
    ASSERT_EQ(mem[2], 0x80u);
    ASSERT_EQ(mem[3], 0x01u);
}

TEST(BitTest, SetBit_Uint64Bits03263) {
    uint8_t mem[8] = {};
    SetBit(mem, 0, true);   // byte 0 MSB
    SetBit(mem, 32, true);  // byte 4 MSB
    SetBit(mem, 63, true);  // byte 7 LSB
    ASSERT_EQ(mem[0], 0x80u);
    ASSERT_EQ(mem[4], 0x80u);
    ASSERT_EQ(mem[7], 0x01u);
}

TEST(BitTest, SetBit_ClearOneBitIn0xFF) {
    uint8_t v = 0xFF;
    SetBit(&v, 4, false);  // mask ~0x08
    ASSERT_EQ(v, 0xF7u);
}

TEST(BitTest, SetBit_SetOneBitInZero) {
    uint8_t v = 0;
    SetBit(&v, 5, true);  // bit 5 from MSB → mask 0x04
    ASSERT_EQ(v, 0x04u);
}

TEST(BitTest, SetBit_BoundaryIndices7And8) {
    uint8_t mem[2] = {};
    SetBit(mem, 7, true);
    SetBit(mem, 8, true);
    ASSERT_TRUE(GetBit(mem, 7));
    ASSERT_TRUE(GetBit(mem, 8));
    ASSERT_EQ(mem[0], 0x01u);  // LSB of byte 0
    ASSERT_EQ(mem[1], 0x80u);  // MSB of byte 1
}

TEST(BitTest, SetBit_MiddleOfByteBit4) {
    uint8_t v = 0;
    SetBit(&v, 4, true);
    ASSERT_EQ(v, 0x08u);  // bit 4 from MSB → mask 0x08
}

TEST(BitTest, SetBit_TwoBytesBits7And8) {
    uint8_t mem[2] = {};
    SetBit(mem, 7, true);
    SetBit(mem, 8, true);
    ASSERT_EQ(mem[0], 0x01u);
    ASSERT_EQ(mem[1], 0x80u);
}

TEST(BitTest, SetBit_DifferentTypes) {
    uint8_t v8 = 0;
    SetBit(&v8, 0, true);
    ASSERT_EQ(v8, 0x80u);
    uint8_t mem16[2] = {};
    SetBit(mem16, 8, true);
    ASSERT_EQ(mem16[1], 0x80u);  // MSB of byte 1
    uint8_t mem32[4] = {};
    SetBit(mem32, 16, true);
    ASSERT_EQ(mem32[2], 0x80u);  // MSB of byte 2
}

TEST(BitTest, SetBit_SequentialSet1ThenSet0) {
    uint8_t v = 0x2A;  // 00101010: bit 0 (MSB) = 0
    SetBit(&v, 0, true);
    SetBit(&v, 0, false);
    ASSERT_EQ(v, 0x2Au);
}

TEST(BitTest, SetBit_MultipleInDifferentBytes) {
    uint32_t v = 0;
    SetBit(&v, 0, true);
    SetBit(&v, 8, true);
    SetBit(&v, 16, true);
    SetBit(&v, 24, true);
    ASSERT_EQ(v, 0x80808080u);  // MSB of each byte
}

// ============================================================================
// ToggleBit — 20 tests
// ============================================================================

TEST(BitTest, ToggleBit_ToggleBit0From0) {
    uint8_t v = 0;
    ToggleBit(&v, 0);
    ASSERT_EQ(v, 0x80u);
}

TEST(BitTest, ToggleBit_ToggleTwiceReturnsOriginal) {
    uint8_t v = 0x42;
    ToggleBit(&v, 3);
    ToggleBit(&v, 3);
    ASSERT_EQ(v, 0x42u);
}

TEST(BitTest, ToggleBit_ToggleBits012) {
    uint8_t v = 0;
    ToggleBit(&v, 0);
    ToggleBit(&v, 1);
    ToggleBit(&v, 2);
    ASSERT_EQ(v, 0xE0u);  // top 3 bits set
}

TEST(BitTest, ToggleBit_ToggleBit7) {
    uint8_t v = 0;
    ToggleBit(&v, 7);
    ASSERT_EQ(v, 0x01u);  // LSB set
}

TEST(BitTest, ToggleBit_ToggleBit8) {
    uint8_t mem[2] = {};
    ToggleBit(mem, 8);
    ASSERT_EQ(mem[0], 0x00u);
    ASSERT_EQ(mem[1], 0x80u);  // MSB of byte 1
}

TEST(BitTest, ToggleBit_ToggleAllBits0To0xFF) {
    uint8_t v = 0;
    for (size_t i = 0; i < 8; ++i)
        ToggleBit(&v, i);
    ASSERT_EQ(v, 0xFFu);
}

TEST(BitTest, ToggleBit_Toggle0xFFTo0) {
    uint8_t v = 0xFF;
    for (size_t i = 0; i < 8; ++i)
        ToggleBit(&v, i);
    ASSERT_EQ(v, 0u);
}

TEST(BitTest, ToggleBit_BoundaryBits7And8) {
    uint8_t mem[2] = {};
    ToggleBit(mem, 7);
    ToggleBit(mem, 8);
    ASSERT_TRUE(GetBit(mem, 7));
    ASSERT_TRUE(GetBit(mem, 8));
    ASSERT_EQ(mem[0], 0x01u);  // LSB of byte 0
    ASSERT_EQ(mem[1], 0x80u);  // MSB of byte 1
}

TEST(BitTest, ToggleBit_VerifyWithGetBit) {
    uint8_t v = 0x33;  // 00110011: bit 2 (MSB-first) is set
    ASSERT_TRUE(GetBit(&v, 2));
    ToggleBit(&v, 2);
    ASSERT_FALSE(GetBit(&v, 2));
    ToggleBit(&v, 2);
    ASSERT_TRUE(GetBit(&v, 2));
}

TEST(BitTest, ToggleBit_SameBitFiveTimes) {
    uint8_t v = 0;
    for (int i = 0; i < 5; ++i)
        ToggleBit(&v, 4);
    ASSERT_EQ(v, 0x08u);  // bit 4 from MSB, odd number of toggles = 1
}

TEST(BitTest, ToggleBit_Uint16Bits08And15) {
    uint8_t mem[2] = {};
    ToggleBit(mem, 0);
    ToggleBit(mem, 8);
    ToggleBit(mem, 15);
    ASSERT_EQ(mem[0], 0x80u);  // bit 0 = MSB of byte 0
    ASSERT_EQ(mem[1], 0x81u);  // bit 8 = MSB, bit 15 = LSB of byte 1
}

TEST(BitTest, ToggleBit_Uint32Bits0And31) {
    uint8_t mem[4] = {};
    ToggleBit(mem, 0);
    ToggleBit(mem, 31);
    ASSERT_EQ(mem[0], 0x80u);  // bit 0 = MSB of byte 0
    ASSERT_EQ(mem[3], 0x01u);  // bit 31 = LSB of byte 3
}

TEST(BitTest, ToggleBit_Uint64Bit63) {
    uint8_t mem[8] = {};
    ToggleBit(mem, 63);
    ASSERT_EQ(mem[7], 0x01u);  // bit 63 = LSB of byte 7
}

TEST(BitTest, ToggleBit_AlternatingBits) {
    uint8_t v = 0x0F;  // 00001111: upper nibble = 0
    ToggleBit(&v, 0);
    ToggleBit(&v, 1);
    ToggleBit(&v, 2);
    ToggleBit(&v, 3);
    ASSERT_EQ(v, 0xFFu);  // upper nibble flipped to 1, lower nibble stays 1
}

TEST(BitTest, ToggleBit_DifferentBytes) {
    uint32_t v = 0;
    ToggleBit(&v, 0);
    ToggleBit(&v, 8);
    ToggleBit(&v, 16);
    ToggleBit(&v, 24);
    ASSERT_EQ(v, 0x80808080u);  // MSB of each byte
}

TEST(BitTest, ToggleBit_MiddleBit) {
    uint8_t v = 0;
    ToggleBit(&v, 4);
    ASSERT_EQ(v, 0x08u);  // bit 4 from MSB → mask 0x08
}

TEST(BitTest, ToggleBit_0x55To0xAA) {
    uint8_t v = 0x55;  // 01010101
    for (size_t i = 0; i < 8; ++i)
        ToggleBit(&v, i);  // 10101010
    ASSERT_EQ(v, 0xAAu);
}

TEST(BitTest, ToggleBit_AfterSetBit) {
    uint8_t v = 0;
    SetBit(&v, 3, true);
    ASSERT_EQ(v, 0x10u);  // bit 3 from MSB
    ToggleBit(&v, 3);
    ASSERT_FALSE(GetBit(&v, 3));
}

TEST(BitTest, ToggleBit_ConsecutiveBits) {
    uint8_t v = 0;
    ToggleBit(&v, 2);
    ToggleBit(&v, 3);
    ToggleBit(&v, 4);
    ASSERT_EQ(v, 0x38u);  // bits 2,3,4 from MSB → 0b00111000
}

TEST(BitTest, ToggleBit_Bit16Uint32) {
    uint8_t mem[4] = {};
    ToggleBit(mem, 16);
    ASSERT_EQ(mem[2], 0x80u);  // bit 16 = MSB of byte 2
}

// ============================================================================
// SetLineBits — 20 tests
// ============================================================================

TEST(BitTest, SetLineBits_FullByte0to8Set1) {
    uint8_t v = 0;
    SetLineBits(&v, 0, 8, true);
    ASSERT_EQ(v, 0xFFu);
}

TEST(BitTest, SetLineBits_FullByte0to8Set0) {
    uint8_t v = 0xFF;
    SetLineBits(&v, 0, 8, false);
    ASSERT_EQ(v, 0u);
}

TEST(BitTest, SetLineBits_TwoBytes0to16) {
    uint16_t v = 0;
    SetLineBits(&v, 0, 16, true);
    ASSERT_EQ(v, 0xFFFFu);
}

TEST(BitTest, SetLineBits_PartialBytes4to12) {
    uint8_t mem[2] = {};
    SetLineBits(mem, 4, 12, true);
    ASSERT_EQ(mem[0], 0x0Fu);  // bits 4-7 in BE MSB-first → mask 0xFF>>4
    ASSERT_EQ(mem[1], 0xF0u);  // bits 8-11 in BE MSB-first → mask 0xFF<<4
}

TEST(BitTest, SetLineBits_SingleBit0to1) {
    uint8_t v = 0;
    SetLineBits(&v, 0, 1, true);
    ASSERT_EQ(v, 0x80u);  // bit 0 = MSB
}

TEST(BitTest, SetLineBits_BitOnByteBoundary7to8) {
    uint8_t v = 0;
    SetLineBits(&v, 7, 8, true);
    ASSERT_EQ(v, 0x01u);  // bit 7 = LSB
}

TEST(BitTest, SetLineBits_FirstBitOfSecondByte8to9) {
    uint8_t mem[2] = {};
    SetLineBits(mem, 8, 9, true);
    ASSERT_EQ(mem[0], 0x00u);
    ASSERT_EQ(mem[1], 0x80u);  // bit 8 = MSB of byte 1
}

TEST(BitTest, SetLineBits_HalfByte0to4) {
    uint8_t v = 0;
    SetLineBits(&v, 0, 4, true);
    ASSERT_EQ(v, 0xF0u);  // upper nibble (MSB-first bits 0-3)
}

TEST(BitTest, SetLineBits_SecondHalf4to8) {
    uint8_t v = 0;
    SetLineBits(&v, 4, 8, true);
    ASSERT_EQ(v, 0x0Fu);  // lower nibble (MSB-first bits 4-7)
}

TEST(BitTest, SetLineBits_FullUint320to32) {
    uint32_t v = 0;
    SetLineBits(&v, 0, 32, true);
    ASSERT_EQ(v, 0xFFFFFFFFu);
}

TEST(BitTest, SetLineBits_MiddleOfUint6416to48) {
    uint64_t v = 0;
    SetLineBits(&v, 16, 48, true);
    ASSERT_EQ(v, 0x0000FFFFFFFF0000ULL);
}

TEST(BitTest, SetLineBits_FullUint640to64) {
    uint64_t v = 0;
    SetLineBits(&v, 0, 64, true);
    ASSERT_EQ(v, 0xFFFFFFFFFFFFFFFFULL);
}

TEST(BitTest, SetLineBits_SingleBitInMiddle3to4) {
    uint8_t v = 0;
    SetLineBits(&v, 3, 4, true);
    ASSERT_EQ(v, 0x10u);  // bit 3 from MSB → mask 0x10
}

TEST(BitTest, SetLineBits_MiddleOfByte1to7) {
    uint8_t v = 0;
    SetLineBits(&v, 1, 7, true);
    ASSERT_EQ(v, 0x7Eu);
}

TEST(BitTest, SetLineBits_ThreeBytes0to24) {
    uint8_t mem[4] = {};
    SetLineBits(mem, 0, 24, true);
    ASSERT_EQ(mem[0], 0xFFu);  // bytes 0,1,2 = MSB side in BE
    ASSERT_EQ(mem[1], 0xFFu);
    ASSERT_EQ(mem[2], 0xFFu);
    ASSERT_EQ(mem[3], 0x00u);
}

TEST(BitTest, SetLineBits_OtherBitsUnchanged) {
    uint8_t mem[2] = {0x00, 0xFF};  // explicit BE layout: byte 0 = 0x00, byte 1 = 0xFF
    SetLineBits(mem, 0, 4, false);  // bits 0-3 are in byte 0 which is already 0x00
    ASSERT_EQ(mem[0], 0x00u);
    ASSERT_EQ(mem[1], 0xFFu);
}

TEST(BitTest, SetLineBits_VerifyWithGetBit) {
    uint8_t mem[2] = {};
    SetLineBits(mem, 2, 6, true);
    for (size_t i = 2; i < 6; ++i)
        ASSERT_TRUE(GetBit(mem, i)) << "bit " << i;
    ASSERT_FALSE(GetBit(mem, 1));
    ASSERT_FALSE(GetBit(mem, 6));
}

TEST(BitTest, SetLineBits_FullSecondByte8to16) {
    uint8_t mem[2] = {};
    SetLineBits(mem, 8, 16, true);
    ASSERT_EQ(mem[0], 0x00u);
    ASSERT_EQ(mem[1], 0xFFu);  // byte 1 fully set
}

TEST(BitTest, SetLineBits_TwoEdgeChunks) {
    uint8_t v = 0;
    SetLineBits(&v, 0, 2, true);
    SetLineBits(&v, 6, 8, true);
    ASSERT_EQ(v, 0xC3u);  // bits 0,1,6,7
}

TEST(BitTest, SetLineBits_OverlapWithExistingBits) {
    uint8_t v = 0xFF;
    SetLineBits(&v, 2, 6, false);
    ASSERT_EQ(v, 0xC3u);  // only bits 0,1,6,7 remain
}

#endif  // __BYTE_ORDER__
