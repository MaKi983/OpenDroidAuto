// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: NavigationChannelMessageIdsEnum.proto

#include "NavigationChannelMessageIdsEnum.pb.h"

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
namespace ids {
constexpr NavigationChannelMessage::NavigationChannelMessage(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized){}
struct NavigationChannelMessageDefaultTypeInternal {
  constexpr NavigationChannelMessageDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~NavigationChannelMessageDefaultTypeInternal() {}
  union {
    NavigationChannelMessage _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT NavigationChannelMessageDefaultTypeInternal _NavigationChannelMessage_default_instance_;
}  // namespace ids
}  // namespace proto
}  // namespace aasdk
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_NavigationChannelMessageIdsEnum_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_NavigationChannelMessageIdsEnum_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_NavigationChannelMessageIdsEnum_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_NavigationChannelMessageIdsEnum_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::ids::NavigationChannelMessage, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::aasdk::proto::ids::NavigationChannelMessage)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::aasdk::proto::ids::_NavigationChannelMessage_default_instance_),
};

const char descriptor_table_protodef_NavigationChannelMessageIdsEnum_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n%NavigationChannelMessageIdsEnum.proto\022"
  "\017aasdk.proto.ids\"b\n\030NavigationChannelMes"
  "sage\"F\n\004Enum\022\010\n\004NONE\020\000\022\014\n\006STATUS\020\203\200\002\022\020\n\n"
  "TURN_EVENT\020\204\200\002\022\024\n\016DISTANCE_EVENT\020\205\200\002"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_NavigationChannelMessageIdsEnum_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_NavigationChannelMessageIdsEnum_2eproto = {
  false, false, 156, descriptor_table_protodef_NavigationChannelMessageIdsEnum_2eproto, "NavigationChannelMessageIdsEnum.proto", 
  &descriptor_table_NavigationChannelMessageIdsEnum_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_NavigationChannelMessageIdsEnum_2eproto::offsets,
  file_level_metadata_NavigationChannelMessageIdsEnum_2eproto, file_level_enum_descriptors_NavigationChannelMessageIdsEnum_2eproto, file_level_service_descriptors_NavigationChannelMessageIdsEnum_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_NavigationChannelMessageIdsEnum_2eproto_getter() {
  return &descriptor_table_NavigationChannelMessageIdsEnum_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_NavigationChannelMessageIdsEnum_2eproto(&descriptor_table_NavigationChannelMessageIdsEnum_2eproto);
namespace aasdk {
namespace proto {
namespace ids {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* NavigationChannelMessage_Enum_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_NavigationChannelMessageIdsEnum_2eproto);
  return file_level_enum_descriptors_NavigationChannelMessageIdsEnum_2eproto[0];
}
bool NavigationChannelMessage_Enum_IsValid(int value) {
  switch (value) {
    case 0:
    case 32771:
    case 32772:
    case 32773:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr NavigationChannelMessage_Enum NavigationChannelMessage::NONE;
constexpr NavigationChannelMessage_Enum NavigationChannelMessage::STATUS;
constexpr NavigationChannelMessage_Enum NavigationChannelMessage::TURN_EVENT;
constexpr NavigationChannelMessage_Enum NavigationChannelMessage::DISTANCE_EVENT;
constexpr NavigationChannelMessage_Enum NavigationChannelMessage::Enum_MIN;
constexpr NavigationChannelMessage_Enum NavigationChannelMessage::Enum_MAX;
constexpr int NavigationChannelMessage::Enum_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

class NavigationChannelMessage::_Internal {
 public:
};

NavigationChannelMessage::NavigationChannelMessage(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:aasdk.proto.ids.NavigationChannelMessage)
}
NavigationChannelMessage::NavigationChannelMessage(const NavigationChannelMessage& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:aasdk.proto.ids.NavigationChannelMessage)
}

inline void NavigationChannelMessage::SharedCtor() {
}

NavigationChannelMessage::~NavigationChannelMessage() {
  // @@protoc_insertion_point(destructor:aasdk.proto.ids.NavigationChannelMessage)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void NavigationChannelMessage::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void NavigationChannelMessage::ArenaDtor(void* object) {
  NavigationChannelMessage* _this = reinterpret_cast< NavigationChannelMessage* >(object);
  (void)_this;
}
void NavigationChannelMessage::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void NavigationChannelMessage::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void NavigationChannelMessage::Clear() {
// @@protoc_insertion_point(message_clear_start:aasdk.proto.ids.NavigationChannelMessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* NavigationChannelMessage::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
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

::PROTOBUF_NAMESPACE_ID::uint8* NavigationChannelMessage::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:aasdk.proto.ids.NavigationChannelMessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:aasdk.proto.ids.NavigationChannelMessage)
  return target;
}

size_t NavigationChannelMessage::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:aasdk.proto.ids.NavigationChannelMessage)
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

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData NavigationChannelMessage::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    NavigationChannelMessage::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*NavigationChannelMessage::GetClassData() const { return &_class_data_; }

void NavigationChannelMessage::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to,
                      const ::PROTOBUF_NAMESPACE_ID::Message&from) {
  static_cast<NavigationChannelMessage *>(to)->MergeFrom(
      static_cast<const NavigationChannelMessage &>(from));
}


void NavigationChannelMessage::MergeFrom(const NavigationChannelMessage& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:aasdk.proto.ids.NavigationChannelMessage)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void NavigationChannelMessage::CopyFrom(const NavigationChannelMessage& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:aasdk.proto.ids.NavigationChannelMessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool NavigationChannelMessage::IsInitialized() const {
  return true;
}

void NavigationChannelMessage::InternalSwap(NavigationChannelMessage* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
}

::PROTOBUF_NAMESPACE_ID::Metadata NavigationChannelMessage::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_NavigationChannelMessageIdsEnum_2eproto_getter, &descriptor_table_NavigationChannelMessageIdsEnum_2eproto_once,
      file_level_metadata_NavigationChannelMessageIdsEnum_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace ids
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::aasdk::proto::ids::NavigationChannelMessage* Arena::CreateMaybeMessage< ::aasdk::proto::ids::NavigationChannelMessage >(Arena* arena) {
  return Arena::CreateMessageInternal< ::aasdk::proto::ids::NavigationChannelMessage >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
