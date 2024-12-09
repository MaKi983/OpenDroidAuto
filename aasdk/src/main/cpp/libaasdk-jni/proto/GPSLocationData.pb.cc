// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: GPSLocationData.proto

#include "GPSLocationData.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace aasdk {
namespace proto {
namespace data {
constexpr GPSLocation::GPSLocation(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : timestamp_(uint64_t{0u})
  , latitude_(0)
  , longitude_(0)
  , accuracy_(0u)
  , altitude_(0)
  , speed_(0)
  , bearing_(0){}
struct GPSLocationDefaultTypeInternal {
  constexpr GPSLocationDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~GPSLocationDefaultTypeInternal() {}
  union {
    GPSLocation _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT GPSLocationDefaultTypeInternal _GPSLocation_default_instance_;
}  // namespace data
}  // namespace proto
}  // namespace aasdk
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_GPSLocationData_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_GPSLocationData_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_GPSLocationData_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_GPSLocationData_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::GPSLocation, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::GPSLocation, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::GPSLocation, timestamp_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::GPSLocation, latitude_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::GPSLocation, longitude_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::GPSLocation, accuracy_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::GPSLocation, altitude_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::GPSLocation, speed_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::GPSLocation, bearing_),
  0,
  1,
  2,
  3,
  4,
  5,
  6,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 12, sizeof(::aasdk::proto::data::GPSLocation)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::aasdk::proto::data::_GPSLocation_default_instance_),
};

const char descriptor_table_protodef_GPSLocationData_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\025GPSLocationData.proto\022\020aasdk.proto.dat"
  "a\"\211\001\n\013GPSLocation\022\021\n\ttimestamp\030\001 \002(\004\022\020\n\010"
  "latitude\030\002 \002(\005\022\021\n\tlongitude\030\003 \002(\005\022\020\n\010acc"
  "uracy\030\004 \002(\r\022\020\n\010altitude\030\005 \001(\005\022\r\n\005speed\030\006"
  " \001(\005\022\017\n\007bearing\030\007 \001(\005"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_GPSLocationData_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_GPSLocationData_2eproto = {
  false, false, 181, descriptor_table_protodef_GPSLocationData_2eproto, "GPSLocationData.proto", 
  &descriptor_table_GPSLocationData_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_GPSLocationData_2eproto::offsets,
  file_level_metadata_GPSLocationData_2eproto, file_level_enum_descriptors_GPSLocationData_2eproto, file_level_service_descriptors_GPSLocationData_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_GPSLocationData_2eproto_getter() {
  return &descriptor_table_GPSLocationData_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_GPSLocationData_2eproto(&descriptor_table_GPSLocationData_2eproto);
namespace aasdk {
namespace proto {
namespace data {

// ===================================================================

class GPSLocation::_Internal {
 public:
  using HasBits = decltype(std::declval<GPSLocation>()._has_bits_);
  static void set_has_timestamp(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_latitude(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_longitude(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_accuracy(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
  static void set_has_altitude(HasBits* has_bits) {
    (*has_bits)[0] |= 16u;
  }
  static void set_has_speed(HasBits* has_bits) {
    (*has_bits)[0] |= 32u;
  }
  static void set_has_bearing(HasBits* has_bits) {
    (*has_bits)[0] |= 64u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x0000000f) ^ 0x0000000f) != 0;
  }
};

GPSLocation::GPSLocation(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:aasdk.proto.data.GPSLocation)
}
GPSLocation::GPSLocation(const GPSLocation& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&timestamp_, &from.timestamp_,
    static_cast<size_t>(reinterpret_cast<char*>(&bearing_) -
    reinterpret_cast<char*>(&timestamp_)) + sizeof(bearing_));
  // @@protoc_insertion_point(copy_constructor:aasdk.proto.data.GPSLocation)
}

inline void GPSLocation::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&timestamp_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&bearing_) -
    reinterpret_cast<char*>(&timestamp_)) + sizeof(bearing_));
}

GPSLocation::~GPSLocation() {
  // @@protoc_insertion_point(destructor:aasdk.proto.data.GPSLocation)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void GPSLocation::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void GPSLocation::ArenaDtor(void* object) {
  GPSLocation* _this = reinterpret_cast< GPSLocation* >(object);
  (void)_this;
}
void GPSLocation::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void GPSLocation::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void GPSLocation::Clear() {
// @@protoc_insertion_point(message_clear_start:aasdk.proto.data.GPSLocation)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x0000007fu) {
    ::memset(&timestamp_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&bearing_) -
        reinterpret_cast<char*>(&timestamp_)) + sizeof(bearing_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* GPSLocation::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // required uint64 timestamp = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_timestamp(&has_bits);
          timestamp_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required int32 latitude = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_latitude(&has_bits);
          latitude_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required int32 longitude = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          _Internal::set_has_longitude(&has_bits);
          longitude_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required uint32 accuracy = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          _Internal::set_has_accuracy(&has_bits);
          accuracy_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional int32 altitude = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 40)) {
          _Internal::set_has_altitude(&has_bits);
          altitude_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional int32 speed = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 48)) {
          _Internal::set_has_speed(&has_bits);
          speed_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional int32 bearing = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 56)) {
          _Internal::set_has_bearing(&has_bits);
          bearing_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag == 0) || ((tag & 7) == 4)) {
          CHK_(ptr);
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* GPSLocation::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:aasdk.proto.data.GPSLocation)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required uint64 timestamp = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(1, this->_internal_timestamp(), target);
  }

  // required int32 latitude = 2;
  if (cached_has_bits & 0x00000002u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_latitude(), target);
  }

  // required int32 longitude = 3;
  if (cached_has_bits & 0x00000004u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(3, this->_internal_longitude(), target);
  }

  // required uint32 accuracy = 4;
  if (cached_has_bits & 0x00000008u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(4, this->_internal_accuracy(), target);
  }

  // optional int32 altitude = 5;
  if (cached_has_bits & 0x00000010u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(5, this->_internal_altitude(), target);
  }

  // optional int32 speed = 6;
  if (cached_has_bits & 0x00000020u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(6, this->_internal_speed(), target);
  }

  // optional int32 bearing = 7;
  if (cached_has_bits & 0x00000040u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(7, this->_internal_bearing(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:aasdk.proto.data.GPSLocation)
  return target;
}

size_t GPSLocation::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:aasdk.proto.data.GPSLocation)
  size_t total_size = 0;

  if (_internal_has_timestamp()) {
    // required uint64 timestamp = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_timestamp());
  }

  if (_internal_has_latitude()) {
    // required int32 latitude = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_latitude());
  }

  if (_internal_has_longitude()) {
    // required int32 longitude = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_longitude());
  }

  if (_internal_has_accuracy()) {
    // required uint32 accuracy = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_accuracy());
  }

  return total_size;
}
size_t GPSLocation::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:aasdk.proto.data.GPSLocation)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x0000000f) ^ 0x0000000f) == 0) {  // All required fields are present.
    // required uint64 timestamp = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_timestamp());

    // required int32 latitude = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_latitude());

    // required int32 longitude = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_longitude());

    // required uint32 accuracy = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_accuracy());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000070u) {
    // optional int32 altitude = 5;
    if (cached_has_bits & 0x00000010u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
          this->_internal_altitude());
    }

    // optional int32 speed = 6;
    if (cached_has_bits & 0x00000020u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
          this->_internal_speed());
    }

    // optional int32 bearing = 7;
    if (cached_has_bits & 0x00000040u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
          this->_internal_bearing());
    }

  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData GPSLocation::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    GPSLocation::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GPSLocation::GetClassData() const { return &_class_data_; }

void GPSLocation::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to,
                      const ::PROTOBUF_NAMESPACE_ID::Message&from) {
  static_cast<GPSLocation *>(to)->MergeFrom(
      static_cast<const GPSLocation &>(from));
}


void GPSLocation::MergeFrom(const GPSLocation& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:aasdk.proto.data.GPSLocation)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x0000007fu) {
    if (cached_has_bits & 0x00000001u) {
      timestamp_ = from.timestamp_;
    }
    if (cached_has_bits & 0x00000002u) {
      latitude_ = from.latitude_;
    }
    if (cached_has_bits & 0x00000004u) {
      longitude_ = from.longitude_;
    }
    if (cached_has_bits & 0x00000008u) {
      accuracy_ = from.accuracy_;
    }
    if (cached_has_bits & 0x00000010u) {
      altitude_ = from.altitude_;
    }
    if (cached_has_bits & 0x00000020u) {
      speed_ = from.speed_;
    }
    if (cached_has_bits & 0x00000040u) {
      bearing_ = from.bearing_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void GPSLocation::CopyFrom(const GPSLocation& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:aasdk.proto.data.GPSLocation)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool GPSLocation::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_has_bits_)) return false;
  return true;
}

void GPSLocation::InternalSwap(GPSLocation* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(GPSLocation, bearing_)
      + sizeof(GPSLocation::bearing_)
      - PROTOBUF_FIELD_OFFSET(GPSLocation, timestamp_)>(
          reinterpret_cast<char*>(&timestamp_),
          reinterpret_cast<char*>(&other->timestamp_));
}

::PROTOBUF_NAMESPACE_ID::Metadata GPSLocation::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_GPSLocationData_2eproto_getter, &descriptor_table_GPSLocationData_2eproto_once,
      file_level_metadata_GPSLocationData_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace data
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::aasdk::proto::data::GPSLocation* Arena::CreateMaybeMessage< ::aasdk::proto::data::GPSLocation >(Arena* arena) {
  return Arena::CreateMessageInternal< ::aasdk::proto::data::GPSLocation >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
