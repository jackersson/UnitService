#ifndef DirectShowDeviceUnit_Included
#define DirectShowDeviceUnit_Included

#include <contracts/locations/ilocation.hpp>
#include <observers/observable.hpp>
#include <contracts/devices/access_device/iaccess_coordinator.hpp>
#include <contracts/devices/access_device/iaccess_device_engine.hpp>
#include "idevice_unit.hpp"
#include <contracts/devices/video_device/ivideo_engine.hpp>


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

		struct DirectShowDeviceUnitContext
		{
			contracts::devices::video_device::IVideoEngine* engine    ;
			contracts::data::AbstractRepositoryContainer*   repository;
		};

		class DirectShowDeviceUnit :
			  public contracts::observers::Observable<contracts::locations::ILocation>
			, public contracts::devices::IDeviceObserver<IStreamData>
			, public IIdentification<contracts::devices::video_device::IRawImage>
			, public contracts::devices::access_device::ICaptureDeviceUpdatable
			, public contracts::common::ILifecycle
		{

		public:
			virtual ~DirectShowDeviceUnit() {
				DirectShowDeviceUnit::stop();
			}

			explicit DirectShowDeviceUnit(const DirectShowDeviceUnitContext& context)			
			{
				engine_             = context.engine;
				persons_repository_ = context.repository->get<data_model::Person>();
			}			

			void update(const data_model::CaptureDevice& device) override
			{
				if (device_ == device)
					return;
				stop();
				device_ = device;
				start();
			}
					
			void start() override
			{
				auto dev_name = device_.name;
				if (dev_name == "")
				{
					logger_.error("Device name is not valid");
					return;
				}

				engine_->add(dev_name);
				engine_->subscribe(this, dev_name);
			}

			void stop() override
			{
				engine_->unsubscribe(this);
				engine_->remove(device_.name);
			}
					

			bool verify( data_model::VisitRecord& target
			           , const contracts::devices::video_device::IRawImage& data) override
			{							
				data_model::FaceCharacteristics face;// = try_extract_card(data, card);
				target.set_face(face);				
				return true;
			}

			std::shared_ptr<data_model::VisitRecord> 
				identify(const contracts::devices::video_device::IRawImage& data) override
			{
				std::vector<data_model::FaceCharacteristics> faces;
				auto person_found = false;// try_extract_card(data, faces);
				
				if (!person_found)
					return nullptr;

				auto face = faces[0];
				//TODO maybe need more faces

				auto visit_record = std::make_shared<data_model::VisitRecord >();
				visit_record->set_now();
				visit_record->set_face(face);

				if (person_found)
					visit_record->set_person_id(face.person_id());			
		
				return visit_record;
			}

			void on_error(const contracts::devices::DeviceException& exception) override
			{
				logger_.error("Capture device exception {0}", exception.what());
				for (auto observer : observers_)
					observer->on_error(exception);
			}

			void on_state(const contracts::devices::IDeviceState& state) override
			{
				for (auto observer : observers_)
					observer->on_state(state);
			}

			void on_next(const IStreamData& data) override
			{
				auto im = data.try_get_data(contracts::devices::video_device::StreamTypeColor);
				if (im == nullptr)
					return;
				//TODO async process
			}

		private:
			DirectShowDeviceUnit(const DirectShowDeviceUnit& other) = delete;
			DirectShowDeviceUnit& operator=(const DirectShowDeviceUnit&) = delete;

			bool device_connected() const {
				return engine_->device_enumerator().connected(device_.name);
			}
		
			void on_target_detected(data_model::VisitRecord& visit_record)
			{
				for (auto observer : observers_)
					observer->on_target_detected(visit_record);
			}		

			data_model::CaptureDevice device_    ;
			//IFaceRecognitionEngine*  face_engine_;
			contracts::devices::video_device::IVideoEngine*  engine_;
			contracts::data::IRepository<data_model::Person>* persons_repository_;

			contracts::logging::Logger logger_;
		};
	}
}

#endif
