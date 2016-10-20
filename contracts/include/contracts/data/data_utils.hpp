#ifndef DataUtils_Included
#define DataUtils_Included

#include <datatypes/key.pb.h>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <thread/lock_algorithms.hpp>

namespace contracts
{
	namespace data
	{
		const int UUID_BYTES_SIZE = 16;

		inline DataTypes::DateTime* get_current_time()
		{
			auto now = boost::posix_time::second_clock::local_time();

			auto current_time = new DataTypes::DateTime();
			current_time->set_year (now.date().year() );
			current_time->set_month(now.date().month());
			current_time->set_day  (now.date().day()  );

			current_time->set_hours  (now.time_of_day().hours  ());
			current_time->set_minutes(now.time_of_day().minutes());
			current_time->set_seconds(now.time_of_day().seconds());
			return current_time;
		}


		inline bool try_parse_guid(const std::string& value
			, boost::uuids::uuid& uid)
		{
			try
			{
				uid = boost::lexical_cast<boost::uuids::uuid>(value);
				return true;
			}
			catch (std::exception&) {
				return false;
			}
		}

		//TODO check if guid not empty

		inline void set_guid(const boost::uuids::uuid& guid, DataTypes::Key& key)
		{
			unsigned char data[UUID_BYTES_SIZE];
			memcpy(&data, guid.data, UUID_BYTES_SIZE);

			auto str = new std::string(data, data + sizeof(data) / sizeof(data[0]));
			key.set_allocated_guid(str);
		}

		inline std::string to_bytestring(const std::string& guid)
		{
			boost::uuids::uuid uuid;
			if (!try_parse_guid(guid, uuid))
				throw std::exception("Cannot parse guid from string");

			unsigned char data[UUID_BYTES_SIZE];
			memcpy(&data, uuid.data, UUID_BYTES_SIZE);

			return std::string(data, data + sizeof(data) / sizeof(data[0]));
		}

		inline void set_guid(const std::string& guid, DataTypes::Key& key)
		{
			boost::uuids::uuid uuid;
			if (!try_parse_guid(guid, uuid))
				throw std::exception("Cannot parse guid from string");

			set_guid(uuid, key);
		}

		inline bool get_guid(const DataTypes::Key& key, boost::uuids::uuid& guid)
		{
			if (key.id_type_case() != DataTypes::Key::IdTypeCase::kGuid)
				return false;

			auto key_guid = key.guid();
			memcpy(&guid, key_guid.data(), UUID_BYTES_SIZE);
			return true;
		}

		inline std::string to_string(const DataTypes::Key& key)
		{
			if (key.id_type_case() != DataTypes::Key::IdTypeCase::kGuid)
				return "";

			boost::uuids::uuid guid;
			auto result = get_guid(key, guid);
			if (!result)
				return "";
			return to_string(guid);
		}

		inline std::string uuid_to_string(const boost::uuids::uuid& uid)
		{
			return to_string(uid);
		}

		inline DataTypes::Key get_random_key()
		{
			DataTypes::Key key;
			auto uuid = boost::uuids::random_generator()();
			set_guid(uuid, key);
			return key;
		}

		inline std::string get_random_guid()
		{
			auto uuid = boost::uuids::random_generator()();
			return to_string(uuid);
		}



		inline bool keys_equal(const DataTypes::Key& first
			, const DataTypes::Key& second)
		{
			if (first.id_type_case() != second.id_type_case())
				return false;

			switch (first.id_type_case())
			{
			case DataTypes::Key::kNumber:
			case DataTypes::Key::kIdentifier:
			case DataTypes::Key::ID_TYPE_NOT_SET:
				return false;

			case DataTypes::Key::kGuid:
				return to_string(first) == to_string(second);

			default:
				return false;
			}
		}

	}
}

#endif
