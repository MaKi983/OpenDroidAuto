// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: VideoResolutionEnum.proto

#include "VideoResolutionEnum.pb.h"

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
namespace enums {
constexpr VideoResolution::VideoResolution(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized){}
struct VideoResolutionDefaultTypeInternal {
  constexpr VideoResolutionDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~VideoResolutionDefaultTypeInternal() {}
  union {
    VideoResolution _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT VideoResolutionDefaultTypeInternal _VideoResolution_default_instance_;
}  // namespace enums
}  // namespace proto
}  // namespace aasdk
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_VideoResolutionEnum_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_VideoResolutionEnum_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_VideoResolutionEnum_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_VideoResolutionEnum_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::enums::VideoResolution, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::aasdk::proto::enums::VideoResolution)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::aasdk::proto::enums::_VideoResolution_default_instance_),
};

const char descriptor_table_protodef_VideoResolutionEnum_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\031VideoResolutionEnum.proto\022\021aasdk.proto"
  ".enums\"z\n\017VideoResolution\"g\n\004Enum\022\010\n\004NON"
  "E\020\000\022\t\n\005_480p\020\001\022\t\n\005_720p\020\002\022\n\n\006_1080p\020\003\022\n\n"
  "\006_1440p\020\004\022\013\n\007_720p_p\020\005\022\013\n\007_1080pp\020\006\022\r\n\t_"
  "108s0p_p\020\007"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_VideoResolutionEnum_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_VideoResolutionEnum_2eproto = {
  false, false, 170, descriptor_table_protodef_VideoResolutionEnum_2eproto, "VideoResolutionEnum.proto", 
  &descriptor_table_VideoResolutionEnum_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_VideoResolutionEnum_2eproto::offsets,
  file_level_metadata_VideoResolutionEnum_2eproto, file_level_enum_descriptors_VideoResolutionEnum_2eproto, file_level_service_descriptors_VideoResolutionEnum_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_VideoResolutionEnum_2eproto_getter() {
  return &descriptor_table_VideoResolutionEnum_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_VideoResolutionEnum_2eproto(&descriptor_table_VideoResolutionEnum_2eproto);
namespace aasdk {
namespace proto {
namespace enums {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* VideoResolution_Enum_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_VideoResolutionEnum_2eproto);
  return file_level_enum_descriptors_VideoResolutionEnum_2eproto[0];
}
bool VideoResolution_Enum_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr VideoResolution_Enum VideoResolution::NONE;
constexpr VideoResolution_Enum VideoResolution::_480p;
constexpr VideoResolution_Enum VideoResolution::_720p;
constexpr VideoResolution_Enum VideoResolution::_1080p;
constexpr VideoResolution_Enum VideoResolution::_1440p;
constexpr VideoResolution_Enum VideoResolution::_720p_p;
constexpr VideoResolution_Enum VideoResolution::_1080pp;
constexpr VideoResolution_Enum VideoResolution::_108s0p_p;
constexpr VideoResolution_Enum VideoResolution::Enum_MIN;
constexpr VideoResolution_Enum VideoResolution::Enum_MAX;
constexpr int VideoResolution::Enum_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

class VideoResolution::_Internal {
 public:
};

VideoResolution::VideoResolution(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:aasdk.proto.enums.VideoResolution)
}
VideoResolution::VideoResolution(const VideoResolution& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:aasdk.proto.enums.VideoResolution)
}

inline void VideoResolution::SharedCtor() {
}

VideoResolution::~VideoResolution() {
  // @@protoc_insertion_point(destructor:aasdk.proto.enums.VideoResolution)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void VideoResolution::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void VideoResolution::ArenaDtor(void* object) {
  VideoResolution* _this = reinterpret_cast< VideoResolution* >(object);
  (void)_this;
}
void VideoResolution::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void VideoResolution::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void VideoResolution::Clear() {
// @@protoc_insertion_point(message_clear_start:aasdk.proto.enums.VideoResolution)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* VideoResolution::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
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
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* VideoResolution::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:aasdk.proto.enums.VideoResolution)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:aasdk.proto.enums.VideoResolution)
  return target;
}

size_t VideoResolution::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:aasdk.proto.enums.VideoResolution)
  size_t total_size = 0;

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

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData VideoResolution::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    VideoResolution::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*VideoResolution::GetClassData() const { return &_class_data_; }

void VideoResolution::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to,
                      const ::PROTOBUF_NAMESPACE_ID::Message&from) {
  static_cast<VideoResolution *>(to)->MergeFrom(
      static_cast<const VideoResolution &>(from));
}


void VideoResolution::MergeFrom(const VideoResolution& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:aasdk.proto.enums.VideoResolution)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void VideoResolution::CopyFrom(const VideoResolution& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:aasdk.proto.enums.VideoResolution)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool VideoResolution::IsInitialized() const {
  return true;
}

void VideoResolution::InternalSwap(VideoResolution* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
}

::PROTOBUF_NAMESPACE_ID::Metadata VideoResolution::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_VideoResolutionEnum_2eproto_getter, &descriptor_table_VideoResolutionEnum_2eproto_once,
      file_level_metadata_VideoResolutionEnum_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace enums
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::aasdk::proto::enums::VideoResolution* Arena::CreateMaybeMessage< ::aasdk::proto::enums::VideoResolution >(Arena* arena) {
  return Arena::CreateMessageInternal< ::aasdk::proto::enums::VideoResolution >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
