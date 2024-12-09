// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AVChannelStartIndicationMessage.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_AVChannelStartIndicationMessage_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_AVChannelStartIndicationMessage_2eproto

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
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_AVChannelStartIndicationMessage_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_AVChannelStartIndicationMessage_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_AVChannelStartIndicationMessage_2eproto;
namespace aasdk {
namespace proto {
namespace messages {
class AVChannelStartIndication;
struct AVChannelStartIndicationDefaultTypeInternal;
extern AVChannelStartIndicationDefaultTypeInternal _AVChannelStartIndication_default_instance_;
}  // namespace messages
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> ::aasdk::proto::messages::AVChannelStartIndication* Arena::CreateMaybeMessage<::aasdk::proto::messages::AVChannelStartIndication>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace aasdk {
namespace proto {
namespace messages {

// ===================================================================

class AVChannelStartIndication final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:aasdk.proto.messages.AVChannelStartIndication) */ {
 public:
  inline AVChannelStartIndication() : AVChannelStartIndication(nullptr) {}
  ~AVChannelStartIndication() override;
  explicit constexpr AVChannelStartIndication(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  AVChannelStartIndication(const AVChannelStartIndication& from);
  AVChannelStartIndication(AVChannelStartIndication&& from) noexcept
    : AVChannelStartIndication() {
    *this = ::std::move(from);
  }

  inline AVChannelStartIndication& operator=(const AVChannelStartIndication& from) {
    CopyFrom(from);
    return *this;
  }
  inline AVChannelStartIndication& operator=(AVChannelStartIndication&& from) noexcept {
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
  static const AVChannelStartIndication& default_instance() {
    return *internal_default_instance();
  }
  static inline const AVChannelStartIndication* internal_default_instance() {
    return reinterpret_cast<const AVChannelStartIndication*>(
               &_AVChannelStartIndication_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(AVChannelStartIndication& a, AVChannelStartIndication& b) {
    a.Swap(&b);
  }
  inline void Swap(AVChannelStartIndication* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(AVChannelStartIndication* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline AVChannelStartIndication* New() const final {
    return new AVChannelStartIndication();
  }

  AVChannelStartIndication* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<AVChannelStartIndication>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const AVChannelStartIndication& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const AVChannelStartIndication& from);
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
  void InternalSwap(AVChannelStartIndication* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "aasdk.proto.messages.AVChannelStartIndication";
  }
  protected:
  explicit AVChannelStartIndication(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kSessionFieldNumber = 1,
    kConfigFieldNumber = 2,
  };
  // required int32 session = 1;
  bool has_session() const;
  private:
  bool _internal_has_session() const;
  public:
  void clear_session();
  ::PROTOBUF_NAMESPACE_ID::int32 session() const;
  void set_session(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_session() const;
  void _internal_set_session(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // required uint32 config = 2;
  bool has_config() const;
  private:
  bool _internal_has_config() const;
  public:
  void clear_config();
  ::PROTOBUF_NAMESPACE_ID::uint32 config() const;
  void set_config(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_config() const;
  void _internal_set_config(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:aasdk.proto.messages.AVChannelStartIndication)
 private:
  class _Internal;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::int32 session_;
  ::PROTOBUF_NAMESPACE_ID::uint32 config_;
  friend struct ::TableStruct_AVChannelStartIndicationMessage_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// AVChannelStartIndication

// required int32 session = 1;
inline bool AVChannelStartIndication::_internal_has_session() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool AVChannelStartIndication::has_session() const {
  return _internal_has_session();
}
inline void AVChannelStartIndication::clear_session() {
  session_ = 0;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 AVChannelStartIndication::_internal_session() const {
  return session_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 AVChannelStartIndication::session() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.messages.AVChannelStartIndication.session)
  return _internal_session();
}
inline void AVChannelStartIndication::_internal_set_session(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000001u;
  session_ = value;
}
inline void AVChannelStartIndication::set_session(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_session(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.messages.AVChannelStartIndication.session)
}

// required uint32 config = 2;
inline bool AVChannelStartIndication::_internal_has_config() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool AVChannelStartIndication::has_config() const {
  return _internal_has_config();
}
inline void AVChannelStartIndication::clear_config() {
  config_ = 0u;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 AVChannelStartIndication::_internal_config() const {
  return config_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 AVChannelStartIndication::config() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.messages.AVChannelStartIndication.config)
  return _internal_config();
}
inline void AVChannelStartIndication::_internal_set_config(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _has_bits_[0] |= 0x00000002u;
  config_ = value;
}
inline void AVChannelStartIndication::set_config(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_config(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.messages.AVChannelStartIndication.config)
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
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_AVChannelStartIndicationMessage_2eproto
