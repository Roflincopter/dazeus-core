/**
 * Copyright (c) Sjors Gielen, 2010
 * See LICENSE for license.
 */

#include <QtCore/QDebug>
#include <ircclient-qt/IrcBuffer>

#include "testplugin.h"

TestPlugin::TestPlugin()
: Plugin()
{}

TestPlugin::~TestPlugin() {}

void TestPlugin::init()
{
  qDebug() << "TestPlugin initialising.";
}

void TestPlugin::connected( Network &net, const Server &serv )
{
  qDebug() << "Connected to network" << net;
  if( &serv != net.activeServer() )
  {
    qWarning() << "Warning! Connected to different server" << serv;
  }
}

void TestPlugin::authenticated( Network &net, const Server &serv )
{
  qDebug() << "Authenticated to network " << net << ", server " << serv;
  net.joinChannel( "#dazjorz" );
}

void TestPlugin::joinedChannel( const QString &who, Irc::Buffer *channel )
{
  User u( who, Network::fromBuffer( channel ) );
  qDebug() << "User " << u << " joined channel " << channel;
  if( u.isMe() )
  {
    channel->message( "Hi all!" );
  }
}

void TestPlugin::leftChannel( const QString &who, const QString &leaveMessage,
                              Irc::Buffer *channel )
{
  qDebug() << "User " << who << " left channel " << channel
           << " (leave message " << leaveMessage << ")";
}

QHash<QString, Plugin::VariableScope> TestPlugin::variables()
{
  QHash<QString, Plugin::VariableScope> variables;
  return variables;
}