/*
  ==============================================================================

   This file is part of the JUCE 9 preview.
   Copyright (c) Raw Material Software Limited

   You may use this code under the terms of the AGPLv3
   (see www.gnu.org/licenses).

   For the JUCE 9 preview this file cannot be licensed commercially.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#ifdef JUCE_PRODUCT_UNLOCKING_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

#define JUCE_CORE_INCLUDE_JNI_HELPERS    1
#define JUCE_CORE_INCLUDE_OBJC_HELPERS   1
#define JUCE_CORE_INCLUDE_NATIVE_HEADERS 1

#ifdef JUCE_IN_APP_PURCHASES_USE_SANDBOX_ENVIRONMENT
 JUCE_COMPILER_WARNING ("JUCE_IN_APP_PURCHASES_USE_SANDBOX_ENVIRONMENT is no longer supported")
#endif

#include "juce_product_unlocking.h"

#if JUCE_IOS || JUCE_MAC
 #import <StoreKit/StoreKit.h>
#endif

#if JUCE_IN_APP_PURCHASES
 #if JUCE_ANDROID
  #include "native/juce_InAppPurchases_android.cpp"
 #elif JUCE_IOS || JUCE_MAC
  #include "native/juce_InAppPurchases_ios.cpp"
 #endif

 #include "in_app_purchases/juce_InAppPurchases.cpp"
#endif

#include "marketplace/juce_OnlineUnlockStatus.cpp"

#if JUCE_MODULE_AVAILABLE_juce_data_structures
 #include "marketplace/juce_TracktionMarketplaceStatus.cpp"
#endif

#if JUCE_MODULE_AVAILABLE_juce_gui_extra
 #include "marketplace/juce_OnlineUnlockForm.cpp"
#endif
