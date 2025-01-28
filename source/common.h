#pragma once
#include "version.h"

#define SLC_STORAGE_PATH           "storage_slc_wiiuident"

#define HAX_SOURCE_PATH			   "fs:/vol/external01/hax"
#define HAX_DESTINATION_PATH	   SLC_STORAGE_PATH ":/sys/hax"

#define HAX_PLUGINS_SOURCE_PATH			   HAX_SOURCE_PATH "/ios_plugins"
#define HAX_PLUGINS_DESTINATION_PATH       HAX_DESTINATION_PATH "/ios_plugins"

#define APP_VERSION                "v1.1"
#define APP_VERSION_FULL           APP_VERSION APP_VERSION_EXTRA
