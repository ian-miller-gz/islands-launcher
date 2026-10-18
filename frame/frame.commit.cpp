// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

void LAUNCHER::commit(const String &id) {
  if (id == IDS::LINK) return STORE::link();
  if (id == IDS::SOURCE::ORIGIN) return SOURCES::CARD::find();
}
