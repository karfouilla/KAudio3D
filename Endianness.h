#ifndef ENDIANNESS_H_INCLUDED
#define ENDIANNESS_H_INCLUDED
/**
 *
 * @file Endianness.h
 * @author karfouilla
 * @version 1.0
 * @date 27 avril 2019
 * @brief Fichier contenant la gestion des endianness (H)
 *
 */
////////////////////////////////////////////////////////////////////////////////
//
// This file is part of KAudio3D.
// KAudio3D is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// KAudio3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with KAudio3D.  If not, see <https://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

#include <cstdint>

#ifdef BYTE_ORDER
#  define htobe16convert(x) ((x) = htobe16(x))
#  define htobe32convert(x) ((x) = htobe32(x))
#  define htobe64convert(x) ((x) = htobe64(x))

#  define betoh16convert(x) ((x) = be16toh(x))
#  define betoh32convert(x) ((x) = be32toh(x))
#  define betoh64convert(x) ((x) = be64toh(x))

#  define htole16convert(x) ((x) = htole16(x))
#  define htole32convert(x) ((x) = htole32(x))
#  define htole64convert(x) ((x) = htole64(x))

#  define letoh16convert(x) ((x) = le16toh(x))
#  define letoh32convert(x) ((x) = le32toh(x))
#  define letoh64convert(x) ((x) = le64toh(x))
#else
#ifdef __posix__
#  include <endian.h>

#  define GE_BYTE_ORDER BYTE_ORDER

#  define htobe16convert(x) ((x) = htobe16(x))
#  define htobe32convert(x) ((x) = htobe32(x))
#  define htobe64convert(x) ((x) = htobe64(x))

#  define betoh16convert(x) ((x) = be16toh(x))
#  define betoh32convert(x) ((x) = be32toh(x))
#  define betoh64convert(x) ((x) = be64toh(x))

#  define htole16convert(x) ((x) = htole16(x))
#  define htole32convert(x) ((x) = htole32(x))
#  define htole64convert(x) ((x) = htole64(x))

#  define letoh16convert(x) ((x) = le16toh(x))
#  define letoh32convert(x) ((x) = le32toh(x))
#  define letoh64convert(x) ((x) = le64toh(x))
#else // __posix__

#  define LITTLE_ENDIAN 1234
#  define BIG_ENDIAN 4321

// La structure qui suit et les fonctions Swap ont été inspirée de la SDL
#  if defined(__hppa__) || defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || (defined(__MIPS__) && defined(__MISPEB__)) || defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || defined(__sparc__)

#    define BYTE_ORDER BIG_ENDIAN

#    define htobe16convert(x)
#    define htobe32convert(x)
#    define htobe64convert(x)

#    define betoh16convert(x)
#    define betoh32convert(x)
#    define betoh64convert(x)

#    define htole16convert(x) Swap16(x)
#    define htole32convert(x) Swap32(x)
#    define htole64convert(x) Swap64(x)

#    define letoh16convert(x) Swap16(x)
#    define letoh32convert(x) Swap32(x)
#    define letoh64convert(x) Swap64(x)

#  else // BIG_ENDIAN condition

#    define BYTE_ORDER LITTLE_ENDIAN

#    define htobe16convert(x) Swap16(x)
#    define htobe32convert(x) Swap32(x)
#    define htobe64convert(x) Swap64(x)

#    define betoh16convert(x) Swap16(x)
#    define betoh32convert(x) Swap32(x)
#    define betoh64convert(x) Swap64(x)

#    define htole16convert(x)
#    define htole32convert(x)
#    define htole64convert(x)

#    define letoh16convert(x)
#    define letoh32convert(x)
#    define letoh64convert(x)

#  endif // BIG_ENDIAN condition

#  if defined(__GNUC__) && defined(__i386__) && !(__GNUC__ == 2 && __GNUC_MINOR__ == 95) // Proc tests
static inline void Swap16(std::uint16_t& x) noexcept
{
	__asm__("xchgb %b0,%h0": "=q"(x):"0"(x));
}
#  elif defined(__GNUC__) && defined(__x86_64__) // Proc tests
static inline void Swap16(std::uint16_t& x) noexcept
{
	__asm__("xchgb %b0,%h0": "=Q"(x):"0"(x));
}
#  elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__)) // Proc tests
static inline void Swap16(std::uint16_t& x) noexcept
{
	__asm__("rlwimi %0,%2,8,16,23": "=&r"(x):"0"(x >> 8), "r"(x));
}
#  elif defined(__GNUC__) && (defined(__M68000__) || defined(__M68020__)) && !defined(__mcoldfire__) // Proc tests
static inline void Swap16(std::uint16_t& x) noexcept
{
	__asm__("rorw #8,%0": "=d"(x): "0"(x):"cc");
}
#  else
static inline void Swap16(std::uint16_t& x) noexcept
{
    x = static_cast<std::uint16_t>((x << 8) | (x >> 8));
}
#  endif

#  if defined(__GNUC__) && defined(__i386__) // Proc tests
static inline void Swap32(std::uint32_t& x) noexcept
{
	__asm__("bswap %0": "=r"(x):"0"(x));
}
#  elif defined(__GNUC__) && defined(__x86_64__) // Proc tests
static inline void Swap32(std::uint32_t& x) noexcept
{
	__asm__("bswapl %0": "=r"(x):"0"(x));
}
#  elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__)) // Proc tests
static inline void Swap32(std::uint32_t& x) noexcept
{
    std::uint32_t result;

	__asm__("rlwimi %0,%2,24,16,23": "=&r"(result):"0"(x >> 24), "r"(x));
	__asm__("rlwimi %0,%2,8,8,15": "=&r"(result):"0"(result), "r"(x));
	__asm__("rlwimi %0,%2,24,0,7": "=&r"(result):"0"(result), "r"(x));
    x = result;
}
#  elif defined(__GNUC__) && (defined(__M68000__) || defined(__M68020__)) && !defined(__mcoldfire__) // Proc tests
static inline void Swap32(std::uint32_t& x) noexcept
{
	__asm__("rorw #8,%0\n\tswap %0\n\trorw #8,%0": "=d"(x): "0"(x):"cc");
}
#  else // Proc tests
static inline std::uint32_t Swap32(std::uint32_t x) noexcept
{
    x = static_cast<std::uint32_t>((x << 24) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00) | (x >> 24));
}
#  endif // Proc tests

#  if defined(__GNUC__) && defined(__i386__) // Proc tests
static inline std::uint64_t Swap64(std::uint64_t x) noexcept
{
    union U64T32
    {
        struct
        {
            std::uint32_t a, b;
        } s;
        std::uint64_t u;
    }& v(reinterpret_cast<U64T32&>(x));
	__asm__("bswapl %0 ; bswapl %1 ; xchgl %0,%1": "=r"(v.s.a), "=r"(v.s.b):"0"(v.s.a), "1"(v.s.b));
}
#  elif defined(__GNUC__) && defined(__x86_64__) // Proc tests
static inline void Swap64(std::uint64_t& x) noexcept
{
	__asm__("bswapq %0": "=r"(x):"0"(x));
}
#  else // Proc tests
static inline void Swap64(std::uint64_t& x) noexcept
{
    std::uint32_t hi, lo;

    /* Separate into high and low 32-bit values and swap them */
    lo = static_cast<std::uint32_t>(x & 0xFFFFFFFF);
    x >>= 32;
    hi = static_cast<std::uint32_t>(x & 0xFFFFFFFF);
    x = Swap32(lo);
    x <<= 32;
    x |= Swap32(hi);
}
#  endif // Proc tests
#endif // PLATFORM_LINUX
#endif // BYTE_ORDER

/**
 * @defgroup Spécialisation de chaque type pour les conversion d'endianness
 * @{
 */

static inline void htobe(std::uint16_t& v) noexcept
{
	htobe16convert(v);
}
static inline void betoh(std::uint16_t& v) noexcept
{
	betoh16convert(v);
}

static inline void htobe(std::uint32_t& v) noexcept
{
	htobe32convert(v);
}
static inline void betoh(std::uint32_t& v) noexcept
{
	betoh32convert(v);
}

static inline void htobe(std::uint64_t& v) noexcept
{
	htobe64convert(v);
}
static inline void betoh(std::uint64_t& v) noexcept
{
	betoh64convert(v);
}


static inline void htobe(std::int16_t& v) noexcept
{
	std::uint16_t& uv(reinterpret_cast<std::uint16_t&>(v));
	htobe16convert(uv);
}
static inline void betoh(std::int16_t& v) noexcept
{
	std::uint16_t& uv(reinterpret_cast<std::uint16_t&>(v));
	betoh16convert(uv);
}

static inline void htobe(std::int32_t& v) noexcept
{
	std::uint32_t& uv(reinterpret_cast<std::uint32_t&>(v));
	htobe32convert(uv);
}
static inline void betoh(std::int32_t& v) noexcept
{
	std::uint32_t& uv(reinterpret_cast<std::uint32_t&>(v));
	betoh32convert(uv);
}

static inline void htobe(std::int64_t& v) noexcept
{
	std::uint64_t& uv(reinterpret_cast<std::uint64_t&>(v));
	htobe64convert(uv);
}
static inline void betoh(std::int64_t& v) noexcept
{
	std::uint64_t& uv(reinterpret_cast<std::uint64_t&>(v));
	betoh64convert(uv);
}


static inline void htobe(float& v) noexcept
{
	std::uint32_t& uv(reinterpret_cast<std::uint32_t&>(v));
	htobe32convert(uv);
}
static inline void betoh(float& v) noexcept
{
	std::uint32_t& uv(reinterpret_cast<std::uint32_t&>(v));
	betoh32convert(uv);
}

static inline void htobe(double& v) noexcept
{
	std::uint64_t& uv(reinterpret_cast<std::uint64_t&>(v));
	htobe64convert(uv);
}
static inline void betoh(double& v) noexcept
{
	std::uint64_t& uv(reinterpret_cast<std::uint64_t&>(v));
	betoh64convert(uv);
}



static inline void htole(std::uint16_t& v) noexcept
{
	htole16convert(v);
}
static inline void letoh(std::uint16_t& v) noexcept
{
	letoh16convert(v);
}

static inline void htole(std::uint32_t& v) noexcept
{
	htole32convert(v);
}
static inline void letoh(std::uint32_t& v) noexcept
{
	letoh32convert(v);
}

static inline void htole(std::uint64_t& v) noexcept
{
	htole64convert(v);
}
static inline void letoh(std::uint64_t& v) noexcept
{
	letoh64convert(v);
}


static inline void htole(std::int16_t& v) noexcept
{
	std::uint16_t& uv(reinterpret_cast<std::uint16_t&>(v));
	htole16convert(uv);
}
static inline void letoh(std::int16_t& v) noexcept
{
	std::uint16_t& uv(reinterpret_cast<std::uint16_t&>(v));
	letoh16convert(uv);
}

static inline void htole(std::int32_t& v) noexcept
{
	std::uint32_t& uv(reinterpret_cast<std::uint32_t&>(v));
	htole32convert(uv);
}
static inline void letoh(std::int32_t& v) noexcept
{
	std::uint32_t& uv(reinterpret_cast<std::uint32_t&>(v));
	letoh32convert(uv);
}

static inline void htole(std::int64_t& v) noexcept
{
	std::uint64_t& uv(reinterpret_cast<std::uint64_t&>(v));
	htole64convert(uv);
}
static inline void letoh(std::int64_t& v) noexcept
{
	std::uint64_t& uv(reinterpret_cast<std::uint64_t&>(v));
	letoh64convert(uv);
}


static inline void htole(float& v) noexcept
{
	std::uint32_t& uv(reinterpret_cast<std::uint32_t&>(v));
	htole32convert(uv);
}
static inline void letoh(float& v) noexcept
{
	std::uint32_t& uv(reinterpret_cast<std::uint32_t&>(v));
	letoh32convert(uv);
}

static inline void htole(double& v) noexcept
{
	std::uint64_t& uv(reinterpret_cast<std::uint64_t&>(v));
	htole64convert(uv);
}
static inline void letoh(double& v) noexcept
{
	std::uint64_t& uv(reinterpret_cast<std::uint64_t&>(v));
	letoh64convert(uv);
}

/**
 * @}
 */

#endif // ENDIANNESS_H_INCLUDED
