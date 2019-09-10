// checkforupdate.cpp
//

///// Includes /////

#include "monocleclient/checkforupdate.h"

#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Globals /////

const QString domain(QString("http://updates.monoclesecurity.com/client/"));

///// Methods /////

CheckForUpdate::CheckForUpdate(const utility::Version& version) :
  version_(version),
  manager_(new QNetworkAccessManager(this))
{
  connect(manager_, &QNetworkAccessManager::finished, this, &CheckForUpdate::NetworkReply);

}

CheckForUpdate::~CheckForUpdate()
{

}

void CheckForUpdate::Check()
{
  manager_->get(QNetworkRequest(QUrl(domain + QString("versions.xml"))));

}

void CheckForUpdate::NetworkReply(QNetworkReply* networkreply)
{
  networkreply->deleteLater();

  // Error checking
  if ((networkreply == nullptr) || networkreply->error())
  {
    emit UpdateAvailable(false, std::vector<UpdateVersion>(), UpdateVersion());
    LOG_WARNING(tr("Failed to check for update"));
    return;
  }

  pugi::xml_document document;
  if (!document.load(networkreply->readAll().toStdString().c_str()))
  {
    emit UpdateAvailable(false, std::vector<UpdateVersion>(), UpdateVersion());
    LOG_WARNING(tr("Invalid update xml"));
    return;
  }

  const pugi::xml_node versions = document.child("versions");
  if (!versions)
  {
    emit UpdateAvailable(false, std::vector<UpdateVersion>(), UpdateVersion());
    LOG_WARNING(tr("Invalid update xml"));
    return;
  }

  // Figure out all the versions
  std::vector<UpdateVersion> updateversions;
  for (const pugi::xml_node& version : versions.children("version"))
  {
    const pugi::xml_node majornode = version.child("major");
    const pugi::xml_node minornode = version.child("minor");
    const pugi::xml_node buildnode = version.child("build");
    const pugi::xml_node datenode = version.child("date");
#ifdef _WIN32
    const pugi::xml_node filenamenode = version.child("windowsfilename");
#endif
    const pugi::xml_node changesnode = version.child("changes");

    if (!majornode || !minornode || !buildnode || !datenode || !changesnode)
    {
      emit UpdateAvailable(false, std::vector<UpdateVersion>(), UpdateVersion());
      LOG_WARNING(tr("Invalid update version"));
      continue;
    }
#ifdef _WIN32
    if (!filenamenode)
    {
      emit UpdateAvailable(false, std::vector<UpdateVersion>(), UpdateVersion());
      LOG_WARNING(tr("Invalid update version"));
      continue;
    }
#endif

    try
    {
      UpdateVersion updateversion(utility::Version(boost::lexical_cast<unsigned int>(majornode.text().get()), boost::lexical_cast<unsigned int>(minornode.text().get()), boost::lexical_cast<unsigned int>(buildnode.text().get())));
      updateversion.date_ = QDate::fromString(QString::fromStdString(datenode.text().get()));
      if (!updateversion.date_.isValid())
      {
        LOG_WARNING(tr("Invalid release date"));

      }

      for (pugi::xml_node changenode : changesnode.children("change"))
      {
        if (!changenode)
        {
          LOG_WARNING(tr("Empty changelist change"));
          continue;
        }
#ifdef _WIN32
        updateversion.url_ = QUrl(domain + QString("windows/") + QString(filenamenode.text().get()));
#endif
        updateversion.changelist_.push_back(changenode.text().get());
      }
      
      updateversions.push_back(updateversion);
    }
    catch (...)
    {
      
      continue;
    }
  }

  if (updateversions.empty())
  {
    emit UpdateAvailable(false, std::vector<UpdateVersion>(), UpdateVersion());
    LOG_WARNING(tr("No versions found"));
    return;
  }

  std::sort(updateversions.begin(), updateversions.end(), [](const UpdateVersion& lhs, const UpdateVersion& rhs){ return lhs.version_ > rhs.version_; });

  // Determine if there is a later version available
  if (updateversions.front().version_ > version_)
  {
    emit UpdateAvailable(true, updateversions, updateversions.front());

  }
  else
  {
    emit UpdateAvailable(false, std::vector<UpdateVersion>(), UpdateVersion());

  }
}

}
