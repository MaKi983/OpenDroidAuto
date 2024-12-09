// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: NavigationDistanceEventMessage.proto

#include "NavigationDistanceEventMessage.pb.h"

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
namespace messages {
constexpr NavigationDistanceEvent::NavigationDistanceEvent(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : meters_(0u)
  , timetostepseconds_(0u)
  , distancetostepmillis_(0u)
  , distanceunit_(0)
{}
struct NavigationDistanceEventDefaultTypeInternal {
  constexpr NavigationDistanceEventDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~NavigationDistanceEventDefaultTypeInternal() {}
  union {
    NavigationDistanceEvent _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT NavigationDistanceEventDefaultTypeInternal _NavigationDistanceEvent_default_instance_;
}  // namespace messages
}  // namespace proto
}  // namespace aasdk
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_NavigationDistanceEventMessage_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_NavigationDistanceEventMessage_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_NavigationDistanceEventMessage_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_NavigationDistanceEventMessage_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::messages::NavigationDistanceEvent, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::messages::NavigationDistanceEvent, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::messages::NavigationDistanceEvent, meters_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::messages::NavigationDistanceEvent, timetostepseconds_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::messages::NavigationDistanceEvent, distancetostepmillis_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::messages::NavigationDistanceEvent, distanceunit_),
  0,
  1,
  2,
  3,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 9, sizeof(::aasdk::proto::messages::NavigationDistanceEvent)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::aasdk::proto::messages::_NavigationDistanceEvent_default_instance_),
};

const char descriptor_table_protodef_NavigationDistanceEventMessage_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n$NavigationDistanceEventMessage.proto\022\024"
  "aasdk.proto.messages\032\026DistanceUnitEnum.p"
  "roto\"\236\001\n\027NavigationDistanceEvent\022\016\n\006mete"
  "rs\030\001 \002(\r\022\031\n\021timeToStepSeconds\030\002 \002(\r\022\034\n\024d"
  "istanceToStepMillis\030\003 \002(\r\022:\n\014distanceUni"
  "t\030\004 \002(\0162$.aasdk.proto.enums.DistanceUnit"
  ".Enum"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_NavigationDistanceEventMessage_2eproto_deps[1] = {
  &::descriptor_table_DistanceUnitEnum_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_NavigationDistanceEventMessage_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_NavigationDistanceEventMessage_2eproto = {
  false, false, 245, descriptor_table_protodef_NavigationDistanceEventMessage_2eproto, "NavigationDistanceEventMessage.proto", 
  &descriptor_table_NavigationDistanceEventMessage_2eproto_once, descriptor_table_NavigationDistanceEventMessage_2eproto_deps, 1, 1,
  schemas, file_default_instances, TableStruct_NavigationDistanceEventMessage_2eproto::offsets,
  file_level_metadata_NavigationDistanceEventMessage_2eproto, file_level_enum_descriptors_NavigationDistanceEventMessage_2eproto, file_level_service_descriptors_NavigationDistanceEventMessage_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_NavigationDistanceEventMessage_2eproto_getter() {
  return &descriptor_table_NavigationDistanceEventMessage_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_NavigationDistanceEventMessage_2eproto(&descriptor_table_NavigationDistanceEventMessage_2eproto);
namespace aasdk {
namespace proto {
namespace messages {

// ===================================================================

class NavigationDistanceEvent::_Internal {
 public:
  using HasBits = decltype(std::declval<NavigationDistanceEvent>()._has_bits_);
  static void set_has_meters(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_timetostepseconds(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_distancetostepmillis(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_distanceunit(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x0000000f) ^ 0x0000000f) != 0;
  }
};

NavigationDistanceEvent::NavigationDistanceEvent(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:aasdk.proto.messages.NavigationDistanceEvent)
}
NavigationDistanceEvent::NavigationDistanceEvent(const NavigationDistanceEvent& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&meters_, &from.meters_,
    static_cast<size_t>(reinterpret_cast<char*>(&distanceunit_) -
    reinterpret_cast<char*>(&meters_)) + sizeof(distanceunit_));
  // @@protoc_insertion_point(copy_constructor:aasdk.proto.messages.NavigationDistanceEvent)
}

inline void NavigationDistanceEvent::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&meters_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&distanceunit_) -
    reinterpret_cast<char*>(&meters_)) + sizeof(distanceunit_));
}

NavigationDistanceEvent::~NavigationDistanceEvent() {
  // @@protoc_insertion_point(destructor:aasdk.proto.messages.NavigationDistanceEvent)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void NavigationDistanceEvent::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void NavigationDistanceEvent::ArenaDtor(void* object) {
  NavigationDistanceEvent* _this = reinterpret_cast< NavigationDistanceEvent* >(object);
  (void)_this;
}
void NavigationDistanceEvent::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void NavigationDistanceEvent::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void NavigationDistanceEvent::Clear() {
// @@protoc_insertion_point(message_clear_start:aasdk.proto.messages.NavigationDistanceEvent)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x0000000fu) {
    ::memset(&meters_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&distanceunit_) -
        reinterpret_cast<char*>(&meters_)) + sizeof(distanceunit_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* NavigationDistanceEvent::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // required uint32 meters = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_meters(&has_bits);
          meters_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required uint32 timeToStepSeconds = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_timetostepseconds(&has_bits);
          timetostepseconds_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required uint32 distanceToStepMillis = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          _Internal::set_has_distancetostepmillis(&has_bits);
          distancetostepmillis_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required .aasdk.proto.enums.DistanceUnit.Enum distanceUnit = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::aasdk::proto::enums::DistanceUnit_Enum_IsValid(val))) {
            _internal_set_distanceunit(static_cast<::aasdk::proto::enums::DistanceUnit_Enum>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(4, val, mutable_unknown_fields());
          }
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

::PROTOBUF_NAMESPACE_ID::uint8* NavigationDistanceEvent::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:aasdk.proto.messages.NavigationDistanceEvent)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required uint32 meters = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(1, this->_internal_meters(), target);
  }

  // required uint32 timeToStepSeconds = 2;
  if (cached_has_bits & 0x00000002u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(2, this->_internal_timetostepseconds(), target);
  }

  // required uint32 distanceToStepMillis = 3;
  if (cached_has_bits & 0x00000004u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(3, this->_internal_distancetostepmillis(), target);
  }

  // required .aasdk.proto.enums.DistanceUnit.Enum distanceUnit = 4;
  if (cached_has_bits & 0x00000008u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      4, this->_internal_distanceunit(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:aasdk.proto.messages.NavigationDistanceEvent)
  return target;
}

size_t NavigationDistanceEvent::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:aasdk.proto.messages.NavigationDistanceEvent)
  size_t total_size = 0;

  if (_internal_has_meters()) {
    // required uint32 meters = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_meters());
  }

  if (_internal_has_timetostepseconds()) {
    // required uint32 timeToStepSeconds = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_timetostepseconds());
  }

  if (_internal_has_distancetostepmillis()) {
    // required uint32 distanceToStepMillis = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_distancetostepmillis());
  }

  if (_internal_has_distanceunit()) {
    // required .aasdk.proto.enums.DistanceUnit.Enum distanceUnit = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_distanceunit());
  }

  return total_size;
}
size_t NavigationDistanceEvent::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:aasdk.proto.messages.NavigationDistanceEvent)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x0000000f) ^ 0x0000000f) == 0) {  // All required fields are present.
    // required uint32 meters = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_meters());

    // required uint32 timeToStepSeconds = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_timetostepseconds());

    // required uint32 distanceToStepMillis = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_distancetostepmillis());

    // required .aasdk.proto.enums.DistanceUnit.Enum distanceUnit = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_distanceunit());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData NavigationDistanceEvent::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    NavigationDistanceEvent::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*NavigationDistanceEvent::GetClassData() const { return &_class_data_; }

void NavigationDistanceEvent::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to,
                      const ::PROTOBUF_NAMESPACE_ID::Message&from) {
  static_cast<NavigationDistanceEvent *>(to)->MergeFrom(
      static_cast<const NavigationDistanceEvent &>(from));
}


void NavigationDistanceEvent::MergeFrom(const NavigationDistanceEvent& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:aasdk.proto.messages.NavigationDistanceEvent)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x0000000fu) {
    if (cached_has_bits & 0x00000001u) {
      meters_ = from.meters_;
    }
    if (cached_has_bits & 0x00000002u) {
      timetostepseconds_ = from.timetostepseconds_;
    }
    if (cached_has_bits & 0x00000004u) {
      distancetostepmillis_ = from.distancetostepmillis_;
    }
    if (cached_has_bits & 0x00000008u) {
      distanceunit_ = from.distanceunit_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void NavigationDistanceEvent::CopyFrom(const NavigationDistanceEvent& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:aasdk.proto.messages.NavigationDistanceEvent)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool NavigationDistanceEvent::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_has_bits_)) return false;
  return true;
}

void NavigationDistanceEvent::InternalSwap(NavigationDistanceEvent* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(NavigationDistanceEvent, distanceunit_)
      + sizeof(NavigationDistanceEvent::distanceunit_)
      - PROTOBUF_FIELD_OFFSET(NavigationDistanceEvent, meters_)>(
          reinterpret_cast<char*>(&meters_),
          reinterpret_cast<char*>(&other->meters_));
}

::PROTOBUF_NAMESPACE_ID::Metadata NavigationDistanceEvent::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_NavigationDistanceEventMessage_2eproto_getter, &descriptor_table_NavigationDistanceEventMessage_2eproto_once,
      file_level_metadata_NavigationDistanceEventMessage_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace messages
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::aasdk::proto::messages::NavigationDistanceEvent* Arena::CreateMaybeMessage< ::aasdk::proto::messages::NavigationDistanceEvent >(Arena* arena) {
  return Arena::CreateMessageInternal< ::aasdk::proto::messages::NavigationDistanceEvent >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
