// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME vmc_targetSim_dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "Ex01MCApplication.h"
#include "Ex01MCStack.h"
#include "Ex01DetectorConstructionOld.h"
#include "Ex01Particle.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_Ex01Particle(void *p = nullptr);
   static void *newArray_Ex01Particle(Long_t size, void *p);
   static void delete_Ex01Particle(void *p);
   static void deleteArray_Ex01Particle(void *p);
   static void destruct_Ex01Particle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Ex01Particle*)
   {
      ::Ex01Particle *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Ex01Particle >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Ex01Particle", ::Ex01Particle::Class_Version(), "Ex01Particle.h", 31,
                  typeid(::Ex01Particle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Ex01Particle::Dictionary, isa_proxy, 4,
                  sizeof(::Ex01Particle) );
      instance.SetNew(&new_Ex01Particle);
      instance.SetNewArray(&newArray_Ex01Particle);
      instance.SetDelete(&delete_Ex01Particle);
      instance.SetDeleteArray(&deleteArray_Ex01Particle);
      instance.SetDestructor(&destruct_Ex01Particle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Ex01Particle*)
   {
      return GenerateInitInstanceLocal(static_cast<::Ex01Particle*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Ex01Particle*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Ex01MCStack(void *p = nullptr);
   static void *newArray_Ex01MCStack(Long_t size, void *p);
   static void delete_Ex01MCStack(void *p);
   static void deleteArray_Ex01MCStack(void *p);
   static void destruct_Ex01MCStack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Ex01MCStack*)
   {
      ::Ex01MCStack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Ex01MCStack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Ex01MCStack", ::Ex01MCStack::Class_Version(), "Ex01MCStack.h", 32,
                  typeid(::Ex01MCStack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Ex01MCStack::Dictionary, isa_proxy, 4,
                  sizeof(::Ex01MCStack) );
      instance.SetNew(&new_Ex01MCStack);
      instance.SetNewArray(&newArray_Ex01MCStack);
      instance.SetDelete(&delete_Ex01MCStack);
      instance.SetDeleteArray(&deleteArray_Ex01MCStack);
      instance.SetDestructor(&destruct_Ex01MCStack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Ex01MCStack*)
   {
      return GenerateInitInstanceLocal(static_cast<::Ex01MCStack*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Ex01MCStack*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Ex01MCApplication(void *p = nullptr);
   static void *newArray_Ex01MCApplication(Long_t size, void *p);
   static void delete_Ex01MCApplication(void *p);
   static void deleteArray_Ex01MCApplication(void *p);
   static void destruct_Ex01MCApplication(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Ex01MCApplication*)
   {
      ::Ex01MCApplication *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Ex01MCApplication >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Ex01MCApplication", ::Ex01MCApplication::Class_Version(), "Ex01MCApplication.h", 35,
                  typeid(::Ex01MCApplication), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Ex01MCApplication::Dictionary, isa_proxy, 4,
                  sizeof(::Ex01MCApplication) );
      instance.SetNew(&new_Ex01MCApplication);
      instance.SetNewArray(&newArray_Ex01MCApplication);
      instance.SetDelete(&delete_Ex01MCApplication);
      instance.SetDeleteArray(&deleteArray_Ex01MCApplication);
      instance.SetDestructor(&destruct_Ex01MCApplication);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Ex01MCApplication*)
   {
      return GenerateInitInstanceLocal(static_cast<::Ex01MCApplication*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Ex01MCApplication*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Ex01DetectorConstructionOld(void *p = nullptr);
   static void *newArray_Ex01DetectorConstructionOld(Long_t size, void *p);
   static void delete_Ex01DetectorConstructionOld(void *p);
   static void deleteArray_Ex01DetectorConstructionOld(void *p);
   static void destruct_Ex01DetectorConstructionOld(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Ex01DetectorConstructionOld*)
   {
      ::Ex01DetectorConstructionOld *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Ex01DetectorConstructionOld >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Ex01DetectorConstructionOld", ::Ex01DetectorConstructionOld::Class_Version(), "Ex01DetectorConstructionOld.h", 37,
                  typeid(::Ex01DetectorConstructionOld), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Ex01DetectorConstructionOld::Dictionary, isa_proxy, 4,
                  sizeof(::Ex01DetectorConstructionOld) );
      instance.SetNew(&new_Ex01DetectorConstructionOld);
      instance.SetNewArray(&newArray_Ex01DetectorConstructionOld);
      instance.SetDelete(&delete_Ex01DetectorConstructionOld);
      instance.SetDeleteArray(&deleteArray_Ex01DetectorConstructionOld);
      instance.SetDestructor(&destruct_Ex01DetectorConstructionOld);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Ex01DetectorConstructionOld*)
   {
      return GenerateInitInstanceLocal(static_cast<::Ex01DetectorConstructionOld*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Ex01DetectorConstructionOld*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Ex01Particle::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Ex01Particle::Class_Name()
{
   return "Ex01Particle";
}

//______________________________________________________________________________
const char *Ex01Particle::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Ex01Particle*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Ex01Particle::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Ex01Particle*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Ex01Particle::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Ex01Particle*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Ex01Particle::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Ex01Particle*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Ex01MCStack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Ex01MCStack::Class_Name()
{
   return "Ex01MCStack";
}

//______________________________________________________________________________
const char *Ex01MCStack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Ex01MCStack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Ex01MCStack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Ex01MCStack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Ex01MCStack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Ex01MCStack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Ex01MCStack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Ex01MCStack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Ex01MCApplication::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Ex01MCApplication::Class_Name()
{
   return "Ex01MCApplication";
}

//______________________________________________________________________________
const char *Ex01MCApplication::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Ex01MCApplication*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Ex01MCApplication::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Ex01MCApplication*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Ex01MCApplication::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Ex01MCApplication*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Ex01MCApplication::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Ex01MCApplication*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Ex01DetectorConstructionOld::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Ex01DetectorConstructionOld::Class_Name()
{
   return "Ex01DetectorConstructionOld";
}

//______________________________________________________________________________
const char *Ex01DetectorConstructionOld::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Ex01DetectorConstructionOld*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Ex01DetectorConstructionOld::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Ex01DetectorConstructionOld*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Ex01DetectorConstructionOld::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Ex01DetectorConstructionOld*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Ex01DetectorConstructionOld::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Ex01DetectorConstructionOld*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Ex01Particle::Streamer(TBuffer &R__b)
{
   // Stream an object of class Ex01Particle.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Ex01Particle::Class(),this);
   } else {
      R__b.WriteClassBuffer(Ex01Particle::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Ex01Particle(void *p) {
      return  p ? new(p) ::Ex01Particle : new ::Ex01Particle;
   }
   static void *newArray_Ex01Particle(Long_t nElements, void *p) {
      return p ? new(p) ::Ex01Particle[nElements] : new ::Ex01Particle[nElements];
   }
   // Wrapper around operator delete
   static void delete_Ex01Particle(void *p) {
      delete (static_cast<::Ex01Particle*>(p));
   }
   static void deleteArray_Ex01Particle(void *p) {
      delete [] (static_cast<::Ex01Particle*>(p));
   }
   static void destruct_Ex01Particle(void *p) {
      typedef ::Ex01Particle current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Ex01Particle

//______________________________________________________________________________
void Ex01MCStack::Streamer(TBuffer &R__b)
{
   // Stream an object of class Ex01MCStack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Ex01MCStack::Class(),this);
   } else {
      R__b.WriteClassBuffer(Ex01MCStack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Ex01MCStack(void *p) {
      return  p ? new(p) ::Ex01MCStack : new ::Ex01MCStack;
   }
   static void *newArray_Ex01MCStack(Long_t nElements, void *p) {
      return p ? new(p) ::Ex01MCStack[nElements] : new ::Ex01MCStack[nElements];
   }
   // Wrapper around operator delete
   static void delete_Ex01MCStack(void *p) {
      delete (static_cast<::Ex01MCStack*>(p));
   }
   static void deleteArray_Ex01MCStack(void *p) {
      delete [] (static_cast<::Ex01MCStack*>(p));
   }
   static void destruct_Ex01MCStack(void *p) {
      typedef ::Ex01MCStack current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Ex01MCStack

//______________________________________________________________________________
void Ex01MCApplication::Streamer(TBuffer &R__b)
{
   // Stream an object of class Ex01MCApplication.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Ex01MCApplication::Class(),this);
   } else {
      R__b.WriteClassBuffer(Ex01MCApplication::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Ex01MCApplication(void *p) {
      return  p ? new(p) ::Ex01MCApplication : new ::Ex01MCApplication;
   }
   static void *newArray_Ex01MCApplication(Long_t nElements, void *p) {
      return p ? new(p) ::Ex01MCApplication[nElements] : new ::Ex01MCApplication[nElements];
   }
   // Wrapper around operator delete
   static void delete_Ex01MCApplication(void *p) {
      delete (static_cast<::Ex01MCApplication*>(p));
   }
   static void deleteArray_Ex01MCApplication(void *p) {
      delete [] (static_cast<::Ex01MCApplication*>(p));
   }
   static void destruct_Ex01MCApplication(void *p) {
      typedef ::Ex01MCApplication current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Ex01MCApplication

//______________________________________________________________________________
void Ex01DetectorConstructionOld::Streamer(TBuffer &R__b)
{
   // Stream an object of class Ex01DetectorConstructionOld.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Ex01DetectorConstructionOld::Class(),this);
   } else {
      R__b.WriteClassBuffer(Ex01DetectorConstructionOld::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Ex01DetectorConstructionOld(void *p) {
      return  p ? new(p) ::Ex01DetectorConstructionOld : new ::Ex01DetectorConstructionOld;
   }
   static void *newArray_Ex01DetectorConstructionOld(Long_t nElements, void *p) {
      return p ? new(p) ::Ex01DetectorConstructionOld[nElements] : new ::Ex01DetectorConstructionOld[nElements];
   }
   // Wrapper around operator delete
   static void delete_Ex01DetectorConstructionOld(void *p) {
      delete (static_cast<::Ex01DetectorConstructionOld*>(p));
   }
   static void deleteArray_Ex01DetectorConstructionOld(void *p) {
      delete [] (static_cast<::Ex01DetectorConstructionOld*>(p));
   }
   static void destruct_Ex01DetectorConstructionOld(void *p) {
      typedef ::Ex01DetectorConstructionOld current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Ex01DetectorConstructionOld

namespace {
  void TriggerDictionaryInitialization_libvmc_targetSim_Impl() {
    static const char* headers[] = {
"Ex01MCApplication.h",
"Ex01MCStack.h",
"Ex01DetectorConstructionOld.h",
"Ex01Particle.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/evdokimov/mysw/INSTALL/root/include",
"/home/evdokimov/mysw/INSTALL/vmc/include/vmc",
"/home/evdokimov/hyperon/HyperonMC-factory/targetSim/source/include",
"/home/evdokimov/mysw/pythia8308/include",
"/home/evdokimov/hyperon/HyperonMC-factory/targetSim/build",
"/home/evdokimov/hyperon/HyperonMC-factory/targetSim/source",
"/home/evdokimov/mysw/INSTALL/root/include/",
"/home/evdokimov/hyperon/HyperonMC-factory/targetSim/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libvmc_targetSim dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$Ex01Particle.h")))  __attribute__((annotate("$clingAutoload$Ex01MCApplication.h")))  Ex01Particle;
class __attribute__((annotate("$clingAutoload$Ex01MCStack.h")))  __attribute__((annotate("$clingAutoload$Ex01MCApplication.h")))  Ex01MCStack;
class __attribute__((annotate("$clingAutoload$Ex01MCApplication.h")))  Ex01MCApplication;
class __attribute__((annotate("$clingAutoload$Ex01DetectorConstructionOld.h")))  Ex01DetectorConstructionOld;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libvmc_targetSim dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Ex01MCApplication.h"
#include "Ex01MCStack.h"
#include "Ex01DetectorConstructionOld.h"
#include "Ex01Particle.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Ex01DetectorConstructionOld", payloadCode, "@",
"Ex01MCApplication", payloadCode, "@",
"Ex01MCStack", payloadCode, "@",
"Ex01Particle", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libvmc_targetSim",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libvmc_targetSim_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libvmc_targetSim_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libvmc_targetSim() {
  TriggerDictionaryInitialization_libvmc_targetSim_Impl();
}
