// test_auth_range.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>

static const std::string CODES_56 = "23456789abcdefghjklmnpqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";

std::string toBase56(uint32_t value)
{
	std::string str;

	do
	{
		str.insert(0, std::string(1, CODES_56[value % 56]));
		value /= 56;
	} while (value > 0);

	return str;
}

uint32_t Base56ToInt(std::string value)
{
	std::reverse(value.begin(), value.end());

	size_t ret = 0;
	uint32_t count = 1;
	for (char& character : value) {
		ret += CODES_56.find(character) * count;
		count *= 56;
	}

	return static_cast<uint32_t>(ret);
}

static const std::string CODES_32 = "23456789ABCDEFGHJKLMNPQRSTUVWXYZ";

std::string toBase32(uint32_t value)
{
	static const uint32_t sizeof_codes = 32;

	std::string str;

	do
	{
		str.insert(0, std::string(1, CODES_32[value % sizeof_codes]));
		value /= sizeof_codes;
	} while (value > 0);

	return str;
}

uint32_t Base32ToInt(std::string value)
{
	static const uint32_t sizeof_codes = 32;

	std::reverse(value.begin(), value.end());

	size_t ret = 0;
	uint32_t count = 1;
	for (char& character : value) {
		ret += CODES_32.find(character) * count;
		count *= sizeof_codes;
	}

	return static_cast<uint32_t>(ret);
}


// 拼凑
template < typename _result_type, typename _value_type >
__inline _result_type make_uint_t(_value_type l, _value_type h)
{
	static_assert(sizeof(_result_type) == 2 * sizeof(_value_type), "must sizeof(_result_type) == 2 * sizeof(_value_type)");

	constexpr auto move_bits = sizeof(_value_type) * 8;

	_result_type r = h;
	r <<= move_bits;
	r |= l;

	return r;
}


__inline uint32_t make_uint32(uint16_t l, uint16_t h)
{
	return make_uint_t<uint32_t>(l, h);
}

// 拆分
template < typename _raw_type, typename _value_type >
__inline void split_uint_t(_raw_type v, _value_type& l, _value_type& h)
{
	static_assert(sizeof(_raw_type) == 2 * sizeof(_value_type), "sizeof(_raw_type) == 2 * sizeof(_value_type)");

	constexpr auto move_bits = sizeof(_value_type) * 8;

	l = static_cast<_value_type>(v);
	h = static_cast<_value_type>(v >> move_bits);
}

__inline void split_uint16(uint16_t v, uint8_t& l, uint8_t& h)
{
	split_uint_t(v, l, h);
}

__inline void split_uint32(uint32_t v, uint16_t& l, uint16_t& h)
{
	split_uint_t(v, l, h);
}


int main()
{
    std::cout << "Hello World!\n";
		{
			uint16_t begin = 176;
			uint16_t end = 177;
			uint32_t val = make_uint32(begin, end);
			std::cout << "val=" << val << std::endl;

			std::cout << "base32(val)=" << toBase32(val) << std::endl;
		}

		{
			uint32_t val = Base32ToInt("ZZZZZZY");
			uint16_t begin = 0;
			uint16_t end = 0;
			split_uint32(val, begin, end);

			std::cout << "val=" << val << ", begin=" << begin << ", end=" << end << std::endl;
		}


}
