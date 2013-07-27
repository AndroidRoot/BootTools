
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE		:= bootunpack
LOCAL_MODULE_TAGS	:= optional

LOCAL_C_INCLUDES	:= $(LOCAL_PATH)/shared \

LOCAL_SRC_FILES		:= src/bootunpack.c

LOCAL_FORCE_STATIC_EXECUTABLE	:= true
LOCAL_STATIC_LIBRARIES			:= libc libmincrypt

include $(BUILD_EXECUTABLE)
