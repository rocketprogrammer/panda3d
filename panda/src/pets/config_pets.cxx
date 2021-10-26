// Filename: config_pets.cxx
// Created by:  dcranall (15Jul04)
//
////////////////////////////////////////////////////////////////////

#include "config_pets.h"
#include "cPetChase.h"
#include "cPetFlee.h"

#include "dconfig.h"

#if !defined(CPPPARSER) && !defined(LINK_ALL_STATIC) && !defined(BUILDING_TOONTOWN)
  #error Buildsystem error: BUILDING_TOONTOWN not defined
#endif

Configure(config_pets);
NotifyCategoryDef(pets, "");

ConfigureFn(config_pets) {
  init_libpets();
}

////////////////////////////////////////////////////////////////////
//     Function: init_libpets
//  Description: Initializes the library.  This must be called at
//               least once before any of the functions or classes in
//               this library can be used.  Normally it will be
//               called by the static initializers and need not be
//               called explicitly, but special cases exist.
////////////////////////////////////////////////////////////////////
void
init_libpets() {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;

  CPetChase::init_type();
  CPetFlee::init_type();
}
