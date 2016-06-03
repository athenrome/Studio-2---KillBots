#ifndef KF_FILE_HEADER
#define KF_FILE_HEADER
#include "vector"
#include "iostream"
#include "string"
#include "kf/kf_types.h"

namespace kf
{
	class KF_API Path
	{
	public:
		Path();
		~Path();
		Path(const std::string &s);
		Path &operator/(const Path &p);
		Path &operator/(const std::string &s);
		Path normalise() const;
		std::string str() const;
		Path parent() const;
	protected:
		bool m_absolute;
		std::vector<std::string> m_path;
		friend std::ostream &operator<<(std::ostream &os, const Path &p);
		

	};

	std::ostream &operator<<(std::ostream &os, const Path &p);

}

#endif