//
//  main.cpp
//  lutrond2
//
//  Created by Ed Cole on 08/12/2018.
//  Copyright © 2018 colege. All rights reserved.
//
// complete rewrite on lutrond using better object models

#include "lutron.hpp"



int main(int argc, const char * argv[]) {
    
    Lutron myLutron;
    myLutron.setLutronHost("another host");
    
    myLutron.readConfig();
    
    myLutron.printConfig();
    
    return 0;
}
