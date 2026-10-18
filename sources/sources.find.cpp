// SPDX-License-Identifier: AGPL-3.0-or-later
#include <string>

#include <common/platform/selection.hpp>

#include "../cartridge.hpp"

using LAUNCHER::SOURCES::Source;

namespace LAUNCHER::SOURCES {
namespace {
constexpr STRING::Hot DONE = "[find] done";
constexpr STRING::Hot TIMEOUT = "timeout ";
constexpr Whole LIMIT = 30;
constexpr STRING::Hot HEADS = "git ls-remote --heads ";
constexpr STRING::Hot FOLDER = "test -d ";
constexpr Whole PASSED = 0;
constexpr Whole BROKEN = 1;

SHELL::OS::Process shell;
Vector<String> names;
Flag active = false;

auto command(const Source &source) -> String {
  const String from = SHELL::OS::quote(source.repository);
  const String folder = source.local ? String(FOLDER) + from + " && " : "";
  namespace STORE = LAUNCHER::STORE;
  return "(" + folder + STORE::PROMPTLESS + TIMEOUT + std::to_string(LIMIT) +
         " " + HEADS + from + ")" + STORE::MUTE + "; echo \"" + DONE + " $?\"";
}

auto code() -> Whole {
  const String head = String(DONE) + " ";
  const String ref = LAUNCHER::STORE::REF;
  for (const String &line : SHELL::OS::PROCESS::harvest(shell)) {
    if (line.starts_with(head))
      return STRING::number<Whole>(line.substr(head.size()));
    const auto cut = line.find(ref);
    if (cut != String::npos)
      names.push_back(STRING::trim(line.substr(cut + ref.size())));
  }
  return ISLANDS::SELECT::NONE;
}

auto answer(Whole status) -> String {
  namespace SOURCES = LAUNCHER::SOURCES;
  if (status != PASSED) return SOURCES::UNREACHED;
  for (const String &name : names)
    if (!SOURCES::GET::taken(SOURCES::sought.leaf, name))
      SOURCES::found.push_back(name);
  if (SOURCES::found.empty()) return SOURCES::NONE;
  return String(SOURCES::FOUND) + " " + std::to_string(SOURCES::found.size());
}

void close(Whole status) {
  SHELL::OS::PROCESS::stop(shell);
  active = false;
  LAUNCHER::SOURCES::noted = answer(status);
}
}  // namespace
}  // namespace LAUNCHER::SOURCES

auto LAUNCHER::SOURCES::GET::finding() -> Flag { return active; }

void LAUNCHER::SOURCES::poll() {
  if (!active) return;
  if (!SHELL::OS::PROCESS::running(shell)) return close(BROKEN);
  const Whole status = code();
  if (status != ISLANDS::SELECT::NONE) close(status);
}

#if SR_PLATFORM == SR_WINDOWS
void LAUNCHER::SOURCES::find(const Source &source) {
  found.clear();
  noted = source.repository.empty() ? EMPTY : UNREACHED;
}
#else
void LAUNCHER::SOURCES::find(const Source &source) {
  if (active) return;
  found.clear();
  if (source.repository.empty()) {
    noted = EMPTY;
    return;
  }
  SHELL::OS::PROCESS::spawn(shell);
  if (!SHELL::OS::PROCESS::running(shell)) {
    noted = UNREACHED;
    return;
  }
  sought = source;
  names.clear();
  active = true;
  noted = FINDING;
  SHELL::OS::PROCESS::feed(shell, command(source));
}
#endif
