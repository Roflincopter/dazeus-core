/**
 * Copyright (c) Sjors Gielen, 2010
 * See LICENSE for license.
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>

#include "user.h"

class Server;
class Network;

struct ServerConfig;
struct NetworkConfig;

QDebug operator<<(QDebug, const Network &);
QDebug operator<<(QDebug, const Network *);

namespace Irc
{
  class Buffer;
};

class Network : public QObject
{
  Q_OBJECT

  public:
                   ~Network();

    static Network *fromNetworkConfig( const NetworkConfig *c );
    static Network *getNetwork( const QString &name );
    static Network *fromBuffer( Irc::Buffer *b );

    enum DisconnectReason {
      UnknownReason,
      ShutdownReason,
      ConfigurationReloadReason,
      SwitchingServersReason,
      ErrorReason,
      AdminRequestReason,
    };

    bool                        autoConnectEnabled() const;
    const QList<ServerConfig*> &servers() const;
    User                       *user();
    const Server               *activeServer() const;
    const NetworkConfig        *config() const;
    int                         serverUndesirability( const ServerConfig *sc ) const;
    QString                     networkName() const;
    QList<QString>              joinedChannels() const { return knownUsers_.keys(); }
    bool                        isIdentified(const QString &user) const;
    bool                        isKnownUser(const QString &user) const;

  public slots:
    void connectToNetwork( bool reconnect = false );
    void disconnectFromNetwork( DisconnectReason reason = UnknownReason );
    void joinChannel( QString channel );
    void leaveChannel( QString channel );
    void say( QString destination, QString message );
    void action( QString destination, QString message );
    void names( QString channel );
    void ctcp( QString destination, QString message );
    void sendWhois( QString destination );
    void flagUndesirableServer( const ServerConfig *sc );
    void serverIsActuallyOkay( const ServerConfig *sc );

  signals:
    void disconnected();
    void motdReceived( const QString &motd, Irc::Buffer *buffer );
    void modeChanged( const QString &origin, const QString &mode, const QString &args, Irc::Buffer *buffer );
    void topicChanged( const QString &origin, const QString &topic, Irc::Buffer *buffer );
    void invited( const QString &origin, const QString &receiver, const QString &channel, Irc::Buffer *buffer );
    void kicked( const QString &origin, const QString &nick, const QString &message, Irc::Buffer *buffer );
    void ctcpRequestReceived( const QString &origin, const QString &request, Irc::Buffer *buffer );
    void ctcpReplyReceived( const QString &origin, const QString &reply, Irc::Buffer *buffer );
    void ctcpActionReceived( const QString &origin, const QString &action, Irc::Buffer *buffer );
    void numericMessageReceived( const QString &origin, uint code, const QStringList &params, Irc::Buffer *buffer );
    void unknownMessageReceived( const QString &origin, const QStringList &params, Irc::Buffer *buffer );
    void ircEvent(const QString &event, const QString &origin, const QStringList &params, Irc::Buffer *buffer );
    void whoisReceived(const QString &origin, const QString &nick, bool identified, Irc::Buffer *buffer );
    void namesReceived(const QString &origin, const QString &channel, const QStringList &names, Irc::Buffer *buffer );

  private:
    void connectToServer( ServerConfig *conf, bool reconnect );
    bool disconnect( const QObject *sender, const char *signal,
      const QObject *receiver, const char *method )
    { return QObject::disconnect( sender, signal, receiver, method ); }
    bool connect( const QObject *sender, const char *signal,
      const QObject *receiver, const char *method,
      Qt::ConnectionType type = Qt::AutoConnection )
    { return QObject::connect( sender, signal, receiver, method, type ); }

                          Network( const QString &name );
    Server               *activeServer_;
    const NetworkConfig  *config_;
    static QHash<QString,Network*> networks_;
    QHash<const ServerConfig*,int> undesirables_;
    User                 *me_;
    QList<QString>        identifiedUsers_;
    QMap<QString,QStringList> knownUsers_;

  private slots:
    void onFailedConnection();
    void joinedChannel(const QString &user, Irc::Buffer *b);
    void kickedChannel(const QString &user, const QString&, const QString&, Irc::Buffer *b);
    void partedChannel(const QString &user, const QString &, Irc::Buffer *b);
    void slotQuit(const QString &origin, const QString&, Irc::Buffer*);
    void slotWhoisReceived(const QString &origin, const QString &nick, bool identified, Irc::Buffer *buf);
    void slotNickChanged( const QString &origin, const QString &nick, Irc::Buffer *buffer );
    void slotNamesReceived(const QString&, const QString&, const QStringList &names, Irc::Buffer *buf );
    void slotIrcEvent(const QString&, const QString&, const QStringList&, Irc::Buffer *buf);
};

#endif
