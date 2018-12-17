//
//  lutron.cpp
//  lutrond2
//
//  Created by Ed Cole on 08/12/2018.
//  Copyright © 2018 colege. All rights reserved.
//

#include "lutron.hpp"
#include <libconfig.h++>

// Device Methods

Device::Device(const Device &copyFrom){ // copy constructor
        location = copyFrom.location;
        name = copyFrom.name;
        type = copyFrom.type;
        state = copyFrom.state;
        // deep copy of components map
        for(map<int,Component>::const_iterator it = copyFrom.components.begin(); it != copyFrom.components.end(); it++){
            components[it->first] = it->second;
        }
    }

void Device::setName(string name){
    this->name = name;
    }

string Device::getName(){
    return this->name;
    }

int Device::getType(){
    return type;
    }

string Device::getLocation(){
    return location;
    }

string Device::getState(){
    return state;
    }

void Device::addComponent(int index, Component new_component){
    Component c(new_component); // make copy of component
    components[index] = c;      // push to map
    }

void Device::printConfig(int index){
    cout << "device_" << index << " = {\n\t\ttype = " << type << ";"<< endl;
    cout << "\t\t" << "location = \"" << location << "\";"<< endl;
    cout << "\t\t" << "name = \"" << name << "\";"<< endl;
    cout << "\t\t" << "state = \"" << state << "\";"<< endl;
    // rattle through component map calling printConfig on each
    for( auto cit = components.begin(); cit != components.end(); cit++){
        cit->second.printConfig();
    }
    
    cout << "\t};" << endl;
}

// Component Methods
Component::Component(const Component &copyFrom){ // copy constructor
        comp = copyFrom.comp;
        num = copyFrom.num;
        name = copyFrom.name;
        type = copyFrom.type;
        value = copyFrom.value;
    }


string Component::getName(){
    return name;
    }

int Component::getNum(){
        return num;
    }

string Component::getComp(){
    return comp;
    }

int Component::getType(){
    return type;
    }

int Component::getValue(){
    return value;
    }

void Component::printConfig(){
    cout << "\t\tcomp_" << num << " = {\n\t\t\tcomp = \"" << comp << "\";"<< endl;
    cout << "\t\t\t" << "num = " << num << ";"<< endl;
    cout << "\t\t\t" << "name = \"" << name << "\";"<< endl;
    cout << "\t\t\t" << "type = " << type << ";"<< endl;
    cout << "\t\t\t" << "value = " << value << ";"<< endl;
    cout << "\t\t};" << endl;
}




// Lutron methods

bool Lutron::readConfig(){  // this could be in constructor, but think about
                            // command line overides. i.e. they could preset object
                            // properties which would have priority over config file
                            // and default values, so you will want to instantiate
                            // a lutron object, preset any overides from command line
                            // flags etc before loading config file.
    
        string  d_key,      // Device key
                c_key,      // Component key
                cp_key,     // Component Parameter key
                str;        // Utility string
    
        int     dev_id,     // Device ID
                comp_id;    // Components ID
    
        Device      new_device;
        Component   new_comp;
    
        libconfig::Config cfg;
    
        try{
            cfg.readFile(CONFIG_FILE);
        }
        catch(const libconfig::FileIOException &fioex){
            std::cerr << "I/O error while reading file." << std::endl;
            return(EXIT_FAILURE);
        }
        catch(const libconfig::ParseException &pex){
            std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
            << " - " << pex.getError() << std::endl;
            return(EXIT_FAILURE);
        }
        
        const libconfig::Setting& root = cfg.getRoot();
    
        // probe for top level variables - test them for existing
        // ( e.g command line overide) presets, before applying config directives
        if (listen_port.length() == 0) root.lookupValue("listen_port",listen_port);
        if (lutron_host.length() == 0) root.lookupValue("lutron_host",lutron_host);
        if (lutron_port.length() == 0) root.lookupValue("lutron_port",lutron_port);
        if (lutron_user.length() == 0) root.lookupValue("lutron_user",lutron_user);
        if (lutron_password.length() == 0) root.lookupValue("lutron_password",lutron_password);
        if (log_file.length() == 0) root.lookupValue("log_file",log_file);
        if (db_file.length() == 0) root.lookupValue("db_file",db_file);
    
        // iterate through top level config settings and devices
        for(auto element = root.begin(); element != root.end(); element++ ){
            d_key = element->getName();
            //std::cout << d_key << std::endl;
            if ( d_key.compare(0, 7, "device_") == 0 ){ // found a device entry
                
                new_device = Device();  // create Device object on heap
                                        // TODO: need to detroy the old Device - memory leak
                
                dev_id = stoi(d_key.substr(7,string::npos));
                // get top level device parameters
                // type - int
                element->lookupValue("type",new_device.type);
                //std::cout << "\ttype = " << new_device.type << std::endl;
                // location - string
                element->lookupValue("location",new_device.location);
                //std::cout << "\tlocation = " << new_device.location << std::endl;
                // name - string
                element->lookupValue("name",new_device.name);
                //std::cout << "\tname = " << new_device.name << std::endl;
                // state - string
                element->lookupValue("state",new_device.state);
                //std::cout << "\tstate = " << new_device.state << std::endl;
                    // iterate through device parameters and components
                    for(auto comp = element->begin(); comp != element->end(); comp++){
                        c_key = comp->getName();
                        if ( c_key.compare(0,5,"comp_") == 0){  // found a component entry
                            //std::cout << "\t" << c_key << std::endl;
                            comp_id = stoi(c_key.substr(5,string::npos));
                            
                            new_comp = Component(); // TODO: need to detroy the old component - memory leak
                            
                            comp->lookupValue("comp",new_comp.comp);
                            //std::cout << "\t\tcomp = " << new_comp.comp << std::endl;
//                            int num;
                            comp->lookupValue("num",new_comp.num);
                            //std::cout << "\t\tnum = " << new_comp.num << std::endl;
//                            string name;
                            comp->lookupValue("name",new_comp.name);
                            //std::cout << "\t\tname = " << new_comp.name << std::endl;
//                            int type;
                            comp->lookupValue("type",new_comp.type);
                            //std::cout << "\t\ttype = " << new_comp.type << std::endl;
//                            int value;
                            comp->lookupValue("value",new_comp.value);
                            //std::cout << "\t\tvalue = " << new_comp.value << std::endl;
//
                            new_device.addComponent(new_comp.num, new_comp);
                            // TODO delete new_comp - done with it here; or will it be destroyed when this
                            // method returns.
                            
                        }// else, it's one of the device level parameters already probed above
                    }// for comp
                    addDevice(dev_id, new_device);
                    // TODO delete new_device - done with it here; or will it be destroyed when this
                    // method returns.
                
            }// else, it's one of the top level parameters already probed above
            
        }//for element
        
        return (EXIT_SUCCESS);

}

void Lutron::printConfig(){
    
    // rattle through devices calling printConfig Method on each
    for(auto dit = devices.begin(); dit != devices.end(); dit++){
        dit->second.printConfig(dit->first);
        }//for
    }

std::string Lutron::getHost(){
    return lutron_host;
    }

void Lutron::addDevice(int idx,Device dev){
    Device d(dev); // make copy of device
    
    devices[idx] = d; // push it into map
    }

void Lutron::setLutronHost(std::string host){
    this->lutron_host = host;
    }
