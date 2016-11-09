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

			virtual bool grant_access(const data_model::Location& location) = 0;
			virtual void
				update_with(const std::vector<data_model::Location>& locations) = 0;

			virtual void
				update(const data_model::Location& location) = 0;

			virtual bool contains(const data_model::Location& location) const = 0;

			virtual bool contains( const std::string& device_name
				                  , data_model::DeviceType dev_type) const = 0;

			virtual size_t size() const = 0;		
		};

		typedef std::shared_ptr<ITrackLocationsEngine> ITrackLocationsEnginePtr;
	}
}

#endif
