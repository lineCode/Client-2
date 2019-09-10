// group.h
//

#ifndef IDLIY0123972619821732198321CV1EPVI
#define IDLIY0123972619821732198321CV1EPVI

///// Includes /////

#include <QObject>

///// Namespaces /////

namespace client
{

///// Classes /////

class Group : public QObject
{
 Q_OBJECT

 public:

  Group(const uint64_t token, const QString& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);
  ~Group();
  
  inline uint64_t GetToken() const { return token_; }
  inline void SetName(const QString& name) { name_ = name; }
  inline const QString& GetName() const { return name_; }
  inline void SetManageUsers(const bool manageusers) { manageusers_ = manageusers; }
  inline bool GetManageUsers() const { return manageusers_; }
  inline void SetManageRecordings(const bool managerecordings) { managerecordings_ = managerecordings; }
  inline bool GetManageRecordings() const { return managerecordings_; }
  inline void SetManageMaps(const bool managemaps) { managemaps_ = managemaps; }
  inline bool GetManageMaps() const { return managemaps_; }
  inline void SetManageDevice(const bool managedevice) { managedevice_ = managedevice; }
  inline bool GetManageDevice() const { return managedevice_; }
  inline void SetAllRecordings(const bool allrecordings) { allrecordings_ = allrecordings; }
  inline bool GetAllRecordings() const { return allrecordings_; }
  inline void SetRecordings(const std::vector<uint64_t>& recordings) { recordings_ = recordings; }
  inline const std::vector<uint64_t>& GetRecordings() const { return recordings_; }

 private:

  uint64_t token_;
  QString name_;
  bool manageusers_;
  bool managerecordings_;
  bool managemaps_;
  bool managedevice_;
  bool allrecordings_;
  std::vector<uint64_t> recordings_;
 
};

}

#endif
