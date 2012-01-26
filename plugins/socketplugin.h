/**
 * Copyright (c) Sjors Gielen, 2011
 * See LICENSE for license.
 */

#ifndef SOCKETPLUGIN_H
#define SOCKETPLUGIN_H

#include "plugin.h"
#include <QtCore/QVariant>
#include <QtCore/QIODevice>
#include <QtCore/QByteArray>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QLocalServer>

#include <sstream>
#include <libjson.h>

class SocketPlugin : public Plugin
{
  Q_OBJECT

  struct SocketInfo {
   public:
    SocketInfo(QString t = QString()) : type(t), waitingSize(0) {}
    bool isSubscribed(QString t) const {
      return subscriptions.contains(t);
    }
    void dispatch(QIODevice *d, QString event, QStringList parameters) {
      Q_ASSERT(!event.contains(' '));

      JSONNode params(JSON_ARRAY);
      params.set_name("params");
      foreach(const QString &p, parameters) {
        params.push_back(JSONNode("", libjson::to_json_string(p.toLatin1().constData())));
      }

      JSONNode n(JSON_NODE);
      n.push_back(JSONNode("event", libjson::to_json_string(event.toLatin1().constData())));
      n.push_back(params);

      std::string jsonMsg = libjson::to_std_string(n.write());
      std::string message;
      std::stringstream mstr;
      mstr << jsonMsg.length();
      mstr << jsonMsg;
      mstr >> message;
      d->write(message.c_str(), message.length());
    }
    QString type;
    QList<QString> subscriptions;
    int waitingSize;
  };

  public:
            SocketPlugin(PluginManager *man);
  virtual  ~SocketPlugin();

  public slots:
    virtual void init();
    virtual void welcomed( Network &net );
    virtual void joined( Network &net, const QString &who, Irc::Buffer *channel );

  private slots:
    void newTcpConnection();
    void newLocalConnection();
    void poll();

  private:
    QList<QTcpServer*> tcpServers_;
    QList<QLocalServer*> localServers_;
    QMap<QIODevice*,SocketInfo> sockets_;
    void handle(QIODevice *dev, const QByteArray &line, SocketInfo &info);
};

#endif
