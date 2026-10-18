// SPDX-License-Identifier: AGPL-3.0-or-later
#include "monitor.internal.hpp"

#define NOMINMAX
#define NOUSER
#include <windows.h>

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
