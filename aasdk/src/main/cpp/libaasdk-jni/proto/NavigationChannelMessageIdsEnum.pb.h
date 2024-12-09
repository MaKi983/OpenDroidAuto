// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: NavigationChannelMessageIdsEnum.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_NavigationChannelMessageIdsEnum_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_NavigationChannelMessageIdsEnum_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_NavigationChannelMessageIdsEnum_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_NavigationChannelMessageIdsEnum_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_NavigationChannelMessageIdsEnum_2eproto;
namespace aasdk {
namespace proto {
namespace ids {
class NavigationChannelMessage;
struct NavigationChannelMessageDefaultTypeInternal;
extern NavigationChannelMessageDefaultTypeInternal _NavigationChannelMessage_default_instance_;
}  // namespace ids
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> ::aasdk::proto::ids::NavigationChannelMessage* Arena::CreateMaybeMessage<::aasdk::proto::ids::NavigationChannelMessage>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace aasdk {
namespace proto {
namespace ids {

enum NavigationChannelMessage_Enum : int {
  NavigationChannelMessage_Enum_NONE = 0,
  NavigationChannelMessage_Enum_STATUS = 32771,
  NavigationChannelMessage_Enum_TURN_EVENT = 32772,
  NavigationChannelMessage_Enum_DISTANCE_EVENT = 32773
};
bool NavigationChannelMessage_Enum_IsValid(int value);
constexpr NavigationChannelMessage_Enum NavigationChannelMessage_Enum_Enum_MIN = NavigationChannelMessage_Enum_NONE;
constexpr NavigationChannelMessage_Enum NavigationChannelMessage_Enum_Enum_MAX = NavigationChannelMessage_Enum_DISTANCE_EVENT;
constexpr int NavigationChannelMessage_Enum_Enum_ARRAYSIZE = NavigationChannelMessage_Enum_Enum_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* NavigationChannelMessage_Enum_descriptor();
template<typename T>
inline const std::string& NavigationChannelMessage_Enum_Name(T enum_t_value) {
  static_assert(::std::is_same<T, NavigationChannelMessage_Enum>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function NavigationChannelMessage_Enum_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    NavigationChannelMessage_Enum_descriptor(), enum_t_value);
}
inline bool NavigationChannelMessage_Enum_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, NavigationChannelMessage_Enum* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<NavigationChannelMessage_Enum>(
    NavigationChannelMessage_Enum_descriptor(), name, value);
}
// ===================================================================

class NavigationChannelMessage final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:aasdk.proto.ids.NavigationChannelMessage) */ {
 public:
  inline NavigationChannelMessage() : NavigationChannelMessage(nullptr) {}
  ~NavigationChannelMessage() override;
  explicit constexpr NavigationChannelMessage(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  NavigationChannelMessage(const NavigationChannelMessage& from);
  NavigationChannelMessage(NavigationChannelMessage&& from) noexcept
    : NavigationChannelMessage() {
    *this = ::std::move(from);
  }

  inline NavigationChannelMessage& operator=(const NavigationChannelMessage& from) {
    CopyFrom(from);
    return *this;
  }
  inline NavigationChannelMessage& operator=(NavigationChannelMessage&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const NavigationChannelMessage& default_instance() {
    return *internal_default_instance();
  }
  static inline const NavigationChannelMessage* internal_default_instance() {
    return reinterpret_cast<const NavigationChannelMessage*>(
               &_NavigationChannelMessage_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(NavigationChannelMessage& a, NavigationChannelMessage& b) {
    a.Swap(&b);
  }
  inline void Swap(NavigationChannelMessage* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(NavigationChannelMessage* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline NavigationChannelMessage* New() const final {
    return new NavigationChannelMessage();
  }

  NavigationChannelMessage* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<NavigationChannelMessage>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const NavigationChannelMessage& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const NavigationChannelMessage& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to, const ::PROTOBUF_NAMESPACE_ID::Message&from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(NavigationChannelMessage* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "aasdk.proto.ids.NavigationChannelMessage";
  }
  protected:
  explicit NavigationChannelMessage(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef NavigationChannelMessage_Enum Enum;
  static constexpr Enum NONE =
    NavigationChannelMessage_Enum_NONE;
  static constexpr Enum STATUS =
    NavigationChannelMessage_Enum_STATUS;
  static constexpr Enum TURN_EVENT =
    NavigationChannelMessage_Enum_TURN_EVENT;
  static constexpr Enum DISTANCE_EVENT =
    NavigationChannelMessage_Enum_DISTANCE_EVENT;
  static inline bool Enum_IsValid(int value) {
    return NavigationChannelMessage_Enum_IsValid(value);
  }
  static constexpr Enum Enum_MIN =
    NavigationChannelMessage_Enum_Enum_MIN;
  static constexpr Enum Enum_MAX =
    NavigationChannelMessage_Enum_Enum_MAX;
  static constexpr int Enum_ARRAYSIZE =
    NavigationChannelMessage_Enum_Enum_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  Enum_descriptor() {
    return NavigationChannelMessage_Enum_descriptor();
  }
  template<typename T>
  static inline const std::string& Enum_Name(T enum_t_value) {
    static_assert(::std::is_same<T, Enum>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function Enum_Name.");
    return NavigationChannelMessage_Enum_Name(enum_t_value);
  }
  static inline bool Enum_Parse(::PROTOBUF_NAMESPACE_ID::ConstStringParam name,
      Enum* value) {
    return NavigationChannelMessage_Enum_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:aasdk.proto.ids.NavigationChannelMessage)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_NavigationChannelMessageIdsEnum_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// NavigationChannelMessage

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace ids
}  // namespace proto
}  // namespace aasdk

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::aasdk::proto::ids::NavigationChannelMessage_Enum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::aasdk::proto::ids::NavigationChannelMessage_Enum>() {
  return ::aasdk::proto::ids::NavigationChannelMessage_Enum_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_NavigationChannelMessageIdsEnum_2eproto
