//
//  AdManagerProtocol.h
//  DayDayUp
//
//  Created by LIUBO on 15/12/27.
//
//

#ifndef AdManagerProtocol_h
#define AdManagerProtocol_h

#include <stdio.h>

class AdManagerProtocol {
public:
    
    static AdManagerProtocol* getInstance();
    void virtual showAD(int n) {};
};

#endif /* AdManagerProtocol_h */