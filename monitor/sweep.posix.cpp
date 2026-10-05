// SPDX-License-Identifier: AGPL-3.0-or-later
#include <signal.h>

#include "monitor.internal.hpp"

// The roster probe: a null signal reaches a live process and fails on a
// reaped one.
void MONITOR::sweep() {
  for (auto &entry : roster) entry.running = kill(entry.pid, 0) == 0;
}
