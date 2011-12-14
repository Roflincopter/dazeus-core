/**
 * Copyright (c) Sjors Gielen, 2010
 * See LICENSE for license.
 */

#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <libircclient.h>
#include <cassert>

#include "user.h"
#include "network.h"

// #define SERVER_FULLDEBUG

class QTimer;
struct ServerConfig;

class ServerThread;
class Server;

QDebug operator<<(QDebug, const Server &);
QDebug operator<<(QDebug, const Server *);

namespace Irc {
	struct Buffer {
		// TEMPORARY PLACEHOLDER
		Server *session_;
		QString receiver_;
		inline Server *session() const { return session_; }
		inline void setReceiver(const QString &p) { receiver_ = p; }
		inline const QString &receiver() const { return receiver_; }

		Buffer(Server *s) : session_(s) {}

		void message(const QString &message);
	};
};

class Server : public QObject
{
Q_OBJECT

friend class TestServer;
friend class ServerThread;

private:
	   Server();

public:
	  ~Server();
	static Server *fromServerConfig( const ServerConfig *c );
	const ServerConfig *config() const;
	QString motd() const;

public slots:
	void connectToServer();
	void disconnectFromServer( Network::DisconnectReason );
	void quit( const QString &reason );
	void whois( const QString &destination );
	void ctcpAction( const QString &destination, const QString &message );
	void ctcpRequest( const QString &destination, const QString &message );
	void join( const QString &channel, const QString &key = QString() );
	void part( const QString &channel, const QString &reason = QString() );
	void message( const QString &destination, const QString &message );

signals:
	void connected();
	void disconnected();
	void welcomed();
	void connectionTimeout();

	void motdReceived( const QString &motd, Irc::Buffer *buffer );
	void joined( const QString &origin, Irc::Buffer *buffer );
	void parted( const QString &origin, const QString &message, Irc::Buffer *buffer );
	void quit(   const QString &origin, const QString &message, Irc::Buffer *buffer );
	void nickChanged( const QString &origin, const QString &nick, Irc::Buffer *buffer );
	void modeChanged( const QString &origin, const QString &mode, const QString &args, Irc::Buffer *buffer );
	void topicChanged( const QString &origin, const QString &topic, Irc::Buffer *buffer );
	void invited( const QString &origin, const QString &receiver, const QString &channel, Irc::Buffer *buffer );
	void kicked( const QString &origin, const QString &nick, const QString &message, Irc::Buffer *buffer );
	void messageReceived( const QString &origin, const QString &message, Irc::Buffer *buffer );
	void noticeReceived( const QString &origin, const QString &notice, Irc::Buffer *buffer );
	void ctcpRequestReceived( const QString &origin, const QString &request, Irc::Buffer *buffer );
	void ctcpReplyReceived( const QString &origin, const QString &reply, Irc::Buffer *buffer );
	void ctcpActionReceived( const QString &origin, const QString &action, Irc::Buffer *buffer );
	void numericMessageReceived( const QString &origin, uint code, const QStringList &params, Irc::Buffer *buffer );
	void unknownMessageReceived( const QString &origin, const QStringList &params, Irc::Buffer *buffer );

private:
	const ServerConfig *config_;
	QString   motd_;
	QTimer   *connectTimer_;
	ServerThread *thread_;
};

#endif
