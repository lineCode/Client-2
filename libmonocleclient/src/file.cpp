// file.cpp
//

///// Includes /////

#include "monocleclient/file.h"

#include <utility/utility.hpp>

///// Namespaces /////

namespace client
{

///// Methods /////

File::File(const uint64_t token, const QString& path, const QString& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate) :
  token_(token),
  path_(path),
  mountpoint_(mountpoint),
  numchunks_(numchunks),
  chunksize_(chunksize),
  automount_(automount),
  state_(state),
  monitorstate_(monitorstate)
{
  // We could check here whether mountpoint and path are related and provide a warning message if they are not, but it is difficult to do in a cross platform way

}

File::~File()
{

}

void File::StateChanged(const monocle::FileState state)
{
  state_ = state;

}

void File::MonitorStateChanged(const monocle::FileMonitorState monitorstate)
{
  monitorstate_ = monitorstate;

}

}
