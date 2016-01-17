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
#include <string.h>

class AdManagerProtocol {
public:
    static AdManagerProtocol* getInstance();
    virtual void showAD(int n){};
    virtual void share(const char* path, int score) {
        printf("path %s \n", path);
        printf("score %d", score);
    };
};

#endif /* AdManagerProtocol_h */