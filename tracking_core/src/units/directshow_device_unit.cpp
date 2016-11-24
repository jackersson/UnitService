#include "units/directshow_device_unit.hpp"

#include <data/models/face_characteristics.hpp>
#include <data/models/visit_record.hpp>
#include <data/models/person.hpp>
#include <data/models/devices.hpp>

#include <opencv/cv.hpp>

#include <tbb/task_group.h>

using namespace data_model;
using namespace contracts::devices;

namespace tracking
{
	namespace units
	{	
		inline
			cv::Mat bytes_to_mat(unsigned char* bytes, int width, int height)
		{
			auto image = cv::Mat(height, width, CV_8UC3, bytes);
			return image;
		}

		DirectShowDeviceUnit::~DirectShowDeviceUnit() {
			DirectShowDeviceUnit::stop();
		}
		
		DirectShowDeviceUnit::DirectShowDeviceUnit(video_device::IVideoEngine* engine)
			: device_(std::make_unique<CaptureDevice>())
			, engine_(engine)
			, persons_repository_(nullptr)
			, tasks_(std::make_unique<tbb::task_group>())
		{
			if (engine_ == nullptr)
				throw std::exception("Access device engine can't be null");
		}

		DirectShowDeviceUnit::DirectShowDeviceUnit(video_device::IVideoEngine* engine
			, contracts::data::AbstractRepositoryContainer* repository)
			: engine_(engine)
			, device_(std::make_unique<CaptureDevice>())
			, tasks_(std::make_unique<tbb::task_group>())
		{
			if (engine_ == nullptr)
				throw std::exception("Access device engine can't be null");

			if (repository == nullptr)
				persons_repository_ = repository->get<Person>();
		}

		void DirectShowDeviceUnit::update(const CaptureDevice& device) 
		{
			if (*device_ == device)
				return;
			stop();
			*device_ = device;
			start();
		}

		void DirectShowDeviceUnit::start() 
		{
			if (device_->is_empty())
			{
				logger_.error("Device name is not valid");
				return;
			}

			engine_->add(*device_);
			engine_->subscribe(this, *device_);
		}

		void DirectShowDeviceUnit::stop() 
		{
			engine_->unsubscribe(this);
			engine_->remove(*device_);
		}

		bool DirectShowDeviceUnit::verify( VisitRecord& target
			                               , const video_device::IRawImage& data) 
		{
			FaceCharacteristics face;// = try_extract_card(data, card);
			target.set_face(face);
			return true;
		}

		std::shared_ptr<VisitRecord>
			DirectShowDeviceUnit::identify(const video_device::IRawImage& data) 
		{
			std::vector<data_model::FaceCharacteristics> faces;
			auto person_found = false;// try_extract_card(data, faces);

			if (!person_found)
				return nullptr;

			auto face = faces[0];
			//TODO maybe need more faces

			auto visit_record = std::make_shared<VisitRecord >();
			visit_record->set_now();
			visit_record->set_face(face);

			if (person_found)
				visit_record->set_person_id(face.person_id());

			return visit_record;
		}

		void DirectShowDeviceUnit::on_error(const DeviceException& exception) 
		{
			logger_.error("Capture device exception {0}", exception.what());
			for (auto observer : observers_)
				observer->on_error(exception);
		}

		void DirectShowDeviceUnit::on_state(const IDeviceState& state) 
		{
			for (auto observer : observers_)
				observer->on_state(state);
		}

		void DirectShowDeviceUnit::on_next(const IStreamData& data)
		{
		//	auto im = data.try_get_data(video_device::StreamTypeColor);
		//	if (im == nullptr)
		//		return;
			
			//tasks_->run([im, this]()
			//{
				//auto frame = bytes_to_mat(im->data(), im->width(), im->height());
			  //cv::namedWindow("view", CV_WINDOW_NORMAL);
				//cv::setWindowProperty("view", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
		  	cv::imshow("view", data.color());
				if (cvWaitKey(10) >= 0)
				{
					return;
				}
			//});
		}			

		void DirectShowDeviceUnit::on_target_detected(data_model::VisitRecord& visit_record)
		{
			for (auto observer : observers_)
				observer->on_target_detected(visit_record);
		}
	}
}

