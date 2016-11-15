#ifndef DirectShowDeviceUnit_Included
#define DirectShowDeviceUnit_Included

#include <contracts/locations/ilocation.hpp>
#include <observers/observable.hpp>
#include "idevice_unit.hpp"
#include <contracts/devices/video_device/ivideo_engine.hpp>
#include <data/irepository.hpp>
#include <logging/logger.hpp>

namespace data_model{
	class VisitRecord;
	class CaptureDevice;
	class Person;
}

namespace tracking
{
	namespace units
	{		
		/*
		class IFaceRecognitionEngine
		{
		public:
			Services::BiometricResponse* verify( DataTypes::Key*        person
				                                 , DataTypes::FrameBytes* frame  )  {		  	
				return nullptr;
		  }

			Services::BiometricResponse* identify( DataTypes::FrameBytes* photo )  {		  	
				return nullptr;
		  }

			Services::BiometricResponse* enroll ( DataTypes::FrameBytes* photo )
			{
				return nullptr;
			}

		};
		*/

		class DirectShowDeviceUnit :
			  public contracts::observers::Observable<contracts::locations::ILocation>
			, public contracts::devices::IDeviceObserver<IStreamData>
			, public IIdentification<contracts::devices::video_device::IRawImage>
			, public contracts::devices::access_device::ICaptureDeviceUpdatable
			, public contracts::common::ILifecycle
		{

		public:
			virtual ~DirectShowDeviceUnit();

			explicit
				DirectShowDeviceUnit(contracts::devices::video_device::IVideoEngine* engine);

			DirectShowDeviceUnit(contracts::devices::video_device::IVideoEngine* engine
				, contracts::data::AbstractRepositoryContainer* repository);

			void update(const data_model::CaptureDevice& device) override;
					
			void start() override;

			void stop() override;

			bool verify(data_model::VisitRecord& target
				, const contracts::devices::video_device::IRawImage& data) override;

			std::shared_ptr<data_model::VisitRecord>
				identify(const contracts::devices::video_device::IRawImage& data) override;

			void on_error(const contracts::devices::DeviceException& exception) override;

			void on_state(const contracts::devices::IDeviceState& state) override;

			void on_next(const IStreamData& data) override;

		private:
			DirectShowDeviceUnit(const DirectShowDeviceUnit& other) = delete;
			DirectShowDeviceUnit& operator=(const DirectShowDeviceUnit&) = delete;

			void on_target_detected(data_model::VisitRecord& visit_record);
			
			std::unique_ptr<data_model::CaptureDevice> device_;
			//IFaceRecognitionEngine*  face_engine_;
			contracts::devices::video_device::IVideoEngine*  engine_;
			contracts::data::IRepository<data_model::Person>* persons_repository_;

			contracts::logging::Logger logger_;
		};
	}
}

#endif
