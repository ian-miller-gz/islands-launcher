// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

void LAUNCHER::LINE::refresh() {
  if (!STORE::GET::reaching()) refused.clear();
  if (showing != IDS::LINES) return;
  GUI::set(document, IDS::LINE, GUI::Text{GET::word(IDS::LINE, followed)});
  GUI::set(document, IDS::LINESTATE, GUI::Text{offer.state});
}
