// SPDX-License-Identifier: AGPL-3.0-or-later
#include <filesystem>

#include <common/platform/selection.hpp>

#include "../cartridge.hpp"

using LAUNCHER::STORE::Offer;

namespace LAUNCHER::STORE {
namespace {
constexpr STRING::Hot DONE = "[reach] done";
constexpr STRING::Hot QUIET = " >/dev/null 2>&1";

SHELL::OS::Process shell;
Whole at = ISLANDS::SELECT::NONE;

auto pending() -> Whole {
  namespace STORE = LAUNCHER::STORE;
  for (Whole row = 0; row < STORE::offers.size(); row += 1)
    if (
      STORE::offers[row].linked && STORE::offers[row].state == STORE::REACHING)
      return row;
  return ISLANDS::SELECT::NONE;
}

auto command(const Offer &offer) -> String {
  namespace STORE = LAUNCHER::STORE;
  const String repository = SHELL::OS::quote(offer.repository);
  return String(STORE::PROBE) + repository + STORE::HEAD + QUIET + " && (" +
         STORE::GET::clone(offer.repository, STORE::GET::staging(offer)) + ")" +
         QUIET + "; echo \"" + DONE + " $?\"";
}

void open(Whole row) {
  SHELL::OS::PROCESS::spawn(shell);
  if (!SHELL::OS::PROCESS::running(shell)) {
    LAUNCHER::STORE::offers[row].state = LAUNCHER::STORE::UNREACHED;
    return;
  }
  at = row;
  SHELL::OS::PROCESS::feed(shell, command(LAUNCHER::STORE::offers[row]));
}

auto code() -> Whole {
  const String head = String(DONE) + " ";
  for (const String &line : SHELL::OS::PROCESS::harvest(shell))
    if (line.starts_with(head))
      return STRING::number<Whole>(line.substr(head.size()));
  return ISLANDS::SELECT::NONE;
}

void close(Whole status) {
  SHELL::OS::PROCESS::stop(shell);
  const Whole row = at;
  at = ISLANDS::SELECT::NONE;
  if (row >= LAUNCHER::STORE::offers.size()) return;
  Offer &offer = LAUNCHER::STORE::offers[row];
  if (status) {
    offer.state = LAUNCHER::STORE::UNREACHED;
    return;
  }
  offer.state.clear();
  LAUNCHER::STORE::verify(offer);
  LAUNCHER::STORE::claim();
}
}  // namespace
}  // namespace LAUNCHER::STORE

#if SR_PLATFORM == SR_WINDOWS
// Without git the reach cannot probe or clone: the link's manifest is read
// off the forge, one linked offer per frame, and the release asset is
// fetched only when the offer is installed.
void LAUNCHER::STORE::reach() {
  const Whole row = pending();
  if (row == ISLANDS::SELECT::NONE) return;
  Offer &offer = offers[row];
  offer.state.clear();
  if (report(offer)) verify(offer);
  claim();
}
#else
void LAUNCHER::STORE::reach() {
  if (at == ISLANDS::SELECT::NONE) {
    const Whole row = pending();
    if (row != ISLANDS::SELECT::NONE) open(row);
    return;
  }
  if (!SHELL::OS::PROCESS::running(shell)) return close(1);
  const Whole status = code();
  if (status != ISLANDS::SELECT::NONE) close(status);
}
#endif
