// SPDX-License-Identifier: AGPL-3.0-or-later
#include <filesystem>

#include "../cartridge.hpp"

using LAUNCHER::STORE::Offer;

namespace LAUNCHER::STORE {
namespace {
constexpr STRING::Hot DONE = "[reach] done";
constexpr STRING::Hot SWEEP = "rm -rf ";
constexpr STRING::Hot CLONE = "git clone --depth 1 ";
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
  const String home = SHELL::OS::quote(STORE::GET::staging(offer));
  return String(STORE::PROBE) + repository + STORE::HEAD + QUIET + " && " +
         SWEEP + home + " && " + CLONE + repository + " " + home + QUIET +
         "; echo \"" + DONE + " $?\"";
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
}
}  // namespace
}  // namespace LAUNCHER::STORE

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
