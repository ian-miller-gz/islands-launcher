// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <cstdlib>

#include "../cartridge.hpp"

using LAUNCHER::SOURCES::Source;

namespace LAUNCHER::SOURCES::CARD {
namespace {
constexpr Char ROOT = '/';
constexpr STRING::Hot TILDE = "~";
constexpr STRING::Hot NEST = "~/";
constexpr STRING::Hot HOME = "HOME";

auto rooted(const String &path) -> Flag {
  return path.starts_with(ROOT) || path.starts_with(TILDE);
}

auto expanded(const String &path) -> String {
  const STRING::Hot home = std::getenv(HOME);
  if (home == nullptr || (path != TILDE && !path.starts_with(NEST)))
    return path;
  return String(home) + path.substr(String(TILDE).size());
}

void seat(const String &text) {
  namespace SOURCE = LAUNCHER::IDS::SOURCE;
  GUI::set(LAUNCHER::document, SOURCE::ORIGIN, GUI::Text{text});
  GUI::set(
    LAUNCHER::document, SOURCE::ORIGIN,
    GUI::Caret{.at = text.size(), .from = text.size()});
  GUI::edit(LAUNCHER::document, SOURCE::ORIGIN);
}
}  // namespace
}  // namespace LAUNCHER::SOURCES::CARD

void LAUNCHER::SOURCES::CARD::ask(const String &leaf) {
  if (leaf.empty()) return;
  CARD::leaf = leaf;
  const String listed = GET::listed(leaf);
  local = rooted(listed);
  marks.clear();
  noted.clear();
  if (GET::finding()) {
    noted = FINDING;
  } else {
    found.clear();
    sought = {};
  }
  LINE::fold();
  GUI::set(document, IDS::SOURCE::CARD, GUI::Visibility{true});
  seat(listed);
}

void LAUNCHER::SOURCES::CARD::flip() { local = !local; }

void LAUNCHER::SOURCES::CARD::find() {
  if (GET::finding()) return;
  Source draft = {
    .leaf = leaf,
    .repository =
      STRING::trim(String(GUI::GET::text(document, IDS::SOURCE::ORIGIN)))};
  if (rooted(draft.repository)) local = true;
  draft.local = local;
  if (draft.local) draft.repository = expanded(draft.repository);
  marks.clear();
  SOURCES::find(draft);
}

void LAUNCHER::SOURCES::CARD::mark(Whole seat) {
  const Whole at = GUI::GET::first(document, IDS::SOURCE::FOUND) + seat;
  if (at >= found.size()) return;
  const auto standing = std::ranges::find(marks, found[at]);
  if (standing == marks.end())
    marks.push_back(found[at]);
  else
    marks.erase(standing);
}
