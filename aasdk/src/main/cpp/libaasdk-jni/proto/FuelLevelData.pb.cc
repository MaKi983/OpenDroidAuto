// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: FuelLevelData.proto

#include "FuelLevelData.pb.h"

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
constexpr FuelLevel::FuelLevel(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : fuel_level_(0)
  , range_(0)
  , low_fuel_(false){}
struct FuelLevelDefaultTypeInternal {
  constexpr FuelLevelDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~FuelLevelDefaultTypeInternal() {}
  union {
    FuelLevel _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT FuelLevelDefaultTypeInternal _FuelLevel_default_instance_;
}  // namespace data
}  // namespace proto
}  // namespace aasdk
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_FuelLevelData_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_FuelLevelData_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_FuelLevelData_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_FuelLevelData_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::FuelLevel, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::FuelLevel, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::FuelLevel, fuel_level_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::FuelLevel, range_),
  PROTOBUF_FIELD_OFFSET(::aasdk::proto::data::FuelLevel, low_fuel_),
  0,
  1,
  2,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 8, sizeof(::aasdk::proto::data::FuelLevel)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::aasdk::proto::data::_FuelLevel_default_instance_),
};

const char descriptor_table_protodef_FuelLevelData_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\023FuelLevelData.proto\022\020aasdk.proto.data\""
  "@\n\tFuelLevel\022\022\n\nfuel_level\030\001 \002(\005\022\r\n\005rang"
  "e\030\002 \002(\005\022\020\n\010low_fuel\030\003 \002(\010"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_FuelLevelData_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_FuelLevelData_2eproto = {
  false, false, 105, descriptor_table_protodef_FuelLevelData_2eproto, "FuelLevelData.proto", 
  &descriptor_table_FuelLevelData_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_FuelLevelData_2eproto::offsets,
  file_level_metadata_FuelLevelData_2eproto, file_level_enum_descriptors_FuelLevelData_2eproto, file_level_service_descriptors_FuelLevelData_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_FuelLevelData_2eproto_getter() {
  return &descriptor_table_FuelLevelData_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_FuelLevelData_2eproto(&descriptor_table_FuelLevelData_2eproto);
namespace aasdk {
namespace proto {
namespace data {

// ===================================================================

class FuelLevel::_Internal {
 public:
  using HasBits = decltype(std::declval<FuelLevel>()._has_bits_);
  static void set_has_fuel_level(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_range(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_low_fuel(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000007) ^ 0x00000007) != 0;
  }
};

FuelLevel::FuelLevel(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:aasdk.proto.data.FuelLevel)
}
FuelLevel::FuelLevel(const FuelLevel& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&fuel_level_, &from.fuel_level_,
    static_cast<size_t>(reinterpret_cast<char*>(&low_fuel_) -
    reinterpret_cast<char*>(&fuel_level_)) + sizeof(low_fuel_));
  // @@protoc_insertion_point(copy_constructor:aasdk.proto.data.FuelLevel)
}

void FuelLevel::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&fuel_level_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&low_fuel_) -
    reinterpret_cast<char*>(&fuel_level_)) + sizeof(low_fuel_));
}

FuelLevel::~FuelLevel() {
  // @@protoc_insertion_point(destructor:aasdk.proto.data.FuelLevel)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void FuelLevel::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void FuelLevel::ArenaDtor(void* object) {
  FuelLevel* _this = reinterpret_cast< FuelLevel* >(object);
  (void)_this;
}
void FuelLevel::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void FuelLevel::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void FuelLevel::Clear() {
// @@protoc_insertion_point(message_clear_start:aasdk.proto.data.FuelLevel)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    ::memset(&fuel_level_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&low_fuel_) -
        reinterpret_cast<char*>(&fuel_level_)) + sizeof(low_fuel_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* FuelLevel::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // required int32 fuel_level = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_fuel_level(&has_bits);
          fuel_level_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required int32 range = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_range(&has_bits);
          range_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required bool low_fuel = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          _Internal::set_has_low_fuel(&has_bits);
          low_fuel_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
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

::PROTOBUF_NAMESPACE_ID::uint8* FuelLevel::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:aasdk.proto.data.FuelLevel)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required int32 fuel_level = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_fuel_level(), target);
  }

  // required int32 range = 2;
  if (cached_has_bits & 0x00000002u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_range(), target);
  }

  // required bool low_fuel = 3;
  if (cached_has_bits & 0x00000004u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(3, this->_internal_low_fuel(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:aasdk.proto.data.FuelLevel)
  return target;
}

size_t FuelLevel::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:aasdk.proto.data.FuelLevel)
  size_t total_size = 0;

  if (_internal_has_fuel_level()) {
    // required int32 fuel_level = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_fuel_level());
  }

  if (_internal_has_range()) {
    // required int32 range = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_range());
  }

  if (_internal_has_low_fuel()) {
    // required bool low_fuel = 3;
    total_size += 1 + 1;
  }

  return total_size;
}
size_t FuelLevel::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:aasdk.proto.data.FuelLevel)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x00000007) ^ 0x00000007) == 0) {  // All required fields are present.
    // required int32 fuel_level = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_fuel_level());

    // required int32 range = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_range());

    // required bool low_fuel = 3;
    total_size += 1 + 1;

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

void FuelLevel::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:aasdk.proto.data.FuelLevel)
  GOOGLE_DCHECK_NE(&from, this);
  const FuelLevel* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<FuelLevel>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:aasdk.proto.data.FuelLevel)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:aasdk.proto.data.FuelLevel)
    MergeFrom(*source);
  }
}

void FuelLevel::MergeFrom(const FuelLevel& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:aasdk.proto.data.FuelLevel)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      fuel_level_ = from.fuel_level_;
    }
    if (cached_has_bits & 0x00000002u) {
      range_ = from.range_;
    }
    if (cached_has_bits & 0x00000004u) {
      low_fuel_ = from.low_fuel_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void FuelLevel::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:aasdk.proto.data.FuelLevel)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void FuelLevel::CopyFrom(const FuelLevel& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:aasdk.proto.data.FuelLevel)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FuelLevel::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_has_bits_)) return false;
  return true;
}

void FuelLevel::InternalSwap(FuelLevel* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(FuelLevel, low_fuel_)
      + sizeof(FuelLevel::low_fuel_)
      - PROTOBUF_FIELD_OFFSET(FuelLevel, fuel_level_)>(
          reinterpret_cast<char*>(&fuel_level_),
          reinterpret_cast<char*>(&other->fuel_level_));
}

::PROTOBUF_NAMESPACE_ID::Metadata FuelLevel::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_FuelLevelData_2eproto_getter, &descriptor_table_FuelLevelData_2eproto_once,
      file_level_metadata_FuelLevelData_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace data
}  // namespace proto
}  // namespace aasdk
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::aasdk::proto::data::FuelLevel* Arena::CreateMaybeMessage< ::aasdk::proto::data::FuelLevel >(Arena* arena) {
  return Arena::CreateMessageInternal< ::aasdk::proto::data::FuelLevel >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
