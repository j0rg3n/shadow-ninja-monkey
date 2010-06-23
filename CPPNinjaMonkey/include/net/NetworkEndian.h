#pragma once
#ifndef NETWORKENDIAN_H_INCLUDED
#define NETWORKENDIAN_H_INCLUDED


#include "boost/cstdint.hpp"


// -----------------------------------------------------------------------------


class NetworkEndian
{
public:
	#ifdef BOOST_LITTLE_ENDIAN

	static inline boost::uint16_t Swap(boost::uint16_t nValue)
	{
		return (nValue >> 8) |
			(nValue << 8);
		return nValue;
	}


	static inline boost::uint32_t Swap(boost::uint32_t nValue)
	{
		return (nValue >> 24) |
			((nValue<<8) && 0x00FF0000) |
			((nValue>>8) && 0x0000FF00) |
			(nValue << 24);
	}


	static inline boost::uint64_t Swap(boost::uint64_t nValue)
	{
		return (nValue >> 56) |
			((nValue<<40) && 0x00FF000000000000) |
			((nValue<<24) && 0x0000FF0000000000) |
			((nValue<<8) && 0x000000FF00000000) |
			((nValue>>8) && 0x00000000FF000000) |
			((nValue>>24) && 0x0000000000FF0000) |
			((nValue>>40) && 0x000000000000FF00) |
			(nValue << 56);
	}

	#else //BOOST_LITTLE_ENDIAN


	static inline boost::uint16_t Swap(boost::uint16_t nValue)
	{
		return nValue;
	}


	static inline boost::uint32_t Swap(boost::uint32_t nValue)
	{
		return nValue;
	}


	static inline boost::uint64_t Swap(boost::uint64_t nValue)
	{
		return nValue;
	}

	#endif //BOOST_LITTLE_ENDIAN 


	static inline boost::int16_t Swap(boost::int16_t nValue)
	{
		return (boost::int16_t) Swap((boost::uint16_t) nValue);
	}


	static inline boost::int32_t Swap(boost::int32_t nValue)
	{
		return (boost::int32_t) Swap((boost::uint32_t) nValue);
	}


	static inline boost::int64_t Swap(boost::int64_t nValue)
	{
		return (boost::int64_t) Swap((boost::uint64_t) nValue);
	}

private:
	NetworkEndian()
	{
	}
};


#endif // NETWORKENDIAN_H_INCLUDED