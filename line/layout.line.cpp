// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

void LAUNCHER::LINE::refresh() {
  if (showing != IDS::LINES) {
    if (unfolded) unfold(false);
    return;
  }
  GUI::set(document, IDS::LINE, GUI::Text{followed});
  GUI::set(document, IDS::LINESTATE, GUI::Text{offer.state});
  for (Whole at = 0; at < std::size(LINES); at += 1)
    GUI::set(
      document, IDS::choice(at).c_str(),
      GUI::Style{followed == LINES[at] ? STYLES::SELECTED : STYLES::ENTRY});
}
