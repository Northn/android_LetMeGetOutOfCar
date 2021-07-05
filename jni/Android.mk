LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := LetMeGetOutOfCar
LOCAL_DEBUG := false

# LDLIBS
LOCAL_LDLIBS := -ldl -landroid

LOCAL_SRC_FILES := \
	LMGOOC.h \
	LMGOOC.cpp

# Flags
LOCAL_CFLAGS := -w
LOCAL_CPPFLAGS := -w -s -fvisibility=hidden -Wall -fpack-struct=1 -O2 -std=c++17 -fstrict-aliasing -fno-omit-frame-pointer -mfloat-abi=soft -fstack-protector -fno-short-enums -frtti -g2 -gdwarf-2 -mthumb -ferror-limit=1 -fexceptions

ifneq ($(LOCAL_DEBUG),true)
LOCAL_CFLAGS += -DNDEBUG=1
else
LOCAL_SRC_FILES += log.h
LOCAL_CFLAGS += -D_DEBUG=1
LOCAL_LDLIBS += -llog
endif


include $(BUILD_SHARED_LIBRARY)
