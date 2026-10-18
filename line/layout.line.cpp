// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

void LAUNCHER::LINE::refresh() {
  if (showing != IDS::LINES) return;
  GUI::set(document, IDS::LINE, GUI::Text{followed});
  GUI::set(document, IDS::LINESTATE, GUI::Text{offer.state});
}
