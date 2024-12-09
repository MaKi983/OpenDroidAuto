// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AccelData.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_AccelData_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_AccelData_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_AccelData_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_AccelData_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_AccelData_2eproto;
namespace aasdk {
namespace proto {
namespace data {
class Accel;
struct AccelDefaultTypeInternal;
extern AccelDefaultTypeInternal _Accel_default_instance_;
}  // namespace data
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> ::aasdk::proto::data::Accel* Arena::CreateMaybeMessage<::aasdk::proto::data::Accel>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace aasdk {
namespace proto {
namespace data {

// ===================================================================

class Accel final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:aasdk.proto.data.Accel) */ {
 public:
  inline Accel() : Accel(nullptr) {}
  ~Accel() override;
  explicit constexpr Accel(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Accel(const Accel& from);
  Accel(Accel&& from) noexcept
    : Accel() {
    *this = ::std::move(from);
  }

  inline Accel& operator=(const Accel& from) {
    CopyFrom(from);
    return *this;
  }
  inline Accel& operator=(Accel&& from) noexcept {
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
  static const Accel& default_instance() {
    return *internal_default_instance();
  }
  static inline const Accel* internal_default_instance() {
    return reinterpret_cast<const Accel*>(
               &_Accel_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Accel& a, Accel& b) {
    a.Swap(&b);
  }
  inline void Swap(Accel* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Accel* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Accel* New() const final {
    return new Accel();
  }

  Accel* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Accel>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Accel& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const Accel& from);
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
  void InternalSwap(Accel* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "aasdk.proto.data.Accel";
  }
  protected:
  explicit Accel(::PROTOBUF_NAMESPACE_ID::Arena* arena,
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
    kAccelerationXFieldNumber = 1,
    kAccelerationYFieldNumber = 2,
    kAccelerationZFieldNumber = 3,
  };
  // required int32 acceleration_x = 1;
  bool has_acceleration_x() const;
  private:
  bool _internal_has_acceleration_x() const;
  public:
  void clear_acceleration_x();
  ::PROTOBUF_NAMESPACE_ID::int32 acceleration_x() const;
  void set_acceleration_x(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_acceleration_x() const;
  void _internal_set_acceleration_x(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // required int32 acceleration_y = 2;
  bool has_acceleration_y() const;
  private:
  bool _internal_has_acceleration_y() const;
  public:
  void clear_acceleration_y();
  ::PROTOBUF_NAMESPACE_ID::int32 acceleration_y() const;
  void set_acceleration_y(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_acceleration_y() const;
  void _internal_set_acceleration_y(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // required int32 acceleration_z = 3;
  bool has_acceleration_z() const;
  private:
  bool _internal_has_acceleration_z() const;
  public:
  void clear_acceleration_z();
  ::PROTOBUF_NAMESPACE_ID::int32 acceleration_z() const;
  void set_acceleration_z(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_acceleration_z() const;
  void _internal_set_acceleration_z(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:aasdk.proto.data.Accel)
 private:
  class _Internal;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::int32 acceleration_x_;
  ::PROTOBUF_NAMESPACE_ID::int32 acceleration_y_;
  ::PROTOBUF_NAMESPACE_ID::int32 acceleration_z_;
  friend struct ::TableStruct_AccelData_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Accel

// required int32 acceleration_x = 1;
inline bool Accel::_internal_has_acceleration_x() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool Accel::has_acceleration_x() const {
  return _internal_has_acceleration_x();
}
inline void Accel::clear_acceleration_x() {
  acceleration_x_ = 0;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Accel::_internal_acceleration_x() const {
  return acceleration_x_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Accel::acceleration_x() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.Accel.acceleration_x)
  return _internal_acceleration_x();
}
inline void Accel::_internal_set_acceleration_x(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000001u;
  acceleration_x_ = value;
}
inline void Accel::set_acceleration_x(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_acceleration_x(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.Accel.acceleration_x)
}

// required int32 acceleration_y = 2;
inline bool Accel::_internal_has_acceleration_y() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool Accel::has_acceleration_y() const {
  return _internal_has_acceleration_y();
}
inline void Accel::clear_acceleration_y() {
  acceleration_y_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Accel::_internal_acceleration_y() const {
  return acceleration_y_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Accel::acceleration_y() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.Accel.acceleration_y)
  return _internal_acceleration_y();
}
inline void Accel::_internal_set_acceleration_y(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000002u;
  acceleration_y_ = value;
}
inline void Accel::set_acceleration_y(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_acceleration_y(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.Accel.acceleration_y)
}

// required int32 acceleration_z = 3;
inline bool Accel::_internal_has_acceleration_z() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool Accel::has_acceleration_z() const {
  return _internal_has_acceleration_z();
}
inline void Accel::clear_acceleration_z() {
  acceleration_z_ = 0;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Accel::_internal_acceleration_z() const {
  return acceleration_z_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Accel::acceleration_z() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.Accel.acceleration_z)
  return _internal_acceleration_z();
}
inline void Accel::_internal_set_acceleration_z(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000004u;
  acceleration_z_ = value;
}
inline void Accel::set_acceleration_z(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_acceleration_z(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.Accel.acceleration_z)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace data
}  // namespace proto
}  // namespace aasdk

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_AccelData_2eproto
