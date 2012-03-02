/**
 * Copyright (c) Sjors Gielen, 2010
 * See LICENSE for license.
 */

#ifndef DAZEUS_H
#define DAZEUS_H

// TODO remove
#include "network.h"

class Config;
class PluginComm;
class Network;
class Database;

class DaZeus
{
  public:
             DaZeus( std::string configFileName = std::string() );
            ~DaZeus();
    void     setConfigFileName( std::string fileName );
    QString  configFileName() const;
    bool     configLoaded() const;

    Database *database() const;
    const std::list<Network*> &networks() const { return networks_; }

    void     run();
    bool     loadConfig();
    bool     initPlugins();
    void     autoConnect();
    bool     connectDatabase();

  private:
    void     resetConfig();

    Config          *config_;
    std::string      configFileName_;
    PluginComm      *plugins_;
    Database        *database_;
    std::list<Network*>  networks_;
};

#endif
