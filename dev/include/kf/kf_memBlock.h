#ifndef KF_MEMORY_BLOCK_HEADER
#define KF_MEMORY_BLOCK_HEADER

#include "kf/kf_types.h"

#include <ostream>

// to do: add search to memBlock

namespace kf
{
	class KF_API MemBlock
	{
	public:
		MemBlock();
		MemBlock(u8 *start, u64 length);
		~MemBlock();
		void setRange(u8 *start, u64 length);
		bool seek(s64 offset, bool relative = false);
		MemBlock &operator+=(s64 offset);
		MemBlock &operator-=(s64 offset);
		MemBlock &operator++();
		MemBlock &operator--();
		MemBlock operator()(s64 offset, bool relative = true);
		bool operator==(const MemBlock &rhs);
		MemBlock &operator=(const MemBlock &rhs);
		u8 *begin() const;
		u8 *end() const;
		u8 *current() const;
		u64 size() const;
		u64 getU64(bool move = true);
		s64 getS64(bool move = true);
		u32 getU32(bool move = true);
		u32 getU24(bool move = true);
		s32 getS24(bool move = true);
		s32 getS32(bool move = true);
		u16 getU16(bool move = true);
		s16 getS16(bool move = true);
		u8  getU8(bool move = true);
		s8  getS8(bool move = true);
		float  getFloat(bool move = true);
		double getDouble(bool move = true);
		void getBlock(u8 *destination, u64 length, bool move = true);
		std::string getString(u64 length=0, bool move = true);
		std::string getHex(u64 length, u64 division = 0, bool move = true);
		void setU32(u32 value, bool move = true);
		void setS32(s32 value, bool move = true);
		void setU16(u16 value, bool move = true);
		void setS16(s16 value, bool move = true);
		void setU8(u8 value, bool move = true);
		void setS8(s8 value, bool move = true);
		void setFloat(float value, bool move = true);
		void setDouble(double value, bool move = true);
		void setBlock(u8 *source, u64 length, bool move = true);
		void setString(const std::string &value, bool include_terminator = true, u64 length = 0, bool pad = false, bool move = true);
		
		/*
		MemBlock &operator<<(dword value);
		MemBlock &operator<<(sdword value);
		MemBlock &operator<<(word value);
		MemBlock &operator<<(sword value);
		MemBlock &operator<<(byte value);
		MemBlock &operator<<(sbyte value);
		MemBlock &operator<<(float value);
		MemBlock &operator<<(double value);
		MemBlock &operator<<(const std::string &value);
		*/

	protected:
		u8 *m_begin;
		u8 *m_end;
		u8 *m_current;
	};

	KF_API std::ostream &operator<<(std::ostream &os, const MemBlock &src);

	template <int S>
	class MemBlockWrap: public MemBlock
	{
	public:
		kf::u8 m_stackData[S];
		MemBlockWrap()
		{
			m_begin = m_stackData;
			m_end = m_begin + S;
			m_current = m_begin;
		}
	};
}

#endif
