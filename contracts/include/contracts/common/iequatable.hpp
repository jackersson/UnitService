#ifndef IEquatable_Included
#define IEquatable_Included


namespace contracts
{
	namespace common
	{
		template <typename T>
		class IEquatable
		{
		public:
			virtual ~IEquatable() {}

			virtual bool equals(const T& other) const = 0;
		};

		class IObject
		{
		public:
			virtual ~IObject() {}

			virtual size_t get_hash_code() const = 0;
		};
	}
}

#endif
