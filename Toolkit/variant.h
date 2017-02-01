#ifndef VARIANT_H
#define VARIANT_H

#include <cassert>
#include <cstdint>

class Variant
{
public:
  Variant(void) { data.ptr = nullptr; }
  template<typename T> Variant(T val) { operator =(val); }

  template<typename T>
  Variant& operator = (T* val) { data.ptr = val; return *this; }

  Variant& operator = (uint64_t val) { data.u64 = val; return *this; }
  Variant& operator = (uint32_t val) { data.u32 = val; return *this; }
  Variant& operator = (uint16_t val) { data.u16 = val; return *this; }
  Variant& operator = (uint8_t  val) { data.u8  = val; return *this; }
  Variant& operator = (int64_t  val) { data.i64 = val; return *this; }
  Variant& operator = (int32_t  val) { data.i32 = val; return *this; }
  Variant& operator = (int16_t  val) { data.i16 = val; return *this; }
  Variant& operator = (int8_t   val) { data.i8  = val; return *this; }

  template<typename T> T& value(void);

  template<typename T> operator T*(void) { return static_cast<T*>(data.ptr); }

  operator uint64_t&(void) { return data.u64; }
  operator uint32_t&(void) { return data.u32; }
  operator uint16_t&(void) { return data.u16; }
  operator uint8_t& (void) { return data.u8 ; }
  operator int64_t& (void) { return data.i64; }
  operator int32_t& (void) { return data.i32; }
  operator int16_t& (void) { return data.i16; }
  operator int8_t&  (void) { return data.i8 ; }


  operator uint64_t(void) const { return data.u64; }
  operator uint32_t(void) const { return data.u32; }
  operator uint16_t(void) const { return data.u16; }
  operator uint8_t (void) const { return data.u8 ; }
  operator int64_t (void) const { return data.i64; }
  operator int32_t (void) const { return data.i32; }
  operator int16_t (void) const { return data.i16; }
  operator int8_t  (void) const { return data.i8 ; }

private:
  union data_t
  {
    void* ptr;
    uint64_t u64;
    uint32_t u32;
    uint16_t u16;
    uint8_t  u8;
    int64_t  i64;
    int32_t  i32;
    int16_t  i16;
    int8_t   i8;
  } data;
};

#endif // VARIANT_H
