#ifndef endian_h
#define endian_h

/*uint16_t constexpr HostIsLittleEndianTestData(0xABCD);
constexpr bool HostIsLittleEndian(void)
{
	return reinterpret_cast<char *>(&HostIsLittleEndianTestData)[0] == '\xCD';
}*/

#ifdef WINDOWS
constexpr bool HostIsLittleEndian(void) { return true; }
#else
#include <endian.h>
constexpr bool HostIsLittleEndian(void) { return __BYTE_ORDER == __LITTLE_ENDIAN; }
#endif


template <typename Base, bool LittleEndian> class Endian
{
	public:
		static_assert(sizeof(Base) <= 8, "Default Endian only works for integers of size up to 64-bit.");
		static_assert((sizeof(Base) & (sizeof(Base) - 1)) == 0, "Default Endian works only for power of 2 size datatypes.");

		constexpr static Base Swap(Base const &Input)
		{
			return HostIsLittleEndian() == LittleEndian ? Input :
				sizeof(Base) == 8 ? Input :
#ifdef WINDOWS
				sizeof(Base) == 16 ? static_cast<Base>(_byteswap_ushort(static_cast<uint16_t>(Input))) :
				sizeof(Base) == 32 ? static_cast<Base>(_byteswap_ulong(static_cast<uint32_t>(Input))) :
				static_cast<Base>(_byteswap_ulonglong(static_cast<uint64_t>(Input)))
#else
				sizeof(Base) == 16 ? static_cast<Base>((static_cast<uint16_t>(Input) << 8) | (static_cast<uint16_t>(Input) >> 8)) :
				sizeof(Base) == 32 ? static_cast<Base>(::__builtin_bswap32(static_cast<uint32_t>(Input))) :
				static_cast<Base>(::__builtin_bswap64(static_cast<uint64_t>(Input)))

#endif
				;
		}

		constexpr Endian(void) {}
		constexpr Endian(Base const &Data) : Data(Swap(Data)) { }
		constexpr Endian(Endian<Base, LittleEndian> const &Other) : Data(Other.Data) {}
		Endian<Base, LittleEndian> &operator =(Base const &Data) { this->Data = Swap(Data); return *this; }
		Endian<Base, LittleEndian> &operator =(Endian<Base, LittleEndian> const &Other) { this->Data = Other.Data; return *this; }
		operator Base(void) const { return Swap(Data); }

	private:
		Base Data;
};
static_assert(sizeof(Endian<uint8_t, true>) == sizeof(uint8_t), "Endian<uint8_t> size doesn't match underlying data size.");
static_assert(sizeof(Endian<uint16_t, true>) == sizeof(uint16_t), "Endian<uint16_t> size doesn't match underlying data size.");
static_assert(sizeof(Endian<uint32_t, true>) == sizeof(uint32_t), "Endian<uint32_t> size doesn't match underlying data size.");
static_assert(sizeof(Endian<uint64_t, true>) == sizeof(uint64_t), "Endian<uint64_t> size doesn't match underlying data size.");
static_assert(sizeof(Endian<float, true>) == sizeof(float), "Endian<float> size doesn't match underlying data size.");
static_assert(sizeof(Endian<double, true>) == sizeof(double), "Endian<double> size doesn't match underlying data size.");
template <typename Base> using LittleEndian = Endian<Base, true>;
template <typename Base> using BigEndian = Endian<Base, false>;

template <bool LittleEndian> class Endian<Color, LittleEndian>
{
	public:
		constexpr static Color Swap(Color const &Input)
		{
			return Color(Endian<float, LittleEndian>::Swap(Input.Red),
				Endian<float, LittleEndian>::Swap(Input.Green),
				Endian<float, LittleEndian>::Swap(Input.Blue),
				Endian<float, LittleEndian>::Swap(Input.Alpha));
		}
	
		constexpr Endian(void) {}	
		constexpr Endian(Color const &Data) : Data(Swap(Data)) { }
		constexpr Endian(Endian<Color, LittleEndian> const &Other) : Data(Other.Data) {}
		Endian<Color, LittleEndian> &operator =(Color const &Data) { this->Data = Swap(Data); return *this; }
		Endian<Color, LittleEndian> &operator =(Endian<Color, true> const &Other) { this->Data = Other.Data; return *this; }
		operator Color(void) const { return Swap(Data); }
	private:
		Color Data;
};

#endif

