// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

using LAUNCHER::STORE::Offer;

void LAUNCHER::STORE::survey() {
  if (surveyed) return;
  surveyed = true;
  load();
  for (Offer &offer : offers) {
    if (GET::reached(offer)) {
      offer.state = REACHING;
      continue;
    }
    if (report(offer)) verify(offer);
  }
  claim();
}

void LAUNCHER::STORE::resurvey() {
  if (installing == ISLANDS::SELECT::NONE) surveyed = false;
}
