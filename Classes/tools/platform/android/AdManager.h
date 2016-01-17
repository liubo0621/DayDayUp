//
//  AdManager.h
//  DayDayUp
//
//  Created by LIUBO on 15/12/27.
//
//

#ifndef AdManager_h
#define AdManager_h

#include "AdManagerProtocol.h"
#include <stdio.h>
#include <string.h>

class AdManager : public AdManagerProtocol {
public:
    virtual void showAD(int n);
    virtual void share(const char *path, int score);
};

#endif /* AdManager_h */
