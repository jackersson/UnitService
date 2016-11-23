#include "directshow_device_info.hpp"

namespace directshow_device
{	
	DirectShowDeviceInfo::DirectShowDeviceInfo() : device_id_(0)
	{}

	DirectShowDeviceInfo::DirectShowDeviceInfo(const std::string& device_name, int device_id)
		: device_name_(device_name)
		, device_id_(device_id)
	{}

	void DirectShowDeviceInfo::set_capabilities(const std::vector<Capability>& caps) {
		capabilities_ = caps;
	}

	int DirectShowDeviceInfo::id() const {
		return device_id_;
	}

	const std::string& DirectShowDeviceInfo::name() const {
		return device_name_;
	}

	const std::vector<Capability>& DirectShowDeviceInfo::capabilities() const {
		return capabilities_;
	}

	Capability DirectShowDeviceInfo::best_capability() const
	{
		Capability best;
		for (auto cp : capabilities_)
		{
			if (best.size() < cp.size())
				best = cp;
		}
		
		return Capability(640, 480);// best;
	}

	bool DirectShowDeviceInfo::operator==(const DirectShowDeviceInfo &other) const {

		return device_name_ == other.device_name_
			  && device_id_   == other.device_id_;
	}
}

