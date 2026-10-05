// SPDX-License-Identifier: AGPL-3.0-or-later
#include "monitor.internal.hpp"

#define NOMINMAX
#define NOUSER
#include <windows.h>

// The roster probe: a process handle that still waits is a live process;
// one that cannot be opened, or that is signalled, has ended. The exit
// code is not consulted, since a process may exit with the code that
// spells "still active".
static auto running(pid_t pid) -> Flag {
  HANDLE process = OpenProcess(
    SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE,
    static_cast<DWORD>(pid));
  if (process == nullptr) return false;
  const Flag alive = WaitForSingleObject(process, 0) == WAIT_TIMEOUT;
  CloseHandle(process);
  return alive;
}

void MONITOR::sweep() {
  for (auto &entry : roster) entry.running = running(entry.pid);
}
