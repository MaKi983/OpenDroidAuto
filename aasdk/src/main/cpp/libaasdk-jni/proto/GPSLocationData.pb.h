// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: GPSLocationData.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_GPSLocationData_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_GPSLocationData_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_GPSLocationData_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_GPSLocationData_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_GPSLocationData_2eproto;
namespace aasdk {
namespace proto {
namespace data {
class GPSLocation;
struct GPSLocationDefaultTypeInternal;
extern GPSLocationDefaultTypeInternal _GPSLocation_default_instance_;
}  // namespace data
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> ::aasdk::proto::data::GPSLocation* Arena::CreateMaybeMessage<::aasdk::proto::data::GPSLocation>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace aasdk {
namespace proto {
namespace data {

// ===================================================================

class GPSLocation final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:aasdk.proto.data.GPSLocation) */ {
 public:
  inline GPSLocation() : GPSLocation(nullptr) {}
  ~GPSLocation() override;
  explicit constexpr GPSLocation(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  GPSLocation(const GPSLocation& from);
  GPSLocation(GPSLocation&& from) noexcept
    : GPSLocation() {
    *this = ::std::move(from);
  }

  inline GPSLocation& operator=(const GPSLocation& from) {
    CopyFrom(from);
    return *this;
  }
  inline GPSLocation& operator=(GPSLocation&& from) noexcept {
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
  static const GPSLocation& default_instance() {
    return *internal_default_instance();
  }
  static inline const GPSLocation* internal_default_instance() {
    return reinterpret_cast<const GPSLocation*>(
               &_GPSLocation_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(GPSLocation& a, GPSLocation& b) {
    a.Swap(&b);
  }
  inline void Swap(GPSLocation* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(GPSLocation* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline GPSLocation* New() const final {
    return new GPSLocation();
  }

  GPSLocation* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<GPSLocation>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const GPSLocation& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const GPSLocation& from);
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
  void InternalSwap(GPSLocation* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "aasdk.proto.data.GPSLocation";
  }
  protected:
  explicit GPSLocation(::PROTOBUF_NAMESPACE_ID::Arena* arena,
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
    kTimestampFieldNumber = 1,
    kLatitudeFieldNumber = 2,
    kLongitudeFieldNumber = 3,
    kAccuracyFieldNumber = 4,
    kAltitudeFieldNumber = 5,
    kSpeedFieldNumber = 6,
    kBearingFieldNumber = 7,
  };
  // required uint64 timestamp = 1;
  bool has_timestamp() const;
  private:
  bool _internal_has_timestamp() const;
  public:
  void clear_timestamp();
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp() const;
  void set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_timestamp() const;
  void _internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // required int32 latitude = 2;
  bool has_latitude() const;
  private:
  bool _internal_has_latitude() const;
  public:
  void clear_latitude();
  ::PROTOBUF_NAMESPACE_ID::int32 latitude() const;
  void set_latitude(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_latitude() const;
  void _internal_set_latitude(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // required int32 longitude = 3;
  bool has_longitude() const;
  private:
  bool _internal_has_longitude() const;
  public:
  void clear_longitude();
  ::PROTOBUF_NAMESPACE_ID::int32 longitude() const;
  void set_longitude(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_longitude() const;
  void _internal_set_longitude(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // required uint32 accuracy = 4;
  bool has_accuracy() const;
  private:
  bool _internal_has_accuracy() const;
  public:
  void clear_accuracy();
  ::PROTOBUF_NAMESPACE_ID::uint32 accuracy() const;
  void set_accuracy(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_accuracy() const;
  void _internal_set_accuracy(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // optional int32 altitude = 5;
  bool has_altitude() const;
  private:
  bool _internal_has_altitude() const;
  public:
  void clear_altitude();
  ::PROTOBUF_NAMESPACE_ID::int32 altitude() const;
  void set_altitude(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_altitude() const;
  void _internal_set_altitude(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // optional int32 speed = 6;
  bool has_speed() const;
  private:
  bool _internal_has_speed() const;
  public:
  void clear_speed();
  ::PROTOBUF_NAMESPACE_ID::int32 speed() const;
  void set_speed(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_speed() const;
  void _internal_set_speed(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // optional int32 bearing = 7;
  bool has_bearing() const;
  private:
  bool _internal_has_bearing() const;
  public:
  void clear_bearing();
  ::PROTOBUF_NAMESPACE_ID::int32 bearing() const;
  void set_bearing(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_bearing() const;
  void _internal_set_bearing(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:aasdk.proto.data.GPSLocation)
 private:
  class _Internal;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp_;
  ::PROTOBUF_NAMESPACE_ID::int32 latitude_;
  ::PROTOBUF_NAMESPACE_ID::int32 longitude_;
  ::PROTOBUF_NAMESPACE_ID::uint32 accuracy_;
  ::PROTOBUF_NAMESPACE_ID::int32 altitude_;
  ::PROTOBUF_NAMESPACE_ID::int32 speed_;
  ::PROTOBUF_NAMESPACE_ID::int32 bearing_;
  friend struct ::TableStruct_GPSLocationData_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// GPSLocation

// required uint64 timestamp = 1;
inline bool GPSLocation::_internal_has_timestamp() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool GPSLocation::has_timestamp() const {
  return _internal_has_timestamp();
}
inline void GPSLocation::clear_timestamp() {
  timestamp_ = uint64_t{0u};
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 GPSLocation::_internal_timestamp() const {
  return timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 GPSLocation::timestamp() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.GPSLocation.timestamp)
  return _internal_timestamp();
}
inline void GPSLocation::_internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _has_bits_[0] |= 0x00000001u;
  timestamp_ = value;
}
inline void GPSLocation::set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_timestamp(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.GPSLocation.timestamp)
}

// required int32 latitude = 2;
inline bool GPSLocation::_internal_has_latitude() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool GPSLocation::has_latitude() const {
  return _internal_has_latitude();
}
inline void GPSLocation::clear_latitude() {
  latitude_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GPSLocation::_internal_latitude() const {
  return latitude_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GPSLocation::latitude() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.GPSLocation.latitude)
  return _internal_latitude();
}
inline void GPSLocation::_internal_set_latitude(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000002u;
  latitude_ = value;
}
inline void GPSLocation::set_latitude(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_latitude(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.GPSLocation.latitude)
}

// required int32 longitude = 3;
inline bool GPSLocation::_internal_has_longitude() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool GPSLocation::has_longitude() const {
  return _internal_has_longitude();
}
inline void GPSLocation::clear_longitude() {
  longitude_ = 0;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GPSLocation::_internal_longitude() const {
  return longitude_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GPSLocation::longitude() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.GPSLocation.longitude)
  return _internal_longitude();
}
inline void GPSLocation::_internal_set_longitude(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000004u;
  longitude_ = value;
}
inline void GPSLocation::set_longitude(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_longitude(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.GPSLocation.longitude)
}

// required uint32 accuracy = 4;
inline bool GPSLocation::_internal_has_accuracy() const {
  bool value = (_has_bits_[0] & 0x00000008u) != 0;
  return value;
}
inline bool GPSLocation::has_accuracy() const {
  return _internal_has_accuracy();
}
inline void GPSLocation::clear_accuracy() {
  accuracy_ = 0u;
  _has_bits_[0] &= ~0x00000008u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 GPSLocation::_internal_accuracy() const {
  return accuracy_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 GPSLocation::accuracy() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.GPSLocation.accuracy)
  return _internal_accuracy();
}
inline void GPSLocation::_internal_set_accuracy(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _has_bits_[0] |= 0x00000008u;
  accuracy_ = value;
}
inline void GPSLocation::set_accuracy(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_accuracy(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.GPSLocation.accuracy)
}

// optional int32 altitude = 5;
inline bool GPSLocation::_internal_has_altitude() const {
  bool value = (_has_bits_[0] & 0x00000010u) != 0;
  return value;
}
inline bool GPSLocation::has_altitude() const {
  return _internal_has_altitude();
}
inline void GPSLocation::clear_altitude() {
  altitude_ = 0;
  _has_bits_[0] &= ~0x00000010u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GPSLocation::_internal_altitude() const {
  return altitude_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GPSLocation::altitude() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.GPSLocation.altitude)
  return _internal_altitude();
}
inline void GPSLocation::_internal_set_altitude(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000010u;
  altitude_ = value;
}
inline void GPSLocation::set_altitude(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_altitude(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.GPSLocation.altitude)
}

// optional int32 speed = 6;
inline bool GPSLocation::_internal_has_speed() const {
  bool value = (_has_bits_[0] & 0x00000020u) != 0;
  return value;
}
inline bool GPSLocation::has_speed() const {
  return _internal_has_speed();
}
inline void GPSLocation::clear_speed() {
  speed_ = 0;
  _has_bits_[0] &= ~0x00000020u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GPSLocation::_internal_speed() const {
  return speed_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GPSLocation::speed() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.GPSLocation.speed)
  return _internal_speed();
}
inline void GPSLocation::_internal_set_speed(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000020u;
  speed_ = value;
}
inline void GPSLocation::set_speed(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_speed(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.GPSLocation.speed)
}

// optional int32 bearing = 7;
inline bool GPSLocation::_internal_has_bearing() const {
  bool value = (_has_bits_[0] & 0x00000040u) != 0;
  return value;
}
inline bool GPSLocation::has_bearing() const {
  return _internal_has_bearing();
}
inline void GPSLocation::clear_bearing() {
  bearing_ = 0;
  _has_bits_[0] &= ~0x00000040u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GPSLocation::_internal_bearing() const {
  return bearing_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GPSLocation::bearing() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.GPSLocation.bearing)
  return _internal_bearing();
}
inline void GPSLocation::_internal_set_bearing(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000040u;
  bearing_ = value;
}
inline void GPSLocation::set_bearing(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_bearing(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.GPSLocation.bearing)
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
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_GPSLocationData_2eproto
