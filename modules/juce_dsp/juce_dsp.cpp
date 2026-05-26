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

#ifdef JUCE_DSP_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

#include "juce_dsp.h"

#include <juce_audio_formats/juce_audio_formats.h>

#ifndef JUCE_USE_VDSP_FRAMEWORK
 #define JUCE_USE_VDSP_FRAMEWORK 1
#endif

#if (JUCE_MAC || JUCE_IOS) && JUCE_USE_VDSP_FRAMEWORK
 #include <Accelerate/Accelerate.h>
#else
 #undef JUCE_USE_VDSP_FRAMEWORK
#endif

#if JUCE_DSP_USE_INTEL_MKL
 #include <mkl_dfti.h>
#endif

#if _IPP_SEQUENTIAL_STATIC || _IPP_SEQUENTIAL_DYNAMIC || _IPP_PARALLEL_STATIC || _IPP_PARALLEL_DYNAMIC
 #define JUCE_IPP_AVAILABLE 1
#endif

#if JUCE_IPP_AVAILABLE
 #include <ippcore.h>
 #include <ipps.h>
#endif

#if JUCE_USE_PFFFT
 #include <pffft.h>
#endif

#include "processors/juce_FIRFilter.cpp"
#include "processors/juce_IIRFilter.cpp"
#include "processors/juce_FirstOrderTPTFilter.cpp"
#include "processors/juce_Panner.cpp"
#include "processors/juce_Oversampling.cpp"
#include "processors/juce_BallisticsFilter.cpp"
#include "processors/juce_LinkwitzRileyFilter.cpp"
#include "processors/juce_DelayLine.cpp"
#include "processors/juce_DryWetMixer.cpp"
#include "processors/juce_StateVariableTPTFilter.cpp"
#include "maths/juce_SpecialFunctions.cpp"
#include "maths/juce_Matrix.cpp"
#include "maths/juce_LookupTable.cpp"
#include "frequency/juce_FFT.cpp"
#include "frequency/juce_Convolution.cpp"
#include "frequency/juce_Windowing.cpp"
#include "filter_design/juce_FilterDesign.cpp"
#include "widgets/juce_LadderFilter.cpp"
#include "widgets/juce_Compressor.cpp"
#include "widgets/juce_NoiseGate.cpp"
#include "widgets/juce_Limiter.cpp"
#include "widgets/juce_Phaser.cpp"
#include "widgets/juce_Chorus.cpp"

#if JUCE_USE_SIMD
 #if JUCE_INTEL
  #ifdef __AVX2__
   #include "native/juce_SIMDNativeOps_avx.cpp"
  #else
   #include "native/juce_SIMDNativeOps_sse.cpp"
  #endif
 #elif JUCE_ARM
  #include "native/juce_SIMDNativeOps_neon.cpp"
 #else
  #error "SIMD register support not implemented for this platform"
 #endif
#endif

#if JUCE_UNIT_TESTS
 #include "maths/juce_Matrix_test.cpp"
 #include "maths/juce_LogRampedValue_test.cpp"

 #if JUCE_USE_SIMD
  #include "containers/juce_SIMDRegister_test.cpp"
 #endif

 #include "containers/juce_AudioBlock_test.cpp"
 #include "frequency/juce_Convolution_test.cpp"
 #include "frequency/juce_FFT_test.cpp"
 #include "processors/juce_FIRFilter_test.cpp"
 #include "processors/juce_ProcessorChain_test.cpp"
#endif
