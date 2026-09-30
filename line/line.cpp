// SPDX-License-Identifier: AGPL-3.0-or-later
#include <common.hpp>
#include <filesystem>

#include "../cartridge.hpp"

namespace fs = std::filesystem;

namespace LAUNCHER::LINE {
namespace {
auto file() -> String {
  const fs::path path = fs::path(COMMON::STATE) / LAUNCHER::LINE::PATH;
  std::error_code slip;
  fs::create_directories(path.parent_path(), slip);
  return path.string();
}
}  // namespace
}  // namespace LAUNCHER::LINE

auto LAUNCHER::LINE::GET::placed(const String &word) -> Whole {
  for (Whole at = 0; at < std::size(LINES); at += 1)
    if (word == LINES[at]) return at;
  return ISLANDS::SELECT::NONE;
}

void LAUNCHER::LINE::read() {
  followed = STABLE;
  IO::STREAMS::Input in(file());
  String line, key, value;
  while (std::getline(in, line)) {
    const String text = LAUNCHER::GET::bare(line);
    if (!STRING::pair(text, key, value)) continue;
    if (key == KEY && GET::placed(value) != ISLANDS::SELECT::NONE)
      followed = value;
  }
  offer = {.repository = SEAT, .leaf = SEAT};
  if (!GET::cloned()) offer.state = PINNED;
}

void LAUNCHER::LINE::write() {
  IO::STREAMS::Output out(file());
  if (!out) return;
  out << KEY << ": " << followed << "\n";
}

void LAUNCHER::LINE::unfold(Flag on) {
  unfolded = on;
  GUI::set(document, IDS::CHOICES, GUI::Visibility{on});
}

void LAUNCHER::LINE::pick(Whole at) {
  unfold(false);
  if (at >= std::size(LINES) || STORE::GET::running()) return;
  if (followed == LINES[at]) return;
  followed = LINES[at];
  write();
  follow();
}
