//
//  AdManagerProtocol.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/12/27.
//
//

#include "AdManagerProtocol.h"
#include "AdManager.h"

static AdManagerProtocol * adManagerProtocol = nullptr;

AdManagerProtocol* AdManagerProtocol::getInstance(){
    if (adManagerProtocol == nullptr) {
        adManagerProtocol = new AdManager();
    }

    return adManagerProtocol;
}