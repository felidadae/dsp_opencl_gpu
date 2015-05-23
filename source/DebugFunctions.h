//
//  DebugFunctions.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 29.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef FilterModule_Standalone_DebugFunctions_h
#define FilterModule_Standalone_DebugFunctions_h

#include <iostream>

template <class PrintedClass>
void printArray(PrintedClass* arr, int size, std::ostream& stream = std::cout ) {
    
    for(int i=0; i<size; ++i) {
        stream << "[" << i << "] " << arr[i] << std::endl;
    }
    stream << std::endl;
    
}

#endif
