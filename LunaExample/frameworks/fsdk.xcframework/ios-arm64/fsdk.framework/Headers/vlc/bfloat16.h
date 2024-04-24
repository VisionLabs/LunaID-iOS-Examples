#pragma once

#include <cstdint>
#include <cmath>

class bfloat16
{    
    uint16_t value;

    union FloatBits
    {
        float f;
        uint16_t s[2];
        uint32_t i;
    };

    bfloat16(uint16_t x, bool) : value(x)
    {}

public:
    bfloat16() = default;    

    bfloat16(float x)
    {
        *this = x;
    }

    bfloat16& operator = (float x)
    {
        FloatBits b {x};
        switch(std::fpclassify(x))
        {
        case FP_SUBNORMAL:
        case FP_ZERO:
            // sign preserving zero (denormal go to zero)
            value = b.s[1];
            value &= 0x8000;
            break;
        case FP_INFINITE: 
            value = b.s[1];
            break;
        case FP_NAN:
            // truncate and set MSB of the mantissa force QNAN
            value = b.s[1];
            value |= 1 << 6;
            break;
        case FP_NORMAL:
            // round to nearest even and truncate
            unsigned int rounding_bias = 0x00007FFF + (b.s[1] & 0x1);
            b.i += rounding_bias;
            value = b.s[1];
            break;
        }

        return *this;
    }

    operator float() const
    {
        FloatBits b {0};
        b.s[1] = value;
        return b.f;        
    }

    inline bfloat16 operator - () const
    {
        return bfloat16(value ^ 0x8000, true);
    }
};

inline bfloat16 operator + (bfloat16 a, bfloat16 b)
{
    return float(a) + float(b);
}

inline bfloat16 operator - (bfloat16 a, bfloat16 b)
{
    return float(a) + float(-b);
}

inline bfloat16 operator * (bfloat16 a, bfloat16 b)
{
    return float(a) * float(b);
}

inline bfloat16 operator / (bfloat16 a, bfloat16 b)
{
    return float(a) / float(b);
}

inline bfloat16& operator += (bfloat16& a, const bfloat16 b)
{
	a = a + b;
	return a;
}

inline bfloat16& operator -= (bfloat16& a, const bfloat16 b)
{
	a = a - b;
	return a;
}

inline bfloat16& operator *= (bfloat16& a, const bfloat16 b)
{
	a = a * b;
	return a;
}

inline bfloat16& operator /= (bfloat16& a, const bfloat16 b)
{
	a = a / b;
	return a;
}

// Comparison operators
inline bfloat16 operator == (const bfloat16 a, const bfloat16 b)
{
	return float(a) == float(b);
}

inline bfloat16 operator != (const bfloat16 a, const bfloat16 b)
{
	return !(a == b);
}

inline bfloat16 operator < (const bfloat16 a, const bfloat16 b)
{
	return float(a) < float(b);
}

inline bfloat16 operator <= (const bfloat16 a, const bfloat16 b)
{
	return float(a) <= float(b);
}

inline bfloat16 operator > (const bfloat16 a, const bfloat16 b)
{
	return float(a) > float(b);
}

inline bfloat16 operator >= (const bfloat16 a, const bfloat16 b)
{
	return float(a) >= float(b);
}
