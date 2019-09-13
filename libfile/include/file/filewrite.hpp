// Filewrite.hpp
//

#ifndef IDF69P3OA231ASDADASADSADSXT0STS5UA
#define IDF69P3OA231ASDADASADSADSXT0STS5UA

///// Includes /////

#include <boost/container/flat_set.hpp>
#include <boost/filesystem/path.hpp>
#include <flatbuffers/flatbuffers.h>
#include <map>
#include <vector>

#ifdef _WIN32
  #include <windows.h>
#endif

#include "file.hpp"

///// Namespaces /////

namespace file
{

///// Classes /////

// Init and then Open the file, then WriteFrame all of the frames, then Close and Destroy
// When calling Close(), populate the FILE with everything except the frame headers
class FileWrite
{
 public:

  FileWrite();
  ~FileWrite();

  int Init(const boost::filesystem::path& path);
  void Destroy();

  int Open();
  int Close(const FILE& file);

  int WriteH265Frame(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t codecindex, const uint8_t* data, const uint64_t size, const bool marker, const uint64_t time, const std::vector<unsigned char>& signature, const bool donlfield, const std::vector<uint32_t>& offsets);
  int WriteH264Frame(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t codecindex, const uint8_t* data, const uint64_t size, const bool marker, const uint64_t time, const std::vector<unsigned char>& signature, const std::vector<uint32_t>& offsets);
  int WriteJPEGFrame(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t codecindex, const uint8_t* data, const uint64_t size, const uint64_t time, const std::vector<unsigned char>& signature, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const std::array<uint8_t, 64>& lqt, const std::array<uint8_t, 64>& cqt);
  int WriteMetadataFrame(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t codecindex, const uint8_t* data, const uint64_t size, const uint64_t time, const std::vector<unsigned char>& signature);
  int WriteMPEG4Frame(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t codecindex, const uint8_t* data, const uint64_t size, const bool marker, const uint64_t time, const std::vector<unsigned char>& signature);

 private:

  int WriteFrame(const uint8_t* data, const uint64_t size);

  struct FRAMEINDEX
  {
    FRAMEINDEX(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex);

    bool operator<(const FRAMEINDEX& rhs) const;

    uint64_t deviceindex_;
    uint64_t recordingindex_;
    uint64_t trackindex_;
  };

#ifdef _WIN32
  HANDLE file_;
#else
  int file_;
#endif

  uint64_t currentoffset_;

  // <recordingindex, frames>
  std::map<FRAMEINDEX, std::vector< std::unique_ptr<H265FRAMEHEADER> > > h265frameheaders_;
  std::map<FRAMEINDEX, std::vector< std::unique_ptr<H264FRAMEHEADER> > > h264frameheaders_;
  std::map<FRAMEINDEX, std::vector< std::unique_ptr<JPEGFRAMEHEADER> > > jpegframeheaders_;
  std::map<FRAMEINDEX, std::vector< std::unique_ptr<METADATAFRAMEHEADER> > > metadataframeheaders_;
  std::map<FRAMEINDEX, std::vector< std::unique_ptr<MPEG4FRAMEHEADER> > > mpeg4frameheaders_;

  flatbuffers::FlatBufferBuilder fbb_;

};

}

#endif
