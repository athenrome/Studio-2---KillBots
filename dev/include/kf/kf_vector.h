#ifndef KF_VECTOR_HEADER
#define KF_VECTOR_HEADER

#include "kf/kf_vector2.h"

namespace kf
{
	template<typename T2, typename T1>
	T2 convertVector2D(const T1 &v)
	{
	   return T2(v.x,v.y);
	}
}
#endif

