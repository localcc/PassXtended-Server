// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FILES_PASSXTENDED_PROTOS_H_
#define FLATBUFFERS_GENERATED_FILES_PASSXTENDED_PROTOS_H_

#include "flatbuffers/flatbuffers.h"

#include "file_generated.h"

namespace passxtended {
namespace protos {

struct files;

struct files FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ALL_FILES = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<file>> *all_files() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<file>> *>(VT_ALL_FILES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ALL_FILES) &&
           verifier.VerifyVector(all_files()) &&
           verifier.VerifyVectorOfTables(all_files()) &&
           verifier.EndTable();
  }
};

struct filesBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_all_files(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<file>>> all_files) {
    fbb_.AddOffset(files::VT_ALL_FILES, all_files);
  }
  explicit filesBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  filesBuilder &operator=(const filesBuilder &);
  flatbuffers::Offset<files> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<files>(end);
    return o;
  }
};

inline flatbuffers::Offset<files> Createfiles(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<file>>> all_files = 0) {
  filesBuilder builder_(_fbb);
  builder_.add_all_files(all_files);
  return builder_.Finish();
}

inline flatbuffers::Offset<files> CreatefilesDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<file>> *all_files = nullptr) {
  auto all_files__ = all_files ? _fbb.CreateVector<flatbuffers::Offset<file>>(*all_files) : 0;
  return passxtended::protos::Createfiles(
      _fbb,
      all_files__);
}

inline const passxtended::protos::files *Getfiles(const void *buf) {
  return flatbuffers::GetRoot<passxtended::protos::files>(buf);
}

inline const passxtended::protos::files *GetSizePrefixedfiles(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<passxtended::protos::files>(buf);
}

inline bool VerifyfilesBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<passxtended::protos::files>(nullptr);
}

inline bool VerifySizePrefixedfilesBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<passxtended::protos::files>(nullptr);
}

inline void FinishfilesBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<passxtended::protos::files> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedfilesBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<passxtended::protos::files> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace protos
}  // namespace passxtended

#endif  // FLATBUFFERS_GENERATED_FILES_PASSXTENDED_PROTOS_H_