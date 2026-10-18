// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

void LAUNCHER::poll() {
  reap();
  WATCH::poll();
  STORE::poll();
  STORE::reach();
  SOURCES::poll();
}
