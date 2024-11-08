// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: FuelLevelData.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_FuelLevelData_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_FuelLevelData_2eproto

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
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_FuelLevelData_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_FuelLevelData_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_FuelLevelData_2eproto;
namespace aasdk {
namespace proto {
namespace data {
class FuelLevel;
struct FuelLevelDefaultTypeInternal;
extern FuelLevelDefaultTypeInternal _FuelLevel_default_instance_;
}  // namespace data
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> ::aasdk::proto::data::FuelLevel* Arena::CreateMaybeMessage<::aasdk::proto::data::FuelLevel>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace aasdk {
namespace proto {
namespace data {

// ===================================================================

class FuelLevel final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:aasdk.proto.data.FuelLevel) */ {
 public:
  inline FuelLevel() : FuelLevel(nullptr) {}
  ~FuelLevel() override;
  explicit constexpr FuelLevel(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  FuelLevel(const FuelLevel& from);
  FuelLevel(FuelLevel&& from) noexcept
    : FuelLevel() {
    *this = ::std::move(from);
  }

  inline FuelLevel& operator=(const FuelLevel& from) {
    CopyFrom(from);
    return *this;
  }
  inline FuelLevel& operator=(FuelLevel&& from) noexcept {
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
  static const FuelLevel& default_instance() {
    return *internal_default_instance();
  }
  static inline const FuelLevel* internal_default_instance() {
    return reinterpret_cast<const FuelLevel*>(
               &_FuelLevel_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(FuelLevel& a, FuelLevel& b) {
    a.Swap(&b);
  }
  inline void Swap(FuelLevel* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(FuelLevel* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline FuelLevel* New() const final {
    return new FuelLevel();
  }

  FuelLevel* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<FuelLevel>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const FuelLevel& from);
  void MergeFrom(const FuelLevel& from);
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
  void InternalSwap(FuelLevel* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "aasdk.proto.data.FuelLevel";
  }
  protected:
  explicit FuelLevel(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kFuelLevelFieldNumber = 1,
    kRangeFieldNumber = 2,
    kLowFuelFieldNumber = 3,
  };
  // required int32 fuel_level = 1;
  bool has_fuel_level() const;
  private:
  bool _internal_has_fuel_level() const;
  public:
  void clear_fuel_level();
  ::PROTOBUF_NAMESPACE_ID::int32 fuel_level() const;
  void set_fuel_level(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_fuel_level() const;
  void _internal_set_fuel_level(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // required int32 range = 2;
  bool has_range() const;
  private:
  bool _internal_has_range() const;
  public:
  void clear_range();
  ::PROTOBUF_NAMESPACE_ID::int32 range() const;
  void set_range(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_range() const;
  void _internal_set_range(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // required bool low_fuel = 3;
  bool has_low_fuel() const;
  private:
  bool _internal_has_low_fuel() const;
  public:
  void clear_low_fuel();
  bool low_fuel() const;
  void set_low_fuel(bool value);
  private:
  bool _internal_low_fuel() const;
  void _internal_set_low_fuel(bool value);
  public:

  // @@protoc_insertion_point(class_scope:aasdk.proto.data.FuelLevel)
 private:
  class _Internal;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::int32 fuel_level_;
  ::PROTOBUF_NAMESPACE_ID::int32 range_;
  bool low_fuel_;
  friend struct ::TableStruct_FuelLevelData_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// FuelLevel

// required int32 fuel_level = 1;
inline bool FuelLevel::_internal_has_fuel_level() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool FuelLevel::has_fuel_level() const {
  return _internal_has_fuel_level();
}
inline void FuelLevel::clear_fuel_level() {
  fuel_level_ = 0;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 FuelLevel::_internal_fuel_level() const {
  return fuel_level_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 FuelLevel::fuel_level() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.FuelLevel.fuel_level)
  return _internal_fuel_level();
}
inline void FuelLevel::_internal_set_fuel_level(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000001u;
  fuel_level_ = value;
}
inline void FuelLevel::set_fuel_level(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_fuel_level(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.FuelLevel.fuel_level)
}

// required int32 range = 2;
inline bool FuelLevel::_internal_has_range() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool FuelLevel::has_range() const {
  return _internal_has_range();
}
inline void FuelLevel::clear_range() {
  range_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 FuelLevel::_internal_range() const {
  return range_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 FuelLevel::range() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.FuelLevel.range)
  return _internal_range();
}
inline void FuelLevel::_internal_set_range(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000002u;
  range_ = value;
}
inline void FuelLevel::set_range(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_range(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.FuelLevel.range)
}

// required bool low_fuel = 3;
inline bool FuelLevel::_internal_has_low_fuel() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool FuelLevel::has_low_fuel() const {
  return _internal_has_low_fuel();
}
inline void FuelLevel::clear_low_fuel() {
  low_fuel_ = false;
  _has_bits_[0] &= ~0x00000004u;
}
inline bool FuelLevel::_internal_low_fuel() const {
  return low_fuel_;
}
inline bool FuelLevel::low_fuel() const {
  // @@protoc_insertion_point(field_get:aasdk.proto.data.FuelLevel.low_fuel)
  return _internal_low_fuel();
}
inline void FuelLevel::_internal_set_low_fuel(bool value) {
  _has_bits_[0] |= 0x00000004u;
  low_fuel_ = value;
}
inline void FuelLevel::set_low_fuel(bool value) {
  _internal_set_low_fuel(value);
  // @@protoc_insertion_point(field_set:aasdk.proto.data.FuelLevel.low_fuel)
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
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_FuelLevelData_2eproto
