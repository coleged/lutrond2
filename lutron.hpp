//
//  lutron.hpp
//  lutrond2
//
//  Created by Ed Cole on 08/12/2018.
//  Copyright © 2018 colege. All rights reserved.
//
//  re-write of lutrond using better C++.

#ifndef lutron_hpp
#define lutron_hpp

#define CONFIG_FILE "/usr/local/etc/lutrond.conf"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>


// for now I am using strings for all these
//typedef enum {lamp, fan, motor, control} device_t;
//typedef enum {button, led} comp_t;
//typedef enum {on, off}  state_t;


using namespace std;

class Component {
    
    friend class Lutron;
    friend class Device;

public:
    
    Component(){};
    ~Component(){};
    Component(const Component &copyFrom);
    
private:
    int num;
    string comp;
    string name;
    int type;
    int value;

    
    int getNum();
    string getComp();
    string getName();
    int getType();
    int getValue();
    void printConfig();
    
};

class Device {
    
    friend class Lutron;
 
public:
    
    Device(){};
    ~Device(){};
    Device(const Device &copyFrom);

private:
    string location;
    string name;
    int type;
    string state;

    map<int,Component> components;
  
    string getLocation();
    string getName();
    void setName(string name);
    int getType();
    string getState();
    
    void addComponent(int index, Component new_component);
    
    void printConfig(int index);
    
};

class Lutron{
    
public:
    
    Lutron(){};
    ~Lutron(){};
    
    bool readConfig();
    void printConfig();
    
    void setLutronHost(std::string host);
    std::string getHost();
    
private:
    
    string  listen_port;
    string  lutron_host;
    string  lutron_port;
    string  lutron_user;
    string  lutron_password;
    string  log_file;
    string  db_file;
    
    map<int,Device> devices;
    
    void addDevice(int idx,Device dev);
    
};

#endif /* lutron_hpp */
