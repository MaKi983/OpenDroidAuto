// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: SensorStartResponseMessage.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_SensorStartResponseMessage_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_SensorStartResponseMessage_2eproto

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
#include "StatusEnum.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_SensorStartResponseMessage_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_SensorStartResponseMessage_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_SensorStartResponseMessage_2eproto;
namespace aasdk {
namespace proto {
namespace messages {
class SensorStartResponseMessage;
struct SensorStartResponseMessageDefaultTypeInternal;
extern SensorStartResponseMessageDefaultTypeInternal _SensorStartResponseMessage_default_instance_;
}  // namespace messages
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> ::aasdk::proto::messages::SensorStartResponseMessage* Arena::CreateMaybeMessage<::aasdk::proto::messages::SensorStartResponseMessage>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace aasdk {
namespace proto {
namespace messages {

// ===================================================================

class SensorStartResponseMessage final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:aasdk.proto.messages.SensorStartResponseMessage) */ {
 public:
  inline SensorStartResponseMessage() : SensorStartResponseMessage(nullptr) {}
  ~SensorStartResponseMessage() override;
  explicit constexpr SensorStartResponseMessage(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  SensorStartResponseMessage(const SensorStartResponseMessage& from);
  SensorStartResponseMessage(SensorStartResponseMessage&& from) noexcept
    : SensorStartResponseMessage() {
    *this = ::std::move(from);
  }

  inline SensorStartResponseMessage& operator=(const SensorStartResponseMessage& from) {
    CopyFrom(from);
    return *this;
  }
  inline SensorStartResponseMessage& operator=(SensorStartResponseMessage&& from) noexcept {
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
  static const SensorStartResponseMessage& default_instance() {
    return *internal_default_instance();
  }
  static inline const SensorStartResponseMessage* internal_default_instance() {
    return reinterpret_cast<const SensorStartResponseMessage*>(
               &_SensorStartResponseMessage_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(SensorStartResponseMessage& a, SensorStartResponseMessage& b) {
    a.Swap(&b);
  }
  inline void Swap(SensorStartResponseMessage* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SensorStartResponseMessage* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SensorStartResponseMessage* New() const final {
    return new SensorStartResponseMessage();
  }

  SensorStartResponseMessage* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SensorStartResponseMessage>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const SensorStartResponseMessage& from);
  void MergeFrom(const SensorStartResponseMessage& from);
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
  void InternalSwap(SensorStartResponseMessage* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "aasdk.proto.messages.SensorStartResponseMessage";
  }
  protected:
  explicit SensorStartResponseMessage(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kStatusFieldNumber = 1,
  };
  // required .aasdk.proto.enums.Status.Enum status = 1;
  bool has_status() const;
  private:
  bool _internal_has_status() const;
  public:
  void clear_status();
  ::aasdk::proto::enums::Status_Enum status() const;
  void set_status(::aasdk::proto::enums::Status_Enum value);
  private:
  ::aasdk::proto::enums::Status_Enum _internal_status() const;
  void _internal_set_status(::aasdk::proto::enums::Status_Enum value);
  public:

  // @@protoc_insertion_point(class_scope:aasdk.proto.messages.SensorStartResponseMessage)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  int status_;
  friend struct ::TableStruct_SensorStartResponseMessage_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// SensorStartResponseMessage

// required .aasdk.proto.enums.Status.Enum status = 1;
inline bool SensorStartResponseMessage::_internal_has_status() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool SensorStartResponseMessage::has_status() const {
  return _internal_has_status();
}
inline void SensorStartResponseMessage::clear_status() {
  status_ = 0;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::aasdk::proto::enums::Status_Enum SensorStartResponseMessage::_internal_status() const {
  return static_cast< ::aasdk::proto::enums::Status_Enum >(status_);
}
inline ::aasdk::proto::enums::Status_Enum SensorStartResponseMessage::status() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.messages.SensorStartResponseMessage.status)
  return _internal_status();
}
inline void SensorStartResponseMessage::_internal_set_status(::aasdk::proto::enums::Status_Enum value) {
  assert(::aasdk::proto::enums::Status_Enum_IsValid(value));
  _has_bits_[0] |= 0x00000001u;
  status_ = value;
}
inline void SensorStartResponseMessage::set_status(::aasdk::proto::enums::Status_Enum value) {
  _internal_set_status(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.messages.SensorStartResponseMessage.status)
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
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_SensorStartResponseMessage_2eproto
