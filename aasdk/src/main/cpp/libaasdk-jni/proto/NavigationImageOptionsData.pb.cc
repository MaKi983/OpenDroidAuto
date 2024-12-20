// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: NavigationImageOptionsData.proto

#include "NavigationImageOptionsData.pb.h"

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
constexpr NavigationImageOptions::NavigationImageOptions(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : width_(0)
  , height_(0)
  , colour_depth_bits_(0)
  , dunno_(0){}
struct NavigationImageOptionsDefaultTypeInternal {
  constexpr NavigationImageOptionsDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~NavigationImageOptionsDefaultTypeInternal() {}
  union {
    NavigationImageOptions _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT NavigationImageOptionsDefaultTypeInternal _NavigationImageOptions_default_instance_;
}  // namespace data
}  // namespace proto
}  // namespace aasdk
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_NavigationImageOptionsData_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_NavigationImageOptionsData_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_NavigationImageOptionsData_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_NavigationImageOptionsData_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::NavigationImageOptions, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::NavigationImageOptions, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::NavigationImageOptions, width_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::NavigationImageOptions, height_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::NavigationImageOptions, colour_depth_bits_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::NavigationImageOptions, dunno_),
  0,
  1,
  2,
  3,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 9, sizeof(::aasdk::proto::data::NavigationImageOptions)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::aasdk::proto::data::_NavigationImageOptions_default_instance_),
};

const char descriptor_table_protodef_NavigationImageOptionsData_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n NavigationImageOptionsData.proto\022\020aasd"
  "k.proto.data\"a\n\026NavigationImageOptions\022\r"
  "\n\005width\030\001 \002(\005\022\016\n\006height\030\002 \002(\005\022\031\n\021colour_"
  "depth_bits\030\003 \002(\005\022\r\n\005dunno\030\004 \002(\005"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_NavigationImageOptionsData_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_NavigationImageOptionsData_2eproto = {
  false, false, 151, descriptor_table_protodef_NavigationImageOptionsData_2eproto, "NavigationImageOptionsData.proto", 
  &descriptor_table_NavigationImageOptionsData_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_NavigationImageOptionsData_2eproto::offsets,
  file_level_metadata_NavigationImageOptionsData_2eproto, file_level_enum_descriptors_NavigationImageOptionsData_2eproto, file_level_service_descriptors_NavigationImageOptionsData_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_NavigationImageOptionsData_2eproto_getter() {
  return &descriptor_table_NavigationImageOptionsData_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_NavigationImageOptionsData_2eproto(&descriptor_table_NavigationImageOptionsData_2eproto);
namespace aasdk {
namespace proto {
namespace data {

// ===================================================================

class NavigationImageOptions::_Internal {
 public:
  using HasBits = decltype(std::declval<NavigationImageOptions>()._has_bits_);
  static void set_has_width(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_height(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_colour_depth_bits(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_dunno(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x0000000f) ^ 0x0000000f) != 0;
  }
};

NavigationImageOptions::NavigationImageOptions(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:aasdk.proto.data.NavigationImageOptions)
}
NavigationImageOptions::NavigationImageOptions(const NavigationImageOptions& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&width_, &from.width_,
    static_cast<size_t>(reinterpret_cast<char*>(&dunno_) -
    reinterpret_cast<char*>(&width_)) + sizeof(dunno_));
  // @@protoc_insertion_point(copy_constructor:aasdk.proto.data.NavigationImageOptions)
}

void NavigationImageOptions::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&width_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&dunno_) -
    reinterpret_cast<char*>(&width_)) + sizeof(dunno_));
}

NavigationImageOptions::~NavigationImageOptions() {
  // @@protoc_insertion_point(destructor:aasdk.proto.data.NavigationImageOptions)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void NavigationImageOptions::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void NavigationImageOptions::ArenaDtor(void* object) {
  NavigationImageOptions* _this = reinterpret_cast< NavigationImageOptions* >(object);
  (void)_this;
}
void NavigationImageOptions::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void NavigationImageOptions::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void NavigationImageOptions::Clear() {
// @@protoc_insertion_point(message_clear_start:aasdk.proto.data.NavigationImageOptions)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x0000000fu) {
    ::memset(&width_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&dunno_) -
        reinterpret_cast<char*>(&width_)) + sizeof(dunno_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* NavigationImageOptions::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // required int32 width = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_width(&has_bits);
          width_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required int32 height = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_height(&has_bits);
          height_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required int32 colour_depth_bits = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          _Internal::set_has_colour_depth_bits(&has_bits);
          colour_depth_bits_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required int32 dunno = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          _Internal::set_has_dunno(&has_bits);
          dunno_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
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

::PROTOBUF_NAMESPACE_ID::uint8* NavigationImageOptions::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:aasdk.proto.data.NavigationImageOptions)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required int32 width = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_width(), target);
  }

  // required int32 height = 2;
  if (cached_has_bits & 0x00000002u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_height(), target);
  }

  // required int32 colour_depth_bits = 3;
  if (cached_has_bits & 0x00000004u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(3, this->_internal_colour_depth_bits(), target);
  }

  // required int32 dunno = 4;
  if (cached_has_bits & 0x00000008u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(4, this->_internal_dunno(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:aasdk.proto.data.NavigationImageOptions)
  return target;
}

size_t NavigationImageOptions::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:aasdk.proto.data.NavigationImageOptions)
  size_t total_size = 0;

  if (_internal_has_width()) {
    // required int32 width = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_width());
  }

  if (_internal_has_height()) {
    // required int32 height = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_height());
  }

  if (_internal_has_colour_depth_bits()) {
    // required int32 colour_depth_bits = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_colour_depth_bits());
  }

  if (_internal_has_dunno()) {
    // required int32 dunno = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_dunno());
  }

  return total_size;
}
size_t NavigationImageOptions::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:aasdk.proto.data.NavigationImageOptions)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x0000000f) ^ 0x0000000f) == 0) {  // All required fields are present.
    // required int32 width = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_width());

    // required int32 height = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_height());

    // required int32 colour_depth_bits = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_colour_depth_bits());

    // required int32 dunno = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_dunno());

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

void NavigationImageOptions::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:aasdk.proto.data.NavigationImageOptions)
  GOOGLE_DCHECK_NE(&from, this);
  const NavigationImageOptions* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<NavigationImageOptions>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:aasdk.proto.data.NavigationImageOptions)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:aasdk.proto.data.NavigationImageOptions)
    MergeFrom(*source);
  }
}

void NavigationImageOptions::MergeFrom(const NavigationImageOptions& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:aasdk.proto.data.NavigationImageOptions)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x0000000fu) {
    if (cached_has_bits & 0x00000001u) {
      width_ = from.width_;
    }
    if (cached_has_bits & 0x00000002u) {
      height_ = from.height_;
    }
    if (cached_has_bits & 0x00000004u) {
      colour_depth_bits_ = from.colour_depth_bits_;
    }
    if (cached_has_bits & 0x00000008u) {
      dunno_ = from.dunno_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void NavigationImageOptions::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:aasdk.proto.data.NavigationImageOptions)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void NavigationImageOptions::CopyFrom(const NavigationImageOptions& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:aasdk.proto.data.NavigationImageOptions)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool NavigationImageOptions::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_has_bits_)) return false;
  return true;
}

void NavigationImageOptions::InternalSwap(NavigationImageOptions* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(NavigationImageOptions, dunno_)
      + sizeof(NavigationImageOptions::dunno_)
      - PROTOBUF_FIELD_OFFSET(NavigationImageOptions, width_)>(
          reinterpret_cast<char*>(&width_),
          reinterpret_cast<char*>(&other->width_));
}

::PROTOBUF_NAMESPACE_ID::Metadata NavigationImageOptions::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_NavigationImageOptionsData_2eproto_getter, &descriptor_table_NavigationImageOptionsData_2eproto_once,
      file_level_metadata_NavigationImageOptionsData_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace data
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::aasdk::proto::data::NavigationImageOptions* Arena::CreateMaybeMessage< ::aasdk::proto::data::NavigationImageOptions >(Arena* arena) {
  return Arena::CreateMessageInternal< ::aasdk::proto::data::NavigationImageOptions >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
