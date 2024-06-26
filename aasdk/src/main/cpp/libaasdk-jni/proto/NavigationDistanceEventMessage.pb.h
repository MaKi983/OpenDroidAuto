// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: NavigationDistanceEventMessage.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_NavigationDistanceEventMessage_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_NavigationDistanceEventMessage_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017000 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/unknown_field_set.h>
#include "DistanceUnitEnum.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_NavigationDistanceEventMessage_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_NavigationDistanceEventMessage_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_NavigationDistanceEventMessage_2eproto;
namespace aasdk {
namespace proto {
namespace messages {
class NavigationDistanceEvent;
struct NavigationDistanceEventDefaultTypeInternal;
extern NavigationDistanceEventDefaultTypeInternal _NavigationDistanceEvent_default_instance_;
}  // namespace messages
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> ::aasdk::proto::messages::NavigationDistanceEvent* Arena::CreateMaybeMessage<::aasdk::proto::messages::NavigationDistanceEvent>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace aasdk {
namespace proto {
namespace messages {

// ===================================================================

class NavigationDistanceEvent final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:aasdk.proto.messages.NavigationDistanceEvent) */ {
 public:
  inline NavigationDistanceEvent() : NavigationDistanceEvent(nullptr) {}
  ~NavigationDistanceEvent() override;
  explicit constexpr NavigationDistanceEvent(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  NavigationDistanceEvent(const NavigationDistanceEvent& from);
  NavigationDistanceEvent(NavigationDistanceEvent&& from) noexcept
    : NavigationDistanceEvent() {
    *this = ::std::move(from);
  }

  inline NavigationDistanceEvent& operator=(const NavigationDistanceEvent& from) {
    CopyFrom(from);
    return *this;
  }
  inline NavigationDistanceEvent& operator=(NavigationDistanceEvent&& from) noexcept {
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
  static const NavigationDistanceEvent& default_instance() {
    return *internal_default_instance();
  }
  static inline const NavigationDistanceEvent* internal_default_instance() {
    return reinterpret_cast<const NavigationDistanceEvent*>(
               &_NavigationDistanceEvent_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(NavigationDistanceEvent& a, NavigationDistanceEvent& b) {
    a.Swap(&b);
  }
  inline void Swap(NavigationDistanceEvent* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(NavigationDistanceEvent* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline NavigationDistanceEvent* New() const final {
    return new NavigationDistanceEvent();
  }

  NavigationDistanceEvent* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<NavigationDistanceEvent>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const NavigationDistanceEvent& from);
  void MergeFrom(const NavigationDistanceEvent& from);
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
  void InternalSwap(NavigationDistanceEvent* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "aasdk.proto.messages.NavigationDistanceEvent";
  }
  protected:
  explicit NavigationDistanceEvent(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kMetersFieldNumber = 1,
    kTimeToStepSecondsFieldNumber = 2,
    kDistanceToStepMillisFieldNumber = 3,
    kDistanceUnitFieldNumber = 4,
  };
  // required uint32 meters = 1;
  bool has_meters() const;
  private:
  bool _internal_has_meters() const;
  public:
  void clear_meters();
  ::PROTOBUF_NAMESPACE_ID::uint32 meters() const;
  void set_meters(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_meters() const;
  void _internal_set_meters(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // required uint32 timeToStepSeconds = 2;
  bool has_timetostepseconds() const;
  private:
  bool _internal_has_timetostepseconds() const;
  public:
  void clear_timetostepseconds();
  ::PROTOBUF_NAMESPACE_ID::uint32 timetostepseconds() const;
  void set_timetostepseconds(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_timetostepseconds() const;
  void _internal_set_timetostepseconds(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // required uint32 distanceToStepMillis = 3;
  bool has_distancetostepmillis() const;
  private:
  bool _internal_has_distancetostepmillis() const;
  public:
  void clear_distancetostepmillis();
  ::PROTOBUF_NAMESPACE_ID::uint32 distancetostepmillis() const;
  void set_distancetostepmillis(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_distancetostepmillis() const;
  void _internal_set_distancetostepmillis(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // required .aasdk.proto.enums.DistanceUnit.Enum distanceUnit = 4;
  bool has_distanceunit() const;
  private:
  bool _internal_has_distanceunit() const;
  public:
  void clear_distanceunit();
  ::aasdk::proto::enums::DistanceUnit_Enum distanceunit() const;
  void set_distanceunit(::aasdk::proto::enums::DistanceUnit_Enum value);
  private:
  ::aasdk::proto::enums::DistanceUnit_Enum _internal_distanceunit() const;
  void _internal_set_distanceunit(::aasdk::proto::enums::DistanceUnit_Enum value);
  public:

  // @@protoc_insertion_point(class_scope:aasdk.proto.messages.NavigationDistanceEvent)
 private:
  class _Internal;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::uint32 meters_;
  ::PROTOBUF_NAMESPACE_ID::uint32 timetostepseconds_;
  ::PROTOBUF_NAMESPACE_ID::uint32 distancetostepmillis_;
  int distanceunit_;
  friend struct ::TableStruct_NavigationDistanceEventMessage_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// NavigationDistanceEvent

// required uint32 meters = 1;
inline bool NavigationDistanceEvent::_internal_has_meters() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool NavigationDistanceEvent::has_meters() const {
  return _internal_has_meters();
}
inline void NavigationDistanceEvent::clear_meters() {
  meters_ = 0u;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NavigationDistanceEvent::_internal_meters() const {
  return meters_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NavigationDistanceEvent::meters() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.messages.NavigationDistanceEvent.meters)
  return _internal_meters();
}
inline void NavigationDistanceEvent::_internal_set_meters(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _has_bits_[0] |= 0x00000001u;
  meters_ = value;
}
inline void NavigationDistanceEvent::set_meters(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_meters(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.messages.NavigationDistanceEvent.meters)
}

// required uint32 timeToStepSeconds = 2;
inline bool NavigationDistanceEvent::_internal_has_timetostepseconds() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool NavigationDistanceEvent::has_timetostepseconds() const {
  return _internal_has_timetostepseconds();
}
inline void NavigationDistanceEvent::clear_timetostepseconds() {
  timetostepseconds_ = 0u;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NavigationDistanceEvent::_internal_timetostepseconds() const {
  return timetostepseconds_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NavigationDistanceEvent::timetostepseconds() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.messages.NavigationDistanceEvent.timeToStepSeconds)
  return _internal_timetostepseconds();
}
inline void NavigationDistanceEvent::_internal_set_timetostepseconds(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _has_bits_[0] |= 0x00000002u;
  timetostepseconds_ = value;
}
inline void NavigationDistanceEvent::set_timetostepseconds(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_timetostepseconds(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.messages.NavigationDistanceEvent.timeToStepSeconds)
}

// required uint32 distanceToStepMillis = 3;
inline bool NavigationDistanceEvent::_internal_has_distancetostepmillis() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool NavigationDistanceEvent::has_distancetostepmillis() const {
  return _internal_has_distancetostepmillis();
}
inline void NavigationDistanceEvent::clear_distancetostepmillis() {
  distancetostepmillis_ = 0u;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NavigationDistanceEvent::_internal_distancetostepmillis() const {
  return distancetostepmillis_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NavigationDistanceEvent::distancetostepmillis() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.messages.NavigationDistanceEvent.distanceToStepMillis)
  return _internal_distancetostepmillis();
}
inline void NavigationDistanceEvent::_internal_set_distancetostepmillis(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _has_bits_[0] |= 0x00000004u;
  distancetostepmillis_ = value;
}
inline void NavigationDistanceEvent::set_distancetostepmillis(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_distancetostepmillis(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.messages.NavigationDistanceEvent.distanceToStepMillis)
}

// required .aasdk.proto.enums.DistanceUnit.Enum distanceUnit = 4;
inline bool NavigationDistanceEvent::_internal_has_distanceunit() const {
  bool value = (_has_bits_[0] & 0x00000008u) != 0;
  return value;
}
inline bool NavigationDistanceEvent::has_distanceunit() const {
  return _internal_has_distanceunit();
}
inline void NavigationDistanceEvent::clear_distanceunit() {
  distanceunit_ = 0;
  _has_bits_[0] &= ~0x00000008u;
}
inline ::aasdk::proto::enums::DistanceUnit_Enum NavigationDistanceEvent::_internal_distanceunit() const {
  return static_cast< ::aasdk::proto::enums::DistanceUnit_Enum >(distanceunit_);
}
inline ::aasdk::proto::enums::DistanceUnit_Enum NavigationDistanceEvent::distanceunit() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.messages.NavigationDistanceEvent.distanceUnit)
  return _internal_distanceunit();
}
inline void NavigationDistanceEvent::_internal_set_distanceunit(::aasdk::proto::enums::DistanceUnit_Enum value) {
  assert(::aasdk::proto::enums::DistanceUnit_Enum_IsValid(value));
  _has_bits_[0] |= 0x00000008u;
  distanceunit_ = value;
}
inline void NavigationDistanceEvent::set_distanceunit(::aasdk::proto::enums::DistanceUnit_Enum value) {
  _internal_set_distanceunit(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.messages.NavigationDistanceEvent.distanceUnit)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace messages
}  // namespace proto
}  // namespace aasdk

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_NavigationDistanceEventMessage_2eproto
