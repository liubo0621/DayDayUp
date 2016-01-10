//
//  AdManager.h
//  DayDayUp
//
//  Created by LIUBO on 15/12/27.
//
//

#ifndef AdManager_h
#define AdManager_h

#include <stdio.h>
#include "AdManagerProtocol.h"

class AdManager : public AdManagerProtocol {
public:
    void showAD(int n);
};

#endif /* AdManager_h */
