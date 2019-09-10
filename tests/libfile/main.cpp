// main.cpp
//

///// Includes /////

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <stdint.h>
#include <string>
#include <utility/utility.hpp>
#include <vector>

#include "file/fileread.hpp"
#include "file/filewrite.hpp"

///// Functions /////

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

template<class T>
std::vector<T> Generate()
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<int> valuedist(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
  static std::uniform_int_distribution<size_t> sizedist(0, 512);

  const size_t size = sizedist(gen);
  std::vector<T> result;
  result.reserve(size);
  for (size_t i = 0; i < size; ++i)
  {
    result.push_back(static_cast<T>(valuedist(gen)));

  }
  return result;
}

TEST(TestFile, Frames)
{
  const std::string filename("test.dat");

  boost::system::error_code err;
  boost::filesystem::remove(filename, err);

  std::random_device rd;
  std::mt19937 gen(rd());

  const size_t BUFFER_SIZE = 10 * 1024 * 1024;
  std::uniform_int_distribution<int> bufferdist(std::numeric_limits<char>::min(), std::numeric_limits<char>::max());
  std::vector<char> randombuffer;
  randombuffer.reserve(BUFFER_SIZE);
  for (size_t i = 0; i < BUFFER_SIZE; ++i)
  {
    randombuffer.push_back(static_cast<char>(bufferdist(gen)));

  }

  // Write it
  file::FILE file;
  {
    file::FileWrite filewrite;
    ASSERT_EQ(0, filewrite.Init(filename));
    ASSERT_EQ(0, filewrite.Open());

    std::uniform_int_distribution<uint64_t> timedist(0, 1000 * 1000 * 1000);
    const size_t MAX_DATA_SIZE = 128;
    std::uniform_int_distribution<size_t> datadist(0, BUFFER_SIZE - MAX_DATA_SIZE);
    std::uniform_int_distribution<size_t> datasizedist(1, MAX_DATA_SIZE);
    std::uniform_int_distribution<int> markerdist(0, 1);

    const uint64_t numdevices = std::uniform_int_distribution<uint64_t>(1, 10)(gen);
    file.devices_.reserve(numdevices);
    for (uint64_t device = 0; device < numdevices; ++device)
    {
      const uint64_t numrecordings = std::uniform_int_distribution<uint64_t>(1, 10)(gen);
      boost::container::flat_set<file::RECORDING> recordings;
      recordings.reserve(numrecordings);
      for (uint64_t recording = 0; recording < numrecordings; ++recording)
      {
        const uint64_t numtracks = std::uniform_int_distribution<uint64_t>(1, 10)(gen);
        boost::container::flat_set<file::TRACK> tracks;
        tracks.reserve(numtracks);
        for (uint64_t track = 0; track < numtracks; ++track)
        {
          const uint64_t numcodecs = std::uniform_int_distribution<uint64_t>(1, 10)(gen);
          boost::container::flat_set<file::CODEC> codecs;
          codecs.reserve(numcodecs);
          std::vector< std::shared_ptr<file::FRAMEHEADER> > frameheaders;
          for (uint64_t codec = 0; codec < numcodecs; ++codec)
          {
            codecs.insert(file::CODEC(codec, static_cast<int>(codec), std::to_string(codec)));
            const uint64_t numframes = std::uniform_int_distribution<uint64_t>(0, 50)(gen);
            for (uint64_t frame = 0; frame < numframes; ++frame)
            {
              const uint64_t time = timedist(gen);
              const uint64_t offset = datadist(gen);
              const uint64_t size = datasizedist(gen);
              const std::vector<uint8_t> signature = Generate<uint8_t>();
              const bool marker = static_cast<bool>(markerdist(gen));
              const std::vector<uint32_t> offsets = utility::GenerateRandomData<uint32_t>(10);
              ASSERT_EQ(0, filewrite.WriteH264Frame(device, recording, track, codec, randombuffer.data() + offset, size, marker, time, signature, offsets));
              frameheaders.emplace_back(std::make_shared<file::H264FRAMEHEADER>(codec, offset, size, marker, time, signature, offsets));
            }
          }
          std::sort(frameheaders.begin(), frameheaders.end(), [](const std::shared_ptr<file::FRAMEHEADER>& lhs, const std::shared_ptr<file::FRAMEHEADER>& rhs) { return (lhs->time_ < rhs->time_); });
          tracks.insert(file::TRACK(track, std::to_string(track), codecs, frameheaders));
        }
        recordings.insert(file::RECORDING(recording, std::to_string(recording), std::to_string(recording), tracks));
      }
      file.devices_.insert(file::DEVICE(device, "name", "address", "signingkey", recordings));
    }
    ASSERT_EQ(0, filewrite.Close(file));
  }

  // Read it back and check it is equal
  {
    file::FileRead fileread;
    std::shared_ptr<float> progress = std::make_shared<float>(0.0f);
    ASSERT_EQ(0, fileread.Init(filename, progress).get());
    ASSERT_EQ(file.devices_.size(), fileread.GetFile().devices_.size());
    auto i = file.devices_.cbegin();
    auto j = fileread.GetFile().devices_.cbegin();
    for (; i != file.devices_.cend(); ++i, ++j)
    {
      ASSERT_EQ(i->recordings_.size(), j->recordings_.size());
      auto k = i->recordings_.cbegin();
      auto l = j->recordings_.cbegin();
      for (; k != i->recordings_.cend(); ++k, ++l)
      {
        ASSERT_EQ(k->tracks_.size(), l->tracks_.size());
        auto m = k->tracks_.cbegin();
        auto n = l->tracks_.cbegin();
        for (; m != k->tracks_.cend(); ++m, ++n)
        {
          ASSERT_EQ(m->frameheaders_.size(), n->frameheaders_.size());
          auto o = m->frameheaders_.cbegin();
          auto p = n->frameheaders_.cbegin();
          for (; o != m->frameheaders_.cend(); ++o, ++p)
          {
            std::vector<char> buffer;
            buffer.resize((*o)->size_);
            ASSERT_EQ(0, fileread.Read((*p)->offset_, (*p)->size_, buffer.data()));
            ASSERT_EQ(0, memcmp(randombuffer.data() + (*o)->offset_, buffer.data(), (*o)->size_));
            ASSERT_EQ((*o)->time_, (*p)->time_);
          }
        }
      }
    }
  }
}
