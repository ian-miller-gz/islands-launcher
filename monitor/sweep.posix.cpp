// SPDX-License-Identifier: AGPL-3.0-or-later
#include <signal.h>

#include "monitor.internal.hpp"

void MONITOR::sweep() {
  for (auto &entry : roster) entry.running = kill(entry.pid, 0) == 0;
}
