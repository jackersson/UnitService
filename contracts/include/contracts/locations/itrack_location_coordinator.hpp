#ifndef ITrackLocationsEngine_Included
#define ITrackLocationsEngine_Included


namespace contracts
{
	namespace locations
	{
		class ITrackLocationsEngine : public common::IModule
		{
		public:
			virtual ~ITrackLocationsEngine() {}

			virtual bool grant_access(const DataTypes::Location& location) = 0;
			virtual void
				update_with(const std::vector<DataTypes::Location>& locations) = 0;

			virtual void
				update(const DataTypes::Location& location) = 0;

			virtual bool contains(const DataTypes::Location& location) const = 0;

			virtual size_t size() const = 0;
		
		};

		typedef std::shared_ptr<ITrackLocationsEngine> ITrackLocationsEnginePtr;
	}
}

#endif
