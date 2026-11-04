// SPDX-License-Identifier: AGPL-3.0-or-later
#include <filesystem>

#include "../cartridge.hpp"

namespace fs = std::filesystem;

namespace LAUNCHER::UPDATE {
namespace {
auto shown(const String &leaf) -> Flag {
  namespace STORE = LAUNCHER::STORE;
  if (!STORE::GET::running() || STORE::installing >= STORE::offers.size())
    return false;
  return !leaf.empty() && STORE::offers[STORE::installing].leaf == leaf;
}

void gauge(Flag filling, const String &failure) {
  namespace STORE = LAUNCHER::STORE;
  GUI::set(document, IDS::PPROGRESS, GUI::Visibility{filling});
  GUI::set(document, IDS::PSTATE, GUI::Visibility{filling || !failure.empty()});
  if (!filling) {
    GUI::set(document, IDS::PSTATE, GUI::Text{failure});
    return;
  }
  GUI::set(document, IDS::PPROGRESS, GUI::Fraction{STORE::GET::progress()});
  GUI::set(
    document, IDS::PSTATE, GUI::Text{STORE::offers[STORE::installing].state});
}
}  // namespace
}  // namespace LAUNCHER::UPDATE

auto LAUNCHER::GET::leaf() -> String {
  const fs::path chosen = LAUNCHER::chosen;
  if (chosen.parent_path() != fs::path(LAUNCHER::STORE::GET::home())) return {};
  return chosen.filename().string();
}

void LAUNCHER::UPDATE::refresh() {
  const String leaf = GET::leaf();
  const Flag standing = STORE::GET::update(leaf) && !STORE::GET::running();
  GUI::set(document, IDS::PUPDATE, GUI::Visibility{standing});
  gauge(shown(leaf), STORE::GET::failure(leaf));
}

void LAUNCHER::UPDATE::run() { STORE::update(GET::leaf()); }
