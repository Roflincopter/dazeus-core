/**
 * Copyright (c) Sjors Gielen, 2010
 * See LICENSE for license.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QSettings>
#include <vector>

/**
 * These structs are only for configuration as it is in the configuration
 * file. This configuration cannot be changed via the runtime interface of
 * the IRC bot. Things like autojoining channels are not in here, because
 * that's handled by admins during runtime... :)
 */
struct ServerConfig;

struct NetworkConfig {
  std::string name;
  std::string displayName;
  std::string nickName;
  std::string userName;
  std::string fullName;
  std::string password;
  std::vector<ServerConfig*> servers;
  bool autoConnect;
};

struct ServerConfig {
  std::string host;
  uint16_t port;
  uint8_t priority;
  NetworkConfig *network;
  bool ssl;
};

struct DatabaseConfig {
  QString type;
  QString hostname;
  quint16 port;
  QString username;
  QString password;
  QString database;
  QString options;
};

class Config : public QObject
{
  Q_OBJECT

  public:
        Config();
       ~Config();
  bool  loadFromFile( std::string fileName );

  const std::list<NetworkConfig*> &networks();
  const QString               &lastError();
  const DatabaseConfig        *databaseConfig() const;
  const QMap<QString,QVariant> groupConfig(QString plugin) const;

  private:
  std::list<NetworkConfig*> oldNetworks_;
  std::list<NetworkConfig*> networks_;
  QString               error_;
  QSettings            *settings_;
  DatabaseConfig       *databaseConfig_;
};

#endif
