//===========================================
//  PC-BSD source code
//  Copyright (c) 2015, PC-BSD Software/iXsystems
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "sysadm-general.h"
#include "sysadm-iocage.h"
#include "sysadm-global.h"

using namespace sysadm;

//PLEASE: Keep the functions in the same order as listed in pcbsd-general.h
// Return all the default iocage settings
QJsonObject Iocage::getDefaultSettings() {
  QJsonObject retObject;

  QStringList output = General::RunCommand("iocage defaults").split("\n");

  QJsonObject vals;
  for ( int i = 0; i < output.size(); i++)
  {
    if ( output.at(i).indexOf("JID") != -1 )
      continue;

    if ( output.at(i).isEmpty() )
      break;

    QString key = output.at(i).simplified().section("=", 0, 0);
    QString value = output.at(i).simplified().section("=", 1, 1);

    vals.insert(key, value);
  }

  retObject.insert("defaults", vals);
  return retObject;
}

// List the jails on the box
QJsonObject Iocage::getJailSettings(QJsonObject jsin) {
  QJsonObject retObject;

  QStringList keys = jsin.keys();
  if (! keys.contains("jail") ) {
    retObject.insert("error", "Missing required keys");
    return retObject;
  }

  // Get the key values
  QString jail = jsin.value("jail").toString();

  QStringList output = General::RunCommand("iocage get all " + jail).split("\n");

  QJsonObject vals;
  for ( int i = 0; i < output.size(); i++)
  {
    if ( output.at(i).indexOf("JID") != -1 )
      continue;

    if ( output.at(i).isEmpty() )
      break;

    QString key = output.at(i).simplified().section(":", 0, 0);
    QString value = output.at(i).simplified().section(":", 1, 1);

    vals.insert(key, value);
  }

  retObject.insert(jail, vals);
  return retObject;
}

// List the jails on the box
QJsonObject Iocage::listJails() {
  QJsonObject retObject;

  QStringList output = General::RunCommand("iocage list").split("\n");

  for ( int i = 0; i < output.size(); i++)
  {
    if ( output.at(i).indexOf("JID") != -1 )
      continue;

    if ( output.at(i).isEmpty() )
      break;

    QJsonObject jail;
    QString line = output.at(i).simplified();
    QString uuid = line.section(" ", 1, 1);

    jail.insert("jid", line.section(" ", 0, 0));
    jail.insert("boot", line.section(" ", 2, 2));
    jail.insert("state", line.section(" ", 3, 3));
    jail.insert("tag", line.section(" ", 4, 4));
    jail.insert("type", line.section(" ", 5, 5));
    jail.insert("ip4", line.section(" ", 6, 6));

    retObject.insert(uuid, jail);
  }

  return retObject;
}