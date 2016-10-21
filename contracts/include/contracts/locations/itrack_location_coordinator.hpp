#ifndef ITrackLocationsEngine_Included
#define ITrackLocationsEngine_Included


namespace contracts
{
	namespace locations
	{
		class ITrackLocationsEngine
		{
		public:
			virtual ~ITrackLocationsEngine() {}

			virtual bool grant_access(const DataTypes::Location& location) = 0;
			virtual void
				update_with(const std::vector<DataTypes::Location>& locations) = 0;

			virtual void
				update(const DataTypes::Location& location) = 0;
		};

		typedef std::shared_ptr<ITrackLocationsEngine> ITrackLocationsEnginePtr;
	}
}

#endif
