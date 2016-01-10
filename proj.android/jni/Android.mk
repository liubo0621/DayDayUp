LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/game_scene/NormalScene.cpp \
                   ../../Classes/game_scene/ElasticityScene.cpp \
                   ../../Classes/game_scene/TimeScene.cpp \
                   ../../Classes/game_scene/TwinkleScene.cpp \
                   ../../Classes/game_scene/DarkScene.cpp \
                   ../../Classes/game_scene/OppositeGravityScene.cpp \
                   ../../Classes/game_scene/PropsScene.cpp \
                   ../../Classes/gameover_scene/GameOverLayer.cpp \
                   ../../Classes/menu_scene/MenuScene.cpp \
                   ../../Classes/tools/AdManagerProtocol.cpp \
                   ../../Classes/tools/DialogueLayer.cpp \
                   ../../Classes/tools/platform/android/AdManager.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../Classes/game_scene \
$(LOCAL_PATH)/../../Classes/gameover_scene \
$(LOCAL_PATH)/../../Classes/menu_scene \
$(LOCAL_PATH)/../../Classes/tools \
$(LOCAL_PATH)/../../Classes/tools/platform/android

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_SHARED_LIBRARIES := mobclickcpp_shared

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,libmobclickcpp)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
