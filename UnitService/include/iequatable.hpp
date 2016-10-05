#ifndef IEquatable_Included
#define IEquatable_Included

//TODO delete

namespace utils
{
	template <typename T>
	class IEquatable
	{
	public:
		virtual ~IEquatable()	{}

		virtual bool equals(const T& other) const = 0;
	};
}

#endif
