// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AVChannelSetupResponseMessage.proto

#include "AVChannelSetupResponseMessage.pb.h"

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
constexpr AVChannelSetupResponse::AVChannelSetupResponse(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : configs_()
  , media_status_(0)

  , max_unacked_(0u){}
struct AVChannelSetupResponseDefaultTypeInternal {
  constexpr AVChannelSetupResponseDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~AVChannelSetupResponseDefaultTypeInternal() {}
  union {
    AVChannelSetupResponse _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT AVChannelSetupResponseDefaultTypeInternal _AVChannelSetupResponse_default_instance_;
}  // namespace messages
}  // namespace proto
}  // namespace aasdk
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_AVChannelSetupResponseMessage_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_AVChannelSetupResponseMessage_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_AVChannelSetupResponseMessage_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_AVChannelSetupResponseMessage_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::messages::AVChannelSetupResponse, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::messages::AVChannelSetupResponse, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::messages::AVChannelSetupResponse, media_status_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::messages::AVChannelSetupResponse, max_unacked_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::messages::AVChannelSetupResponse, configs_),
  0,
  1,
  ~0u,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 8, sizeof(::aasdk::proto::messages::AVChannelSetupResponse)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::aasdk::proto::messages::_AVChannelSetupResponse_default_instance_),
};

const char descriptor_table_protodef_AVChannelSetupResponseMessage_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n#AVChannelSetupResponseMessage.proto\022\024a"
  "asdk.proto.messages\032\036AVChannelSetupStatu"
  "sEnum.proto\"\202\001\n\026AVChannelSetupResponse\022B"
  "\n\014media_status\030\001 \002(\0162,.aasdk.proto.enums"
  ".AVChannelSetupStatus.Enum\022\023\n\013max_unacke"
  "d\030\002 \002(\r\022\017\n\007configs\030\003 \003(\r"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_AVChannelSetupResponseMessage_2eproto_deps[1] = {
  &::descriptor_table_AVChannelSetupStatusEnum_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_AVChannelSetupResponseMessage_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_AVChannelSetupResponseMessage_2eproto = {
  false, false, 224, descriptor_table_protodef_AVChannelSetupResponseMessage_2eproto, "AVChannelSetupResponseMessage.proto", 
  &descriptor_table_AVChannelSetupResponseMessage_2eproto_once, descriptor_table_AVChannelSetupResponseMessage_2eproto_deps, 1, 1,
  schemas, file_default_instances, TableStruct_AVChannelSetupResponseMessage_2eproto::offsets,
  file_level_metadata_AVChannelSetupResponseMessage_2eproto, file_level_enum_descriptors_AVChannelSetupResponseMessage_2eproto, file_level_service_descriptors_AVChannelSetupResponseMessage_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_AVChannelSetupResponseMessage_2eproto_getter() {
  return &descriptor_table_AVChannelSetupResponseMessage_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_AVChannelSetupResponseMessage_2eproto(&descriptor_table_AVChannelSetupResponseMessage_2eproto);
namespace aasdk {
namespace proto {
namespace messages {

// ===================================================================

class AVChannelSetupResponse::_Internal {
 public:
  using HasBits = decltype(std::declval<AVChannelSetupResponse>()._has_bits_);
  static void set_has_media_status(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_max_unacked(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000003) ^ 0x00000003) != 0;
  }
};

AVChannelSetupResponse::AVChannelSetupResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  configs_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:aasdk.proto.messages.AVChannelSetupResponse)
}
AVChannelSetupResponse::AVChannelSetupResponse(const AVChannelSetupResponse& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_),
      configs_(from.configs_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&media_status_, &from.media_status_,
    static_cast<size_t>(reinterpret_cast<char*>(&max_unacked_) -
    reinterpret_cast<char*>(&media_status_)) + sizeof(max_unacked_));
  // @@protoc_insertion_point(copy_constructor:aasdk.proto.messages.AVChannelSetupResponse)
}

void AVChannelSetupResponse::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&media_status_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&max_unacked_) -
    reinterpret_cast<char*>(&media_status_)) + sizeof(max_unacked_));
}

AVChannelSetupResponse::~AVChannelSetupResponse() {
  // @@protoc_insertion_point(destructor:aasdk.proto.messages.AVChannelSetupResponse)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void AVChannelSetupResponse::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void AVChannelSetupResponse::ArenaDtor(void* object) {
  AVChannelSetupResponse* _this = reinterpret_cast< AVChannelSetupResponse* >(object);
  (void)_this;
}
void AVChannelSetupResponse::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void AVChannelSetupResponse::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void AVChannelSetupResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:aasdk.proto.messages.AVChannelSetupResponse)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  configs_.Clear();
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    ::memset(&media_status_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&max_unacked_) -
        reinterpret_cast<char*>(&media_status_)) + sizeof(max_unacked_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* AVChannelSetupResponse::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // required .aasdk.proto.enums.AVChannelSetupStatus.Enum media_status = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::aasdk::proto::enums::AVChannelSetupStatus_Enum_IsValid(val))) {
            _internal_set_media_status(static_cast<::aasdk::proto::enums::AVChannelSetupStatus_Enum>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(1, val, mutable_unknown_fields());
          }
        } else goto handle_unusual;
        continue;
      // required uint32 max_unacked = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_max_unacked(&has_bits);
          max_unacked_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated uint32 configs = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          ptr -= 1;
          do {
            ptr += 1;
            _internal_add_configs(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr));
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<24>(ptr));
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedUInt32Parser(_internal_mutable_configs(), ptr, ctx);
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

::PROTOBUF_NAMESPACE_ID::uint8* AVChannelSetupResponse::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:aasdk.proto.messages.AVChannelSetupResponse)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required .aasdk.proto.enums.AVChannelSetupStatus.Enum media_status = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      1, this->_internal_media_status(), target);
  }

  // required uint32 max_unacked = 2;
  if (cached_has_bits & 0x00000002u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(2, this->_internal_max_unacked(), target);
  }

  // repeated uint32 configs = 3;
  for (int i = 0, n = this->_internal_configs_size(); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(3, this->_internal_configs(i), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:aasdk.proto.messages.AVChannelSetupResponse)
  return target;
}

size_t AVChannelSetupResponse::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:aasdk.proto.messages.AVChannelSetupResponse)
  size_t total_size = 0;

  if (_internal_has_media_status()) {
    // required .aasdk.proto.enums.AVChannelSetupStatus.Enum media_status = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_media_status());
  }

  if (_internal_has_max_unacked()) {
    // required uint32 max_unacked = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_max_unacked());
  }

  return total_size;
}
size_t AVChannelSetupResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:aasdk.proto.messages.AVChannelSetupResponse)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x00000003) ^ 0x00000003) == 0) {  // All required fields are present.
    // required .aasdk.proto.enums.AVChannelSetupStatus.Enum media_status = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_media_status());

    // required uint32 max_unacked = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_max_unacked());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated uint32 configs = 3;
  {
    size_t data_size = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      UInt32Size(this->configs_);
    total_size += 1 *
                  ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(this->_internal_configs_size());
    total_size += data_size;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void AVChannelSetupResponse::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:aasdk.proto.messages.AVChannelSetupResponse)
  GOOGLE_DCHECK_NE(&from, this);
  const AVChannelSetupResponse* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<AVChannelSetupResponse>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:aasdk.proto.messages.AVChannelSetupResponse)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:aasdk.proto.messages.AVChannelSetupResponse)
    MergeFrom(*source);
  }
}

void AVChannelSetupResponse::MergeFrom(const AVChannelSetupResponse& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:aasdk.proto.messages.AVChannelSetupResponse)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  configs_.MergeFrom(from.configs_);
  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      media_status_ = from.media_status_;
    }
    if (cached_has_bits & 0x00000002u) {
      max_unacked_ = from.max_unacked_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void AVChannelSetupResponse::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:aasdk.proto.messages.AVChannelSetupResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void AVChannelSetupResponse::CopyFrom(const AVChannelSetupResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:aasdk.proto.messages.AVChannelSetupResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AVChannelSetupResponse::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_has_bits_)) return false;
  return true;
}

void AVChannelSetupResponse::InternalSwap(AVChannelSetupResponse* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  configs_.InternalSwap(&other->configs_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(AVChannelSetupResponse, max_unacked_)
      + sizeof(AVChannelSetupResponse::max_unacked_)
      - PROTOBUF_FIELD_OFFSET(AVChannelSetupResponse, media_status_)>(
          reinterpret_cast<char*>(&media_status_),
          reinterpret_cast<char*>(&other->media_status_));
}

::PROTOBUF_NAMESPACE_ID::Metadata AVChannelSetupResponse::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_AVChannelSetupResponseMessage_2eproto_getter, &descriptor_table_AVChannelSetupResponseMessage_2eproto_once,
      file_level_metadata_AVChannelSetupResponseMessage_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace messages
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::aasdk::proto::messages::AVChannelSetupResponse* Arena::CreateMaybeMessage< ::aasdk::proto::messages::AVChannelSetupResponse >(Arena* arena) {
  return Arena::CreateMessageInternal< ::aasdk::proto::messages::AVChannelSetupResponse >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
