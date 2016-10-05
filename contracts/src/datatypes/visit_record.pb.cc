// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: datatypes/visit_record.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "datatypes/visit_record.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace DataTypes {

namespace {

const ::google::protobuf::Descriptor* VisitRecord_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  VisitRecord_reflection_ = NULL;
const ::google::protobuf::Descriptor* VisitRecords_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  VisitRecords_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* AccessState_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_datatypes_2fvisit_5frecord_2eproto() {
  protobuf_AddDesc_datatypes_2fvisit_5frecord_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "datatypes/visit_record.proto");
  GOOGLE_CHECK(file != NULL);
  VisitRecord_descriptor_ = file->message_type(0);
  static const int VisitRecord_offsets_[6] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecord, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecord, person_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecord, location_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecord, time_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecord, card_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecord, state_),
  };
  VisitRecord_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      VisitRecord_descriptor_,
      VisitRecord::default_instance_,
      VisitRecord_offsets_,
      -1,
      -1,
      -1,
      sizeof(VisitRecord),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecord, _internal_metadata_),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecord, _is_default_instance_));
  VisitRecords_descriptor_ = file->message_type(1);
  static const int VisitRecords_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecords, items_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecords, count_),
  };
  VisitRecords_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      VisitRecords_descriptor_,
      VisitRecords::default_instance_,
      VisitRecords_offsets_,
      -1,
      -1,
      -1,
      sizeof(VisitRecords),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecords, _internal_metadata_),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VisitRecords, _is_default_instance_));
  AccessState_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_datatypes_2fvisit_5frecord_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      VisitRecord_descriptor_, &VisitRecord::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      VisitRecords_descriptor_, &VisitRecords::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_datatypes_2fvisit_5frecord_2eproto() {
  delete VisitRecord::default_instance_;
  delete VisitRecord_reflection_;
  delete VisitRecords::default_instance_;
  delete VisitRecords_reflection_;
}

void protobuf_AddDesc_datatypes_2fvisit_5frecord_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::DataTypes::protobuf_AddDesc_datatypes_2fcard_2eproto();
  ::DataTypes::protobuf_AddDesc_datatypes_2fdata_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\034datatypes/visit_record.proto\022\tDataType"
    "s\032\024datatypes/card.proto\032\024datatypes/data."
    "proto\"\305\001\n\013VisitRecord\022\032\n\002id\030\001 \001(\0132\016.Data"
    "Types.Key\022!\n\tperson_id\030\002 \001(\0132\016.DataTypes"
    ".Key\022#\n\013location_id\030\003 \001(\0132\016.DataTypes.Ke"
    "y\022\014\n\004time\030\004 \001(\003\022\035\n\004card\030\005 \001(\0132\017.DataType"
    "s.Card\022%\n\005state\030\006 \001(\0162\026.DataTypes.Access"
    "State\"D\n\014VisitRecords\022%\n\005items\030\001 \003(\0132\026.D"
    "ataTypes.VisitRecord\022\r\n\005count\030\002 \001(\003*5\n\013A"
    "ccessState\022\r\n\tNoneState\020\000\022\013\n\007Granted\020\001\022\n"
    "\n\006Denied\020\002BP\n\007ex.grpcZ\?github.com/Enebra"
    "/ServiceCoordinator/grpc/datatypes/visit"
    "record\242\002\003RTGb\006proto3", 500);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "datatypes/visit_record.proto", &protobuf_RegisterTypes);
  VisitRecord::default_instance_ = new VisitRecord();
  VisitRecords::default_instance_ = new VisitRecords();
  VisitRecord::default_instance_->InitAsDefaultInstance();
  VisitRecords::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_datatypes_2fvisit_5frecord_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_datatypes_2fvisit_5frecord_2eproto {
  StaticDescriptorInitializer_datatypes_2fvisit_5frecord_2eproto() {
    protobuf_AddDesc_datatypes_2fvisit_5frecord_2eproto();
  }
} static_descriptor_initializer_datatypes_2fvisit_5frecord_2eproto_;
const ::google::protobuf::EnumDescriptor* AccessState_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return AccessState_descriptor_;
}
bool AccessState_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}


namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD;
static void MergeFromFail(int line) {
  GOOGLE_CHECK(false) << __FILE__ << ":" << line;
}

}  // namespace


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int VisitRecord::kIdFieldNumber;
const int VisitRecord::kPersonIdFieldNumber;
const int VisitRecord::kLocationIdFieldNumber;
const int VisitRecord::kTimeFieldNumber;
const int VisitRecord::kCardFieldNumber;
const int VisitRecord::kStateFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

VisitRecord::VisitRecord()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:DataTypes.VisitRecord)
}

void VisitRecord::InitAsDefaultInstance() {
  _is_default_instance_ = true;
  id_ = const_cast< ::DataTypes::Key*>(&::DataTypes::Key::default_instance());
  person_id_ = const_cast< ::DataTypes::Key*>(&::DataTypes::Key::default_instance());
  location_id_ = const_cast< ::DataTypes::Key*>(&::DataTypes::Key::default_instance());
  card_ = const_cast< ::DataTypes::Card*>(&::DataTypes::Card::default_instance());
}

VisitRecord::VisitRecord(const VisitRecord& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:DataTypes.VisitRecord)
}

void VisitRecord::SharedCtor() {
    _is_default_instance_ = false;
  _cached_size_ = 0;
  id_ = NULL;
  person_id_ = NULL;
  location_id_ = NULL;
  time_ = GOOGLE_LONGLONG(0);
  card_ = NULL;
  state_ = 0;
}

VisitRecord::~VisitRecord() {
  // @@protoc_insertion_point(destructor:DataTypes.VisitRecord)
  SharedDtor();
}

void VisitRecord::SharedDtor() {
  if (this != default_instance_) {
    delete id_;
    delete person_id_;
    delete location_id_;
    delete card_;
  }
}

void VisitRecord::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* VisitRecord::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return VisitRecord_descriptor_;
}

const VisitRecord& VisitRecord::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_datatypes_2fvisit_5frecord_2eproto();
  return *default_instance_;
}

VisitRecord* VisitRecord::default_instance_ = NULL;

VisitRecord* VisitRecord::New(::google::protobuf::Arena* arena) const {
  VisitRecord* n = new VisitRecord;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void VisitRecord::Clear() {
// @@protoc_insertion_point(message_clear_start:DataTypes.VisitRecord)
  if (GetArenaNoVirtual() == NULL && id_ != NULL) delete id_;
  id_ = NULL;
  if (GetArenaNoVirtual() == NULL && person_id_ != NULL) delete person_id_;
  person_id_ = NULL;
  if (GetArenaNoVirtual() == NULL && location_id_ != NULL) delete location_id_;
  location_id_ = NULL;
  time_ = GOOGLE_LONGLONG(0);
  if (GetArenaNoVirtual() == NULL && card_ != NULL) delete card_;
  card_ = NULL;
  state_ = 0;
}

bool VisitRecord::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:DataTypes.VisitRecord)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .DataTypes.Key id = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_id()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_person_id;
        break;
      }

      // optional .DataTypes.Key person_id = 2;
      case 2: {
        if (tag == 18) {
         parse_person_id:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_person_id()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_location_id;
        break;
      }

      // optional .DataTypes.Key location_id = 3;
      case 3: {
        if (tag == 26) {
         parse_location_id:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_location_id()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(32)) goto parse_time;
        break;
      }

      // optional int64 time = 4;
      case 4: {
        if (tag == 32) {
         parse_time:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &time_)));

        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(42)) goto parse_card;
        break;
      }

      // optional .DataTypes.Card card = 5;
      case 5: {
        if (tag == 42) {
         parse_card:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_card()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(48)) goto parse_state;
        break;
      }

      // optional .DataTypes.AccessState state = 6;
      case 6: {
        if (tag == 48) {
         parse_state:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          set_state(static_cast< ::DataTypes::AccessState >(value));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:DataTypes.VisitRecord)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:DataTypes.VisitRecord)
  return false;
#undef DO_
}

void VisitRecord::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:DataTypes.VisitRecord)
  // optional .DataTypes.Key id = 1;
  if (this->has_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, *this->id_, output);
  }

  // optional .DataTypes.Key person_id = 2;
  if (this->has_person_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, *this->person_id_, output);
  }

  // optional .DataTypes.Key location_id = 3;
  if (this->has_location_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      3, *this->location_id_, output);
  }

  // optional int64 time = 4;
  if (this->time() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(4, this->time(), output);
  }

  // optional .DataTypes.Card card = 5;
  if (this->has_card()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      5, *this->card_, output);
  }

  // optional .DataTypes.AccessState state = 6;
  if (this->state() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      6, this->state(), output);
  }

  // @@protoc_insertion_point(serialize_end:DataTypes.VisitRecord)
}

::google::protobuf::uint8* VisitRecord::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:DataTypes.VisitRecord)
  // optional .DataTypes.Key id = 1;
  if (this->has_id()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, *this->id_, target);
  }

  // optional .DataTypes.Key person_id = 2;
  if (this->has_person_id()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, *this->person_id_, target);
  }

  // optional .DataTypes.Key location_id = 3;
  if (this->has_location_id()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        3, *this->location_id_, target);
  }

  // optional int64 time = 4;
  if (this->time() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(4, this->time(), target);
  }

  // optional .DataTypes.Card card = 5;
  if (this->has_card()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        5, *this->card_, target);
  }

  // optional .DataTypes.AccessState state = 6;
  if (this->state() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      6, this->state(), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:DataTypes.VisitRecord)
  return target;
}

int VisitRecord::ByteSize() const {
// @@protoc_insertion_point(message_byte_size_start:DataTypes.VisitRecord)
  int total_size = 0;

  // optional .DataTypes.Key id = 1;
  if (this->has_id()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        *this->id_);
  }

  // optional .DataTypes.Key person_id = 2;
  if (this->has_person_id()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        *this->person_id_);
  }

  // optional .DataTypes.Key location_id = 3;
  if (this->has_location_id()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        *this->location_id_);
  }

  // optional int64 time = 4;
  if (this->time() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int64Size(
        this->time());
  }

  // optional .DataTypes.Card card = 5;
  if (this->has_card()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        *this->card_);
  }

  // optional .DataTypes.AccessState state = 6;
  if (this->state() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->state());
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void VisitRecord::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:DataTypes.VisitRecord)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const VisitRecord* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const VisitRecord>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:DataTypes.VisitRecord)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:DataTypes.VisitRecord)
    MergeFrom(*source);
  }
}

void VisitRecord::MergeFrom(const VisitRecord& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:DataTypes.VisitRecord)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  if (from.has_id()) {
    mutable_id()->::DataTypes::Key::MergeFrom(from.id());
  }
  if (from.has_person_id()) {
    mutable_person_id()->::DataTypes::Key::MergeFrom(from.person_id());
  }
  if (from.has_location_id()) {
    mutable_location_id()->::DataTypes::Key::MergeFrom(from.location_id());
  }
  if (from.time() != 0) {
    set_time(from.time());
  }
  if (from.has_card()) {
    mutable_card()->::DataTypes::Card::MergeFrom(from.card());
  }
  if (from.state() != 0) {
    set_state(from.state());
  }
}

void VisitRecord::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:DataTypes.VisitRecord)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void VisitRecord::CopyFrom(const VisitRecord& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:DataTypes.VisitRecord)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool VisitRecord::IsInitialized() const {

  return true;
}

void VisitRecord::Swap(VisitRecord* other) {
  if (other == this) return;
  InternalSwap(other);
}
void VisitRecord::InternalSwap(VisitRecord* other) {
  std::swap(id_, other->id_);
  std::swap(person_id_, other->person_id_);
  std::swap(location_id_, other->location_id_);
  std::swap(time_, other->time_);
  std::swap(card_, other->card_);
  std::swap(state_, other->state_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata VisitRecord::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = VisitRecord_descriptor_;
  metadata.reflection = VisitRecord_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// VisitRecord

// optional .DataTypes.Key id = 1;
bool VisitRecord::has_id() const {
  return !_is_default_instance_ && id_ != NULL;
}
void VisitRecord::clear_id() {
  if (GetArenaNoVirtual() == NULL && id_ != NULL) delete id_;
  id_ = NULL;
}
const ::DataTypes::Key& VisitRecord::id() const {
  // @@protoc_insertion_point(field_get:DataTypes.VisitRecord.id)
  return id_ != NULL ? *id_ : *default_instance_->id_;
}
::DataTypes::Key* VisitRecord::mutable_id() {
  
  if (id_ == NULL) {
    id_ = new ::DataTypes::Key;
  }
  // @@protoc_insertion_point(field_mutable:DataTypes.VisitRecord.id)
  return id_;
}
::DataTypes::Key* VisitRecord::release_id() {
  // @@protoc_insertion_point(field_release:DataTypes.VisitRecord.id)
  
  ::DataTypes::Key* temp = id_;
  id_ = NULL;
  return temp;
}
void VisitRecord::set_allocated_id(::DataTypes::Key* id) {
  delete id_;
  id_ = id;
  if (id) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:DataTypes.VisitRecord.id)
}

// optional .DataTypes.Key person_id = 2;
bool VisitRecord::has_person_id() const {
  return !_is_default_instance_ && person_id_ != NULL;
}
void VisitRecord::clear_person_id() {
  if (GetArenaNoVirtual() == NULL && person_id_ != NULL) delete person_id_;
  person_id_ = NULL;
}
const ::DataTypes::Key& VisitRecord::person_id() const {
  // @@protoc_insertion_point(field_get:DataTypes.VisitRecord.person_id)
  return person_id_ != NULL ? *person_id_ : *default_instance_->person_id_;
}
::DataTypes::Key* VisitRecord::mutable_person_id() {
  
  if (person_id_ == NULL) {
    person_id_ = new ::DataTypes::Key;
  }
  // @@protoc_insertion_point(field_mutable:DataTypes.VisitRecord.person_id)
  return person_id_;
}
::DataTypes::Key* VisitRecord::release_person_id() {
  // @@protoc_insertion_point(field_release:DataTypes.VisitRecord.person_id)
  
  ::DataTypes::Key* temp = person_id_;
  person_id_ = NULL;
  return temp;
}
void VisitRecord::set_allocated_person_id(::DataTypes::Key* person_id) {
  delete person_id_;
  person_id_ = person_id;
  if (person_id) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:DataTypes.VisitRecord.person_id)
}

// optional .DataTypes.Key location_id = 3;
bool VisitRecord::has_location_id() const {
  return !_is_default_instance_ && location_id_ != NULL;
}
void VisitRecord::clear_location_id() {
  if (GetArenaNoVirtual() == NULL && location_id_ != NULL) delete location_id_;
  location_id_ = NULL;
}
const ::DataTypes::Key& VisitRecord::location_id() const {
  // @@protoc_insertion_point(field_get:DataTypes.VisitRecord.location_id)
  return location_id_ != NULL ? *location_id_ : *default_instance_->location_id_;
}
::DataTypes::Key* VisitRecord::mutable_location_id() {
  
  if (location_id_ == NULL) {
    location_id_ = new ::DataTypes::Key;
  }
  // @@protoc_insertion_point(field_mutable:DataTypes.VisitRecord.location_id)
  return location_id_;
}
::DataTypes::Key* VisitRecord::release_location_id() {
  // @@protoc_insertion_point(field_release:DataTypes.VisitRecord.location_id)
  
  ::DataTypes::Key* temp = location_id_;
  location_id_ = NULL;
  return temp;
}
void VisitRecord::set_allocated_location_id(::DataTypes::Key* location_id) {
  delete location_id_;
  location_id_ = location_id;
  if (location_id) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:DataTypes.VisitRecord.location_id)
}

// optional int64 time = 4;
void VisitRecord::clear_time() {
  time_ = GOOGLE_LONGLONG(0);
}
 ::google::protobuf::int64 VisitRecord::time() const {
  // @@protoc_insertion_point(field_get:DataTypes.VisitRecord.time)
  return time_;
}
 void VisitRecord::set_time(::google::protobuf::int64 value) {
  
  time_ = value;
  // @@protoc_insertion_point(field_set:DataTypes.VisitRecord.time)
}

// optional .DataTypes.Card card = 5;
bool VisitRecord::has_card() const {
  return !_is_default_instance_ && card_ != NULL;
}
void VisitRecord::clear_card() {
  if (GetArenaNoVirtual() == NULL && card_ != NULL) delete card_;
  card_ = NULL;
}
const ::DataTypes::Card& VisitRecord::card() const {
  // @@protoc_insertion_point(field_get:DataTypes.VisitRecord.card)
  return card_ != NULL ? *card_ : *default_instance_->card_;
}
::DataTypes::Card* VisitRecord::mutable_card() {
  
  if (card_ == NULL) {
    card_ = new ::DataTypes::Card;
  }
  // @@protoc_insertion_point(field_mutable:DataTypes.VisitRecord.card)
  return card_;
}
::DataTypes::Card* VisitRecord::release_card() {
  // @@protoc_insertion_point(field_release:DataTypes.VisitRecord.card)
  
  ::DataTypes::Card* temp = card_;
  card_ = NULL;
  return temp;
}
void VisitRecord::set_allocated_card(::DataTypes::Card* card) {
  delete card_;
  card_ = card;
  if (card) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:DataTypes.VisitRecord.card)
}

// optional .DataTypes.AccessState state = 6;
void VisitRecord::clear_state() {
  state_ = 0;
}
 ::DataTypes::AccessState VisitRecord::state() const {
  // @@protoc_insertion_point(field_get:DataTypes.VisitRecord.state)
  return static_cast< ::DataTypes::AccessState >(state_);
}
 void VisitRecord::set_state(::DataTypes::AccessState value) {
  
  state_ = value;
  // @@protoc_insertion_point(field_set:DataTypes.VisitRecord.state)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int VisitRecords::kItemsFieldNumber;
const int VisitRecords::kCountFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

VisitRecords::VisitRecords()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:DataTypes.VisitRecords)
}

void VisitRecords::InitAsDefaultInstance() {
  _is_default_instance_ = true;
}

VisitRecords::VisitRecords(const VisitRecords& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:DataTypes.VisitRecords)
}

void VisitRecords::SharedCtor() {
    _is_default_instance_ = false;
  _cached_size_ = 0;
  count_ = GOOGLE_LONGLONG(0);
}

VisitRecords::~VisitRecords() {
  // @@protoc_insertion_point(destructor:DataTypes.VisitRecords)
  SharedDtor();
}

void VisitRecords::SharedDtor() {
  if (this != default_instance_) {
  }
}

void VisitRecords::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* VisitRecords::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return VisitRecords_descriptor_;
}

const VisitRecords& VisitRecords::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_datatypes_2fvisit_5frecord_2eproto();
  return *default_instance_;
}

VisitRecords* VisitRecords::default_instance_ = NULL;

VisitRecords* VisitRecords::New(::google::protobuf::Arena* arena) const {
  VisitRecords* n = new VisitRecords;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void VisitRecords::Clear() {
// @@protoc_insertion_point(message_clear_start:DataTypes.VisitRecords)
  count_ = GOOGLE_LONGLONG(0);
  items_.Clear();
}

bool VisitRecords::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:DataTypes.VisitRecords)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .DataTypes.VisitRecord items = 1;
      case 1: {
        if (tag == 10) {
          DO_(input->IncrementRecursionDepth());
         parse_loop_items:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtualNoRecursionDepth(
                input, add_items()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(10)) goto parse_loop_items;
        input->UnsafeDecrementRecursionDepth();
        if (input->ExpectTag(16)) goto parse_count;
        break;
      }

      // optional int64 count = 2;
      case 2: {
        if (tag == 16) {
         parse_count:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &count_)));

        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:DataTypes.VisitRecords)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:DataTypes.VisitRecords)
  return false;
#undef DO_
}

void VisitRecords::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:DataTypes.VisitRecords)
  // repeated .DataTypes.VisitRecord items = 1;
  for (unsigned int i = 0, n = this->items_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->items(i), output);
  }

  // optional int64 count = 2;
  if (this->count() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(2, this->count(), output);
  }

  // @@protoc_insertion_point(serialize_end:DataTypes.VisitRecords)
}

::google::protobuf::uint8* VisitRecords::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:DataTypes.VisitRecords)
  // repeated .DataTypes.VisitRecord items = 1;
  for (unsigned int i = 0, n = this->items_size(); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->items(i), target);
  }

  // optional int64 count = 2;
  if (this->count() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(2, this->count(), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:DataTypes.VisitRecords)
  return target;
}

int VisitRecords::ByteSize() const {
// @@protoc_insertion_point(message_byte_size_start:DataTypes.VisitRecords)
  int total_size = 0;

  // optional int64 count = 2;
  if (this->count() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int64Size(
        this->count());
  }

  // repeated .DataTypes.VisitRecord items = 1;
  total_size += 1 * this->items_size();
  for (int i = 0; i < this->items_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->items(i));
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void VisitRecords::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:DataTypes.VisitRecords)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const VisitRecords* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const VisitRecords>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:DataTypes.VisitRecords)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:DataTypes.VisitRecords)
    MergeFrom(*source);
  }
}

void VisitRecords::MergeFrom(const VisitRecords& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:DataTypes.VisitRecords)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  items_.MergeFrom(from.items_);
  if (from.count() != 0) {
    set_count(from.count());
  }
}

void VisitRecords::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:DataTypes.VisitRecords)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void VisitRecords::CopyFrom(const VisitRecords& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:DataTypes.VisitRecords)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool VisitRecords::IsInitialized() const {

  return true;
}

void VisitRecords::Swap(VisitRecords* other) {
  if (other == this) return;
  InternalSwap(other);
}
void VisitRecords::InternalSwap(VisitRecords* other) {
  items_.UnsafeArenaSwap(&other->items_);
  std::swap(count_, other->count_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata VisitRecords::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = VisitRecords_descriptor_;
  metadata.reflection = VisitRecords_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// VisitRecords

// repeated .DataTypes.VisitRecord items = 1;
int VisitRecords::items_size() const {
  return items_.size();
}
void VisitRecords::clear_items() {
  items_.Clear();
}
const ::DataTypes::VisitRecord& VisitRecords::items(int index) const {
  // @@protoc_insertion_point(field_get:DataTypes.VisitRecords.items)
  return items_.Get(index);
}
::DataTypes::VisitRecord* VisitRecords::mutable_items(int index) {
  // @@protoc_insertion_point(field_mutable:DataTypes.VisitRecords.items)
  return items_.Mutable(index);
}
::DataTypes::VisitRecord* VisitRecords::add_items() {
  // @@protoc_insertion_point(field_add:DataTypes.VisitRecords.items)
  return items_.Add();
}
::google::protobuf::RepeatedPtrField< ::DataTypes::VisitRecord >*
VisitRecords::mutable_items() {
  // @@protoc_insertion_point(field_mutable_list:DataTypes.VisitRecords.items)
  return &items_;
}
const ::google::protobuf::RepeatedPtrField< ::DataTypes::VisitRecord >&
VisitRecords::items() const {
  // @@protoc_insertion_point(field_list:DataTypes.VisitRecords.items)
  return items_;
}

// optional int64 count = 2;
void VisitRecords::clear_count() {
  count_ = GOOGLE_LONGLONG(0);
}
 ::google::protobuf::int64 VisitRecords::count() const {
  // @@protoc_insertion_point(field_get:DataTypes.VisitRecords.count)
  return count_;
}
 void VisitRecords::set_count(::google::protobuf::int64 value) {
  
  count_ = value;
  // @@protoc_insertion_point(field_set:DataTypes.VisitRecords.count)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace DataTypes

// @@protoc_insertion_point(global_scope)
