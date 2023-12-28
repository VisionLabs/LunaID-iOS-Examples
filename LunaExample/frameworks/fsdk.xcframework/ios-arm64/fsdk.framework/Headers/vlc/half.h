#pragma once

#include <cstdint>
#include <limits>

uint16_t float2half(float val);
float half2float(uint16_t val);

class half
{
public:
	half() = default;

	half(float val) : _x(float2half(val))
	{
	}

	operator float() const
	{
		return half2float(_x);
	}

	inline half operator - () const
	{
		half result;
		result._x = _x ^ 0x8000;
		return result;
	}

	static half construct(const uint16_t half_data)
	{
		half result;
		result._x = half_data;
		return result;
	}

private:
	uint16_t _x;

	friend bool isinf(const half a);
	friend bool isnan(const half a);
	friend half abs(const half a);
};

// Arithmetic
inline half operator + (const half a, const half b)
{
	return half(float(a) + float(b));
}

inline half operator - (const half a, const half b)
{
	return half(float(a) - float(b));
}

inline half operator * (const half a, const half b)
{
	return half(float(a) * float(b));
}

inline half operator / (const half a, const half b)
{
	return half(float(a) / float(b));
}

inline half& operator += (half& a, const half b)
{
	a = a + b;
	return a;
}

inline half& operator -= (half& a, const half b)
{
	a = a - b;
	return a;
}

inline half& operator *= (half& a, const half b)
{
	a = a * b;
	return a;
}

inline half& operator /= (half& a, const half b)
{
	a = a / b;
	return a;
}

// Comparison operators
inline half operator == (const half a, const half b)
{
	return float(a) == float(b);
}

inline half operator != (const half a, const half b)
{
	return !(a == b);
}

inline half operator < (const half a, const half b)
{
	return float(a) < float(b);
}

inline half operator <= (const half a, const half b)
{
	return float(a) <= float(b);
}

inline half operator > (const half a, const half b)
{
	return float(a) > float(b);
}

inline half operator >= (const half a, const half b)
{
	return float(a) >= float(b);
}

inline bool isinf(const half a)
{
	return (a._x & 0x7FFF) == 0x7C00;
}

inline bool isnan(const half a)
{
	return (a._x & 0x7fff) > 0x7c00;
}

inline bool isfinite(const half a)
{
	return !isinf(a) && !isnan(a);
}

inline half abs(const half a)
{
	half result;
	result._x = a._x & 0x7FFF;
	return result;
}

namespace detail
{
	union uif
	{
		uint32_t u;
		float f;
	};
}

#ifdef __F16C__
#include <immintrin.h>
#endif

inline uint16_t float2half(float val)
{
#ifdef __F16C__
	return _cvtss_sh(val, 0);
#else
	detail::uif f;
	f.f = val;

	const detail::uif f32infty = { 255 << 23 };
	const detail::uif f16max = { (127 + 16) << 23 };
	const detail::uif denorm_magic = { ((127 - 15) + (23 - 10) + 1) << 23 };
	unsigned int sign_mask = 0x80000000u;
	uint16_t o;
	o = static_cast<uint16_t>(0x0u);

	uint32_t sign = f.u & sign_mask;
	f.u ^= sign;

	// NOTE all the integer compares in this function can be safely
	// compiled into signed compares since all operands are below
	// 0x80000000. Important if you want fast straight SSE2 code
	// (since there's no unsigned PCMPGTD).

	if (f.u >= f16max.u) {  // result is Inf or NaN (all exponent bits set)
		o = (f.u > f32infty.u) ? 0x7e00 : 0x7c00; // NaN->qNaN and Inf->Inf
	}
	else {  // (De)normalized number or zero
		if (f.u < (113 << 23)) {  // resulting FP16 is subnormal or zero
								  // use a magic value to align our 10 mantissa bits at the bottom of
								  // the float. as long as FP addition is round-to-nearest-even this
								  // just works.
			f.f += denorm_magic.f;

			// and one integer subtract of the bias later, we have our final float!
			o = static_cast<unsigned short>(f.u - denorm_magic.u);
		}
		else {
			unsigned int mant_odd = (f.u >> 13) & 1; // resulting mantissa is odd

													 // update exponent, rounding bias part 1
			f.u += ((unsigned int)(15 - 127) << 23) + 0xfff;
			// rounding bias part 2
			f.u += mant_odd;
			// take the bits!
			o = static_cast<unsigned short>(f.u >> 13);
		}
	}

	o |= static_cast<uint16_t>(sign >> 16);
	return o;
#endif
}

inline float half2float(uint16_t _x)
{
#ifdef __F16C__
	return _cvtsh_ss(_x, 0);
#else
	const detail::uif magic = { 113 << 23 };
	const unsigned int shifted_exp = 0x7c00 << 13; // exponent mask after shift
	detail::uif o;

	o.u = (_x & 0x7fff) << 13;             // exponent/mantissa bits
	unsigned int exp = shifted_exp & o.u;   // just the exponent
	o.u += (127 - 15) << 23;                // exponent adjust

											// handle exponent special cases
	if (exp == shifted_exp) {     // Inf/NaN?
		o.u += (128 - 16) << 23;    // extra exp adjust
	}
	else if (exp == 0) {        // Zero/Denormal?
		o.u += 1 << 23;             // extra exp adjust
		o.f -= magic.f;             // renormalize
	}

	o.u |= (_x & 0x8000) << 16;    // sign bit
	return o.f;
#endif
}
