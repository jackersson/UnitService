#ifndef Heartbeat_Included
#define Heartbeat_Included

#include <threadable.hpp>
#include <contracts/services/icoordinator_api.hpp>
#include <contracts/common/logger.hpp>
#include <contracts/data/data_utils.hpp>



namespace grpc_services
{
	namespace features
	{
		typedef std::function<void()> RepeatedCallback ;
		typedef std::function<bool()> ConditionCallback;

		struct HeartbeatContext
		{
			std::shared_ptr<contracts::common::Logger> logger;
			//contracts::services::IHeartbeat* service;
			//DataTypes::HeartbeatMessage      message;

			RepeatedCallback  action;
			ConditionCallback can_perform;
		};

		class Heartbeat : public ::utils::Threadable
		{
		public:
			explicit Heartbeat( const HeartbeatContext& context)
			                	: context_(context)//, service_(context.service)
				                , logger_(context.logger)
			{}

			virtual ~Heartbeat() {
				Threadable::stop();
			}

			void run() override
			{
				while (active())
				{
					std::this_thread::sleep_for(HEARTBEAT_DELAY);

					if (context_.can_perform())
						context_.action(); //on_timer();

					if (cancelation_requested)
					{
						logger_->info("Hearbeat cancel requested");
						break;
					}
				}
			}

			void on_timer() const
			{
				//service_->heart_beat(context_.message);
			}

			/*
			//TODO to utils
			static void fill_message( const std::string& guid
				                      , DataTypes::ServiceType stype
				                      , DataTypes::HeartbeatMessage& msg)
			{				
				auto key = new DataTypes::Key();
				contracts::data::set_guid(guid, *key);
				msg.set_allocated_id(key);
				msg.set_service_type(stype);
			}
			*/

		private:
			HeartbeatContext context_;
			//contracts::services::IHeartbeat* service_;
			std::shared_ptr<contracts::common::Logger> logger_;
			//DataTypes::HeartbeatMessage message_;
			std::chrono::seconds HEARTBEAT_DELAY = std::chrono::seconds(3);

		};
	}
}
#endif