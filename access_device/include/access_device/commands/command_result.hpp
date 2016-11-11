#ifndef CommandResult_Included
#define CommandResult_Included

#include <sstream>
#include <vector>
#include <contracts/devices/access_device/access_device_types.hpp>
#include <contracts/devices/access_device/icommand_result.hpp>

namespace access_device
{
	namespace commands {
		class CommandResult : public contracts::devices::access_device::ICommandResult
		{
			typedef contracts::devices::access_device::access_device_module dev_module;

		public:		
			CommandResult( dev_module module
				           , const std::exception& exeption
				           , bool ok
				           , bool empty)
				: exception_(exeption)
				, module_(module)
				, ok_(ok), empty_(empty)
			{}

			CommandResult()
				: module_(dev_module::NoneModule)
				, ok_(true), empty_(true)				
			{}

			virtual ~CommandResult() {}

			dev_module module() const override
			{
				return module_;
			}

			const std::vector<unsigned char>& data()  const override
			{
				return data_;
			}

			std::string to_string() const override
			{
				std::stringstream ss;
				for ( auto bt : data_)				
					ss << static_cast<int>(bt);				
				return ss.str();
			}

			const std::exception& exception() const override
			{
				return exception_;
			}

			bool ok() const override
			{
				return std::string(exception_.what()) == "Unknown exception";
			}

			bool empty() const override
			{
				return empty_;
			}

			/*
			static CommandResult not_valid(dev_module dev)
			{
				std::exception ex("Command check sum not valid");
				return CommandResult(ex, dev, false, false);
			}

			static CommandResult not_valid(dev_module dev)
			{
				std::exception ex("Command check sum not valid");
				return CommandResult(ex, dev, false, false);
			}					
			*/
			void set_module(dev_module module) {
				 module_ = module;
			}

			void set_data(const std::vector<unsigned char>& data)	{
				data_ = data;
				empty_ = data.size() <= 0;
			}			

			void set_exception(const std::exception& exception)	{
				exception_ = exception;			
			}

			void clear() {
				std::vector<unsigned char> empty_data;
				set_data(empty_data);
				set_exception(std::exception());
			}
						
			
		private:
			std::exception  exception_;
			dev_module      module_   ;

			bool ok_;
			bool empty_;
			std::vector<unsigned char> data_;
		};
	}
}

#endif
