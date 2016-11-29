#ifndef DirectShowDeviceUnit_Included
#define DirectShowDeviceUnit_Included

#include <locations/ilocation.hpp>
#include <observers/observable.hpp>
#include "idevice_unit.hpp"
#include <devices/video_device/ivideo_engine.hpp>
#include <data/irepository.hpp>
#include <logging/logger.hpp>

namespace data_model{
	class VisitRecord;
	class CaptureDevice;
	class Person;
}

namespace tbb {
	class task_group;
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
			  public contracts::observers::Observable<locations::ILocation>
			, public devices::IDeviceObserver<video_device::IStreamDataPtr>
			, public IIdentification<video_device::IRawImage>
			, public video_device::ICaptureDeviceUpdatable
			, public contracts::common::ILifecycle
		{

		public:
			virtual ~DirectShowDeviceUnit();

			explicit
				DirectShowDeviceUnit(video_device::IVideoEngine* engine);

			DirectShowDeviceUnit(video_device::IVideoEngine* engine
				, contracts::data::AbstractRepositoryContainer* repository);

			void update(const data_model::CaptureDevice& device) override;
					
			void start() override;

			void stop() override;

			bool verify(data_model::VisitRecord& target
				, const video_device::IRawImage& data) override;

			std::shared_ptr<data_model::VisitRecord>
				identify(const video_device::IRawImage& data) override;

			void on_error(const devices::DeviceException& exception) override;

			void on_state(const devices::IDeviceState& state) override;

			void on_next(video_device::IStreamDataPtr data) override;

		private:
			std::unique_ptr<tbb::task_group> tasks_;

			DirectShowDeviceUnit(const DirectShowDeviceUnit& other) = delete;
			DirectShowDeviceUnit& operator=(const DirectShowDeviceUnit&) = delete;

			void on_target_detected(data_model::VisitRecord& visit_record);
			
			std::unique_ptr<data_model::CaptureDevice> device_;
			//IFaceRecognitionEngine*  face_engine_;
		  video_device::IVideoEngine*  engine_;
			contracts::data::IRepository<data_model::Person>* persons_repository_;

			contracts::logging::Logger logger_;
		};
	}
}

#endif
