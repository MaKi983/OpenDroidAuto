// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: RelativeInputEventData.proto

#include "RelativeInputEventData.pb.h"

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
constexpr RelativeInputEvent::RelativeInputEvent(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : scan_code_(0u)
  , delta_(0){}
struct RelativeInputEventDefaultTypeInternal {
  constexpr RelativeInputEventDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~RelativeInputEventDefaultTypeInternal() {}
  union {
    RelativeInputEvent _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT RelativeInputEventDefaultTypeInternal _RelativeInputEvent_default_instance_;
}  // namespace data
}  // namespace proto
}  // namespace aasdk
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_RelativeInputEventData_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_RelativeInputEventData_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_RelativeInputEventData_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_RelativeInputEventData_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::RelativeInputEvent, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::RelativeInputEvent, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::RelativeInputEvent, scan_code_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::RelativeInputEvent, delta_),
  0,
  1,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 7, sizeof(::aasdk::proto::data::RelativeInputEvent)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::aasdk::proto::data::_RelativeInputEvent_default_instance_),
};

const char descriptor_table_protodef_RelativeInputEventData_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\034RelativeInputEventData.proto\022\020aasdk.pr"
  "oto.data\"6\n\022RelativeInputEvent\022\021\n\tscan_c"
  "ode\030\001 \002(\r\022\r\n\005delta\030\002 \002(\005"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_RelativeInputEventData_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_RelativeInputEventData_2eproto = {
  false, false, 104, descriptor_table_protodef_RelativeInputEventData_2eproto, "RelativeInputEventData.proto", 
  &descriptor_table_RelativeInputEventData_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_RelativeInputEventData_2eproto::offsets,
  file_level_metadata_RelativeInputEventData_2eproto, file_level_enum_descriptors_RelativeInputEventData_2eproto, file_level_service_descriptors_RelativeInputEventData_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_RelativeInputEventData_2eproto_getter() {
  return &descriptor_table_RelativeInputEventData_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_RelativeInputEventData_2eproto(&descriptor_table_RelativeInputEventData_2eproto);
namespace aasdk {
namespace proto {
namespace data {

// ===================================================================

class RelativeInputEvent::_Internal {
 public:
  using HasBits = decltype(std::declval<RelativeInputEvent>()._has_bits_);
  static void set_has_scan_code(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_delta(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000003) ^ 0x00000003) != 0;
  }
};

RelativeInputEvent::RelativeInputEvent(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:aasdk.proto.data.RelativeInputEvent)
}
RelativeInputEvent::RelativeInputEvent(const RelativeInputEvent& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&scan_code_, &from.scan_code_,
    static_cast<size_t>(reinterpret_cast<char*>(&delta_) -
    reinterpret_cast<char*>(&scan_code_)) + sizeof(delta_));
  // @@protoc_insertion_point(copy_constructor:aasdk.proto.data.RelativeInputEvent)
}

inline void RelativeInputEvent::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&scan_code_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&delta_) -
    reinterpret_cast<char*>(&scan_code_)) + sizeof(delta_));
}

RelativeInputEvent::~RelativeInputEvent() {
  // @@protoc_insertion_point(destructor:aasdk.proto.data.RelativeInputEvent)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void RelativeInputEvent::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void RelativeInputEvent::ArenaDtor(void* object) {
  RelativeInputEvent* _this = reinterpret_cast< RelativeInputEvent* >(object);
  (void)_this;
}
void RelativeInputEvent::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void RelativeInputEvent::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void RelativeInputEvent::Clear() {
// @@protoc_insertion_point(message_clear_start:aasdk.proto.data.RelativeInputEvent)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    ::memset(&scan_code_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&delta_) -
        reinterpret_cast<char*>(&scan_code_)) + sizeof(delta_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* RelativeInputEvent::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // required uint32 scan_code = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_scan_code(&has_bits);
          scan_code_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required int32 delta = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_delta(&has_bits);
          delta_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
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

::PROTOBUF_NAMESPACE_ID::uint8* RelativeInputEvent::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:aasdk.proto.data.RelativeInputEvent)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required uint32 scan_code = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(1, this->_internal_scan_code(), target);
  }

  // required int32 delta = 2;
  if (cached_has_bits & 0x00000002u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_delta(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:aasdk.proto.data.RelativeInputEvent)
  return target;
}

size_t RelativeInputEvent::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:aasdk.proto.data.RelativeInputEvent)
  size_t total_size = 0;

  if (_internal_has_scan_code()) {
    // required uint32 scan_code = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_scan_code());
  }

  if (_internal_has_delta()) {
    // required int32 delta = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_delta());
  }

  return total_size;
}
size_t RelativeInputEvent::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:aasdk.proto.data.RelativeInputEvent)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x00000003) ^ 0x00000003) == 0) {  // All required fields are present.
    // required uint32 scan_code = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_scan_code());

    // required int32 delta = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_delta());

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

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData RelativeInputEvent::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    RelativeInputEvent::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*RelativeInputEvent::GetClassData() const { return &_class_data_; }

void RelativeInputEvent::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to,
                      const ::PROTOBUF_NAMESPACE_ID::Message&from) {
  static_cast<RelativeInputEvent *>(to)->MergeFrom(
      static_cast<const RelativeInputEvent &>(from));
}


void RelativeInputEvent::MergeFrom(const RelativeInputEvent& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:aasdk.proto.data.RelativeInputEvent)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      scan_code_ = from.scan_code_;
    }
    if (cached_has_bits & 0x00000002u) {
      delta_ = from.delta_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void RelativeInputEvent::CopyFrom(const RelativeInputEvent& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:aasdk.proto.data.RelativeInputEvent)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RelativeInputEvent::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_has_bits_)) return false;
  return true;
}

void RelativeInputEvent::InternalSwap(RelativeInputEvent* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(RelativeInputEvent, delta_)
      + sizeof(RelativeInputEvent::delta_)
      - PROTOBUF_FIELD_OFFSET(RelativeInputEvent, scan_code_)>(
          reinterpret_cast<char*>(&scan_code_),
          reinterpret_cast<char*>(&other->scan_code_));
}

::PROTOBUF_NAMESPACE_ID::Metadata RelativeInputEvent::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_RelativeInputEventData_2eproto_getter, &descriptor_table_RelativeInputEventData_2eproto_once,
      file_level_metadata_RelativeInputEventData_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace data
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::aasdk::proto::data::RelativeInputEvent* Arena::CreateMaybeMessage< ::aasdk::proto::data::RelativeInputEvent >(Arena* arena) {
  return Arena::CreateMessageInternal< ::aasdk::proto::data::RelativeInputEvent >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
