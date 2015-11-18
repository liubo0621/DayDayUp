//
//  lua_binding.cpp

//
//  Created by zhouhouzhen on 15-7-21.
//  Copyright (c) 2015年 Umeng Inc. All rights reserved.
//
//#ifndef UMENG_MOBCLICKCPP_SOURCE

#include "lua_binding.h"
#include "MobClickCpp.h"
#include "LuaBasicConversions.h"
//#include "tolua++.h"
extern "C" {
#include "lua.h"
}
#define UMENG_SUCCEED 0
#define UMENG_FAILED 1


//class testMobClickOnlineConfigUpdateDelegate:public umeng::MobClickOnlineConfigUpdateDelegate{
//public:
//    void onUpdateFinished(bool succeed, void* pUserData){
//        int a = 1;
//        a *= 1;
//    }
//};

//template <class T>
//void object_to_luaval(lua_State* L,const char* type, T* ret)
//{
//    if(NULL != ret)
//    {
//        if (std::is_base_of<cocos2d::Ref, T>::value)
//        {
//            // use c style cast, T may not polymorphic
//            cocos2d::Ref* dynObject = (cocos2d::Ref*)(ret);
//            int ID = (int)(dynObject->_ID) ;
//            int* luaID = &(dynObject->_luaID);
//            toluafix_pushusertype_ccobject(L,ID, luaID, (void*)ret,type);
//        }
//        else
//        {
//            tolua_pushusertype(L,(void*)ret,type);
//        }
//    }
//    else
//    {
//        lua_pushnil(L);
//    }
//}




template <class T>
bool umeng_luaval_to_object(lua_State* L, int lo, const char* type, T** ret)
{
    if(NULL == L || lua_gettop(L) < lo)
        return false;
    if (luaval_is_usertype(L, lo, type, 0))
    {
        *ret = static_cast<T*>(tolua_tousertype(L, lo, 0));
        return true;
    }
    return false;
}


bool umeng_luaval_to_stdmap(lua_State* L, int lo, std::map<std::string, std::string>* ret, const char* funcName)
{
    if ( NULL == L || NULL == ret)
        return false;
    tolua_Error tolua_err;
    if (tolua_istable(L, lo, 0, &tolua_err)){
        std::string stringKey = "";
        std::string stringValue = "";
        std::map<std::string, std::string>& dict = *ret;
        lua_pushnil(L);                                             /* first key L: lotable ..... nil */
        while ( 0 != lua_next(L, lo ) )                             /* L: lotable ..... key value */
        {
            if (!lua_isstring(L, -2))
            {
                lua_pop(L, 1);                                      /* removes 'value'; keep 'key' for next iteration*/
                continue;
            }
            if(luaval_to_std_string(L, -2, &stringKey))
            {
                if(lua_type(L, -1) == LUA_TSTRING)
                {
                    if(luaval_to_std_string(L, -1, &stringValue))
                    {
                        dict[stringKey] = stringValue;
                    }
                }
            }
            lua_pop(L, 1);                                          /* L: lotable ..... key */
        }
        return true;
    }
    ret = NULL;
    return true;
}

int lua_umeng_mobclickcpp_setLogEnabled(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 1){
        bool logEnable;
        if(luaval_to_boolean(L, 1, &logEnable, "umeng_setLogEnabled")){
            umeng::MobClickCpp::setLogEnabled(logEnable);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_setProxy(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 2)
    {
        const char* ipaddr;
        int portnum;
        std::string ipaddr_tmp;
        
        if (luaval_to_std_string(L, 1, &ipaddr_tmp, "umeng_setProxy")&&
            luaval_to_int32(L, 2, &portnum, "umeng_setProxy")){
            ipaddr = ipaddr_tmp.c_str();
            umeng::MobClickCpp::setProxy(ipaddr,portnum);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_setSessionIdleLimit(lua_State* L)
{
    int paranum = 0;
    
    paranum = lua_gettop(L);
    if (paranum == 1)
    {
        int limitnum;
        if(luaval_to_int32(L, 1, &limitnum, "umeng_setSessionIdleLimit")){
            umeng::MobClickCpp::setSessionIdleLimit(limitnum);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_setEncryptEnabled(lua_State* L)
{
    int paranum = 0;
    
    paranum = lua_gettop(L);
    if (paranum == 1)
    {
        bool arg1;
        if(luaval_to_boolean(L, 1, &arg1, "umeng_setEncryptEnabled")){
            umeng::MobClickCpp::setEncryptEnabled(arg1);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_doNotCallItFromYourCode(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 2)
    {
        const char* appkey;
        const char* channelid;
        std::string appkey_tmp;
        std::string channelid_tmp;
        if (luaval_to_std_string(L, 1, &appkey_tmp, "umeng_doNotCallItFromYourCode")&&
            luaval_to_std_string(L, 2, &channelid_tmp, "umeng_doNotCallItFromYourCode")){
            appkey = appkey_tmp.c_str();
            channelid = channelid_tmp.c_str();
            umeng::MobClickCpp::doNotCallItFromYourCode(appkey,channelid);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}

int lua_umeng_mobclickcpp_end(lua_State* L)
{
    umeng::MobClickCpp::end();
    return 0;
}
int lua_umeng_mobclickcpp_mainloop(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 1)
    {
        double dt;
        if(luaval_to_number(L, 1, &dt, "umeng_mainloop")){
            umeng::MobClickCpp::mainloop(dt);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}

int lua_umeng_mobclickcpp_event(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 1)
    {
        const char* eventid;
        std::string eventid_tmp;
        if(luaval_to_std_string(L, 1, &eventid_tmp, "umeng_event")){
            eventid = eventid_tmp.c_str();
            umeng::MobClickCpp::event(eventid);
            return UMENG_SUCCEED;
        }
    }else if(paranum == 2){
        const char* eventid;
        const char* label;
        
        std::string eventid_tmp;
        bool id_ok = luaval_to_std_string(L, 1, &eventid_tmp, "umeng_event");
        std::string label_tmp;
        tolua_Error tolua_err;
        if(tolua_isstring(L, 2, 0, &tolua_err)){
            id_ok &= luaval_to_std_string(L, 2, &label_tmp, "umeng_event");
            if(id_ok){
                eventid = eventid_tmp.c_str();
                label = label_tmp.c_str();
                umeng::MobClickCpp::event(eventid,label);
                return UMENG_SUCCEED;
            }
        }else{
            std::map<std::string,std::string> dict;
            id_ok &=  umeng_luaval_to_stdmap(L, 2, &dict, "umeng_event");
            if(id_ok){
                eventid = eventid_tmp.c_str();
                umeng::MobClickCpp::event(eventid,&dict);
                return UMENG_SUCCEED;
            }
        }
    }else if(paranum == 3){
        bool ok = true;
        const char* eventid;
        
        std::string eventid_tmp; ok &= luaval_to_std_string(L, 1, &eventid_tmp, "umeng_event");
        eventid = eventid_tmp.c_str();
        std::map<std::string,std::string> dict;
        ok &=  umeng_luaval_to_stdmap(L, 2, &dict, "umeng_event");
        int counter;
        ok &= luaval_to_int32(L, 3, &counter, "umeng_event");
        if (ok){
            umeng::MobClickCpp::event(eventid,&dict,counter);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_eventWithAttributesString(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if(paranum == 2){
        bool ok = true;
        const char* eventid;
        std::string eventid_tmp;
        ok &= luaval_to_std_string(L, 1, &eventid_tmp, "umeng_eventWithAttributesString");
        eventid = eventid_tmp.c_str();
        const char* attri;
        std::string attri_tmp;
        ok &= luaval_to_std_string(L, 2, &attri_tmp, "umeng_eventWithAttributesString");
        attri = attri_tmp.c_str();
        if (ok){
            umeng::MobClickCpp::eventWithAttributesString(eventid,attri);
            return UMENG_SUCCEED;
        }
    }else if(paranum == 3){
        bool ok = true;
        const char* eventid;
        std::string eventid_tmp;
        ok &= luaval_to_std_string(L, 1, &eventid_tmp, "umeng_eventWithAttributesString");
        eventid = eventid_tmp.c_str();
        const char* attri;
        std::string attri_tmp;
        ok &= luaval_to_std_string(L, 2, &attri_tmp, "umeng_eventWithAttributesString");
        attri = attri_tmp.c_str();
        int counter;
        ok &= luaval_to_int32(L, 3, &counter, "umeng_eventWithAttributesString");
        if (ok){
            umeng::MobClickCpp::eventWithAttributesString(eventid,attri,counter);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_beginLogPageView(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if(paranum == 1){
        const char* pName;
        std::string pName_tmp;
        if(luaval_to_std_string(L, 1, &pName_tmp, "umeng_beginLogPageView")){
            pName = pName_tmp.c_str();
            umeng::MobClickCpp::beginLogPageView(pName);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_endLogPageView(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if(paranum == 1){
        std::string pName;
        if(luaval_to_std_string(L, 1, &pName, "umeng_endLogPageView")){
            umeng::MobClickCpp::endLogPageView(pName.c_str());
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_updateOnlineConfig(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 0){
        umeng::MobClickCpp::updateOnlineConfig();
    }else if(paranum == 1){
        umeng::MobClickOnlineConfigUpdateDelegate* delegate = NULL;
        if(umeng_luaval_to_object(L, 1, "MobClickOnlineConfigUpdateDelegate", &delegate)){
            umeng::MobClickCpp::updateOnlineConfig(delegate);
            return UMENG_SUCCEED;
        }
    }else if (paranum == 2){
        umeng::MobClickOnlineConfigUpdateDelegate* delegate = NULL;
        void * pUserData = NULL;
        if(umeng_luaval_to_object(L, 1, "MobClickOnlineConfigUpdateDelegate", &delegate)){
            pUserData = static_cast<void*>(tolua_touserdata(L, 2, 0));
            umeng::MobClickCpp::updateOnlineConfig(delegate,pUserData);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_getConfigParam(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if(paranum == 1){
        const char* key;
        std::string key_tmp;
        if(luaval_to_std_string(L, 1, &key_tmp, "umeng_getConfigParam")){
            key = key_tmp.c_str();
            const char* ret = umeng::MobClickCpp::getConfigParam(key);
            lua_pushstring(L,ret);
            /*返回返回值个数*/
            return 1;
        }
    }
    /*返回返回值个数*/
    return 0;
}
int lua_umeng_mobclickcpp_profileSignIn(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if(paranum == 1){
        const char* puid;
        std::string puid_tmp;
        if(luaval_to_std_string(L, 1, &puid_tmp, "umeng_profileSignIn")){
            puid = puid_tmp.c_str();
            umeng::MobClickCpp::profileSignIn(puid);
            return UMENG_SUCCEED;
        }
    }else if(paranum == 2){
        const char* puid;
        const char* provider;
        std::string puid_tmp;
        std::string provider_tmp;
        if(luaval_to_std_string(L, 1, &puid_tmp, "umeng_profileSignIn")&&
           luaval_to_std_string(L, 2, &provider_tmp, "umeng_profileSignIn")){
            puid = puid_tmp.c_str();
            provider = provider_tmp.c_str();
            umeng::MobClickCpp::profileSignIn(puid,provider);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_profileSignOff(lua_State* L)
{
    umeng::MobClickCpp::profileSignOff();
    return UMENG_SUCCEED;
}
int lua_umeng_mobclickcpp_setUserLevel(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 1)
    {
        int level;
        if(luaval_to_int32(L, 1, &level, "umeng_setUserLevel")){
            umeng::MobClickCpp::setUserLevel(level);
            return UMENG_SUCCEED;
        };
    }
    return UMENG_FAILED;
}

int lua_umeng_mobclickcpp_startLevel(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 1)
    {
        const char* level;
        std::string level_tmp;
        if(luaval_to_std_string(L, 1, &level_tmp, "umeng_startLevel")){
            level = level_tmp.c_str();
            umeng::MobClickCpp::startLevel(level);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_finishLevel(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 1)
    {
        const char* level;
        std::string level_tmp;
        if(luaval_to_std_string(L, 1, &level_tmp, "umeng_finishLevel")){
            level = level_tmp.c_str();
            umeng::MobClickCpp::finishLevel(level);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_failLevel(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 1)
    {
        const char* level;
        std::string level_tmp;
        if(luaval_to_std_string(L, 1, &level_tmp, "umeng_failLevel")){
            level = level_tmp.c_str();
            umeng::MobClickCpp::failLevel(level);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_pay(lua_State* L)
{
    int paranum = 0;
    
    paranum = lua_gettop(L);
    if (paranum == 3)
    {
        double cash;
        bool ok = true;
        ok &= luaval_to_number(L,1,&cash,"umeng_pay");
        
        int source;
        ok &= luaval_to_int32(L,2,&source,"umeng_pay");
        
        double coin;
        ok &= luaval_to_number(L,3,&coin,"umeng_pay");
        if(ok){
            umeng::MobClickCpp::pay(cash,source,coin);
            return UMENG_SUCCEED;
        }
    }else if(paranum == 5){
        bool ok = true;
        double cash;
        ok &= luaval_to_number(L,1,&cash,"umeng_pay");
        
        int source;
        ok &= luaval_to_int32(L,2,&source,"umeng_pay");
        
        const char* item;
        std::string item_tmp;
        ok &= luaval_to_std_string(L, 3, &item_tmp, "umeng_pay");
        item = item_tmp.c_str();
        
        int amount;
        ok &= luaval_to_int32(L,4,&amount,"umeng_pay");
        
        double price;
        ok &= luaval_to_number(L,5,&price,"umeng_pay");
        if(ok){
            umeng::MobClickCpp::pay(cash,source,item,amount,price);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_buy(lua_State* L)
{
    int paranum = 0;
    
    paranum = lua_gettop(L);
    if (paranum == 3)
    {
        bool ok = true;
        const char* item;
        string item_tmp;
        ok &= luaval_to_std_string(L,1,&item_tmp,"umeng_buy");
        item = item_tmp.c_str();

        int amount;
        ok &= luaval_to_int32(L,2,&amount,"umeng_buy");
        
        double price;
        ok &= luaval_to_number(L,3,&price,"umeng_buy");
        if(ok){
            umeng::MobClickCpp::buy(item,amount,price);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_use(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 3)
    {
        bool ok = true;
        const char* item;
        string item_tmp;
        ok &= luaval_to_std_string(L,1,&item_tmp,"umeng_use");
        item = item_tmp.c_str();

        int amount;
        ok &= luaval_to_int32(L,2,&amount,"umeng_use");
        
        double price;
        ok &= luaval_to_number(L,3,&price,"umeng_use");
        if(ok){
            umeng::MobClickCpp::use(item,amount,price);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_bonus(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 2)
    {
        bool ok = true;
        double coin;
        ok &= luaval_to_number(L,1,&coin,"umeng_bonus");
        
        int source;
        ok &= luaval_to_int32(L,2,&source,"umeng_bonus");
        if(ok){
            umeng::MobClickCpp::bonus(coin,source);
            return UMENG_SUCCEED;
        }
        
    }else if(paranum == 4){
        bool ok = true;
        const char* item;
        string item_tmp;
        ok &= luaval_to_std_string(L,1,&item_tmp,"umeng_bonus");
        item = item_tmp.c_str();

        int amount;
        ok &= luaval_to_int32(L,2,&amount,"umeng_bonus");

        double price;
        ok &= luaval_to_number(L,3,&price,"umeng_bonus");
        
        int source;
        ok &= luaval_to_int32(L,4,&source,"umeng_bonus");
        if(ok){
            umeng::MobClickCpp::bonus(item,amount,price,source);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_setLatency(lua_State* L)
{
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 1)
    {
        unsigned int  latency;
        if(luaval_to_uint32(L,1,&latency,"umeng_setLatency")){
            umeng::MobClickCpp::setLatency(latency);
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
int lua_umeng_mobclickcpp_start(lua_State* L){
    int paranum = 0;
    paranum = lua_gettop(L);
    if (paranum == 1)
    {
        string key;
        if(luaval_to_std_string(L,1,&key,"umeng_mobclickstart")){
            MOBCLICKCPP_START_WITH_APPKEY(key.c_str());
            return UMENG_SUCCEED;
        }
    }else if(paranum == 2){
        string key;
        string channel;
        if(luaval_to_std_string(L,1,&key,"umeng_mobclickstart")&&
           luaval_to_std_string(L,2,&channel,"umeng_mobclickstart")){
            MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL(key.c_str(),channel.c_str());
            return UMENG_SUCCEED;
        }
    }
    return UMENG_FAILED;
}
//static int createMobClickOnlineConfigUpdateDelegateTest(lua_State* L){
//    
//    testMobClickOnlineConfigUpdateDelegate* test=new testMobClickOnlineConfigUpdateDelegate();
//    
//    object_to_luaval(L, "MobClickOnlineConfigUpdateDelegate",(testMobClickOnlineConfigUpdateDelegate*)test);
//    return 1;
//}
int lua_register_UmengClick(lua_State* L)
{
    lua_register(L,"umeng_mobclickstart",lua_umeng_mobclickcpp_start);
    lua_register(L,"umeng_setLogEnabled",lua_umeng_mobclickcpp_setLogEnabled);
    lua_register(L,"umeng_setProxy",lua_umeng_mobclickcpp_setProxy);
    lua_register(L,"umeng_setSessionIdleLimit",lua_umeng_mobclickcpp_setSessionIdleLimit);
    lua_register(L,"umeng_setEncryptEnabled",lua_umeng_mobclickcpp_setEncryptEnabled);
    lua_register(L,"umeng_doNotCallItFromYourCode",lua_umeng_mobclickcpp_doNotCallItFromYourCode);
    lua_register(L,"umeng_end", lua_umeng_mobclickcpp_end);
    lua_register(L,"umeng_mainloop", lua_umeng_mobclickcpp_mainloop);
    lua_register(L,"umeng_event", lua_umeng_mobclickcpp_event);
    lua_register(L,"umeng_eventWithAttributesString", lua_umeng_mobclickcpp_eventWithAttributesString);
    lua_register(L,"umeng_beginLogPageView", lua_umeng_mobclickcpp_beginLogPageView);
    lua_register(L,"umeng_endLogPageView", lua_umeng_mobclickcpp_endLogPageView);
    lua_register(L,"umeng_updateOnlineConfig", lua_umeng_mobclickcpp_updateOnlineConfig);
    lua_register(L,"umeng_getConfigParam", lua_umeng_mobclickcpp_getConfigParam);
    lua_register(L,"umeng_profileSignIn", lua_umeng_mobclickcpp_profileSignIn);
    lua_register(L,"umeng_profileSignOff", lua_umeng_mobclickcpp_profileSignOff);
    lua_register(L,"umeng_setUserLevel", lua_umeng_mobclickcpp_setUserLevel);
    lua_register(L,"umeng_startLevel", lua_umeng_mobclickcpp_startLevel);
    lua_register(L,"umeng_finishLevel", lua_umeng_mobclickcpp_finishLevel);
    lua_register(L,"umeng_failLevel", lua_umeng_mobclickcpp_failLevel);
    lua_register(L,"umeng_pay", lua_umeng_mobclickcpp_pay);
    lua_register(L,"umeng_buy", lua_umeng_mobclickcpp_buy);
    lua_register(L,"umeng_use", lua_umeng_mobclickcpp_use);
    lua_register(L,"umeng_bonus", lua_umeng_mobclickcpp_bonus);
    lua_register(L,"umeng_setLatency", lua_umeng_mobclickcpp_setLatency);

    return 1;
}

//int lua_register_test(lua_State* L){
//    tolua_usertype(L,"MobClickOnlineConfigUpdateDelegate");
////    tolua_cclass(L,"testMobClickOnlineConfigUpdateDelegate","testMobClickOnlineConfigUpdateDelegate","MobClickOnlineConfigUpdateDelegate",NULL);
//    
//    tolua_function(L,"MobClickOnlineConfigUpdateDelegateTest",createMobClickOnlineConfigUpdateDelegateTest);
//    return 1;
//}

int lua_register_mobclick_module(lua_State* L)
{
    lua_getglobal(L, "_G");
    if (lua_istable(L,-1))//stack:...,_G,
    {
        lua_register_UmengClick(L);
        
    }
    lua_pop(L, 1);
    
    
    /*tolua_open(L);                     测试在线参数功能的lua实现接口
    
    tolua_module(L,"umeng",0);
    tolua_beginmodule(L,"umeng");
    lua_register_test(L);
    tolua_endmodule(L);*/
    return 1;
}
//#endif