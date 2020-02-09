/**
 * TOONTOWN OFFLINE SOFTWARE
 * Copyright (c) The Toontown Offline Team.  All rights reserved.
 *
 * Use of this software by anyone other than those of the Toontown Offline team
 * is strictly prohibited without explicit permission from the Toontown Offline team.
 *
 * @file config_toontown.cxx
 * @author jwcotejr
 * @date 2019-01-19
 */

#include "config_toontown.h"
#include "DNASuitPath.h"
#include "DNASuitEdge.h"
#include "DNASuitPoint.h"
#include "DNAAnimBuilding.h"
#include "DNAAnimProp.h"
#include "DNABattleCell.h"
#include "DNACornice.h"
#include "DNAData.h"
#include "DNADoor.h"
#include "DNAFlatBuilding.h"
#include "DNAFlatDoor.h"
#include "DNAGroup.h"
#include "DNAInteractiveProp.h"
#include "DNALandmarkBuilding.h"
#include "DNANode.h"
#include "DNAProp.h"
#include "DNASign.h"
#include "DNASignBaseline.h"
#include "DNASignGraphic.h"
#include "DNAStreet.h"
#include "DNAVisGroup.h"
#include "DNAWall.h"
#include "DNAWindows.h"

#include "dconfig.h"

#if !defined(CPPPARSER) && !defined(LINK_ALL_STATIC) && !defined(BUILDING_DNA)
  #error Buildsystem error: BUILDING_DNA not defined
#endif

Configure(config_toontown);
NotifyCategoryDef(dna, "");

ConfigureFn(config_toontown) {
  init_libtoontown();
}

/**
 * Initializes the library.  This must be called at least once before any of
 * the functions or classes in this library can be used.  Normally it will be
 * called by the static initializers and need not be called explicitly, but
 * special cases exist.
 */
void
init_libtoontown() {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;

  DNASuitPath::init_type();
  DNASuitEdge::init_type();
  DNASuitPoint::init_type();
  DNAAnimBuilding::init_type();
  DNAAnimProp::init_type();
  DNABattleCell::init_type();
  DNACornice::init_type();
  DNAData::init_type();
  DNADoor::init_type();
  DNAFlatBuilding::init_type();
  DNAFlatDoor::init_type();
  DNAGroup::init_type();
  DNAInteractiveProp::init_type();
  DNALandmarkBuilding::init_type();
  DNANode::init_type();
  DNAProp::init_type();
  DNASign::init_type();
  DNASignBaseline::init_type();
  DNASignGraphic::init_type();
  DNAStreet::init_type();
  DNAVisGroup::init_type();
  DNAWall::init_type();
  DNAWindows::init_type();
}
