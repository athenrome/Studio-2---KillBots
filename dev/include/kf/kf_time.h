#ifndef KF_TIME_HEADER
#define KF_TIME_HEADER

#include "kf/kf_types.h"

namespace kf
{
	__forceinline u64 getCpuTime(void)
	{
		__asm rdtsc
	}
}
#endif
