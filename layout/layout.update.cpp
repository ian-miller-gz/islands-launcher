// SPDX-License-Identifier: AGPL-3.0-or-later
#include <filesystem>

#include "../cartridge.hpp"

namespace fs = std::filesystem;

static auto leaf() -> String {
  const fs::path chosen = LAUNCHER::chosen;
  if (chosen.parent_path() != fs::path(LAUNCHER::STORE::GET::home())) return {};
  return chosen.filename().string();
}

void LAUNCHER::UPDATE::refresh() {
  const Flag standing = STORE::GET::update(::leaf()) && !STORE::GET::running();
  GUI::set(document, IDS::PUPDATE, GUI::Visibility{standing});
}

void LAUNCHER::UPDATE::run() { STORE::update(::leaf()); }
