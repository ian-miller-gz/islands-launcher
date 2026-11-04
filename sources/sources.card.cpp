// SPDX-License-Identifier: AGPL-3.0-or-later
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
}  // namespace
}  // namespace LAUNCHER::SOURCES::CARD

void LAUNCHER::SOURCES::CARD::seat(const String &text) {
  local = rooted(text);
  GUI::set(document, IDS::SOURCE::ORIGIN, GUI::Text{text});
  GUI::set(
    document, IDS::SOURCE::ORIGIN,
    GUI::Caret{.at = text.size(), .from = text.size()});
  GUI::edit(document, IDS::SOURCE::ORIGIN);
}

void LAUNCHER::SOURCES::CARD::ask() {
  marked.clear();
  noted.clear();
  if (GET::finding()) {
    noted = FINDING;
  } else {
    found.clear();
    sought = {};
  }
  LINE::fold();
  GUI::set(document, IDS::SOURCE::CARD, GUI::Visibility{true});
  seat(String());
}

auto LAUNCHER::SOURCES::CARD::draft() -> Source {
  Source source = {
    .repository =
      STRING::trim(String(GUI::GET::text(document, IDS::SOURCE::ORIGIN)))};
  if (rooted(source.repository)) local = true;
  source.local = local;
  if (source.local) source.repository = expanded(source.repository);
  source.leaf = STORE::GET::leaf(source.repository);
  return source;
}

void LAUNCHER::SOURCES::CARD::flip() { local = !local; }

void LAUNCHER::SOURCES::CARD::find() {
  if (GET::finding()) return;
  marked.clear();
  SOURCES::find(draft());
}

void LAUNCHER::SOURCES::CARD::mark(Whole row) {
  const Whole at = GUI::GET::first(document, IDS::SOURCE::FOUND) + row;
  if (at >= found.size()) return;
  marked = marked == found[at] ? String() : found[at];
}
