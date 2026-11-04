// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

using LAUNCHER::SOURCES::Source;

namespace LAUNCHER::SOURCES::CARD {
namespace {
constexpr Whole SPAN = 40;
constexpr STRING::Hot GAP = "  ";
constexpr STRING::Hot UNSEATED = "";

auto row(const Source &source) -> String {
  namespace SOURCES = LAUNCHER::SOURCES;
  return LAUNCHER::GET::plain(source.branch) + GAP +
         LAUNCHER::GET::shortened(source.repository, SPAN) + GAP +
         (source.local ? SOURCES::LOCAL : SOURCES::REMOTE);
}

void fill(STRING::Hot list, const Vector<String> &texts) {
  GUI::set(LAUNCHER::document, list, GUI::Rows{texts.size()});
  const Whole first = GUI::GET::first(LAUNCHER::document, list);
  const Whole window = GUI::GET::window(LAUNCHER::document, list);
  for (Whole at = 0; at < window && first + at < texts.size(); at += 1)
    GUI::set(
      LAUNCHER::document, LAUNCHER::IDS::item(list, at).c_str(),
      GUI::Text{LAUNCHER::GET::plain(texts[first + at])});
}

void list(const String &leaf) {
  Vector<String> rows;
  for (const Source *source : LAUNCHER::SOURCES::GET::named(leaf))
    rows.push_back(row(*source));
  fill(LAUNCHER::IDS::SOURCE::LIST, rows);
}

void branches() {
  namespace SOURCE = LAUNCHER::IDS::SOURCE;
  namespace SOURCES = LAUNCHER::SOURCES;
  fill(SOURCE::FOUND, SOURCES::found);
  const Whole first = GUI::GET::first(LAUNCHER::document, SOURCE::FOUND);
  const Whole window = GUI::GET::window(LAUNCHER::document, SOURCE::FOUND);
  for (Whole at = 0; at < window && first + at < SOURCES::found.size();
       at += 1) {
    const String &name = SOURCES::found[first + at];
    GUI::set(
      LAUNCHER::document, SOURCE::row(at).c_str(),
      GUI::Style{
        name == SOURCES::marked ? LAUNCHER::STYLES::SELECTED
                                : LAUNCHER::STYLES::POST});
  }
}
}  // namespace
}  // namespace LAUNCHER::SOURCES::CARD

void LAUNCHER::SOURCES::CARD::refresh() {
  if (!GUI::GET::visibility(document, IDS::SOURCE::CARD)) return;
  leaf = draft().leaf;
  GUI::set(document, IDS::SOURCE::TITLE, GUI::Text{String(TITLE)});
  list(leaf);
  branches();
  GUI::set(
    document, IDS::SOURCE::SIDE, GUI::Text{String(local ? LOCAL : REMOTE)});
  GUI::set(document, IDS::SOURCE::STATE, GUI::Text{noted});
}

void LAUNCHER::SOURCES::CARD::drop() {
  GUI::edit(document, UNSEATED);
  GUI::set(document, IDS::SOURCE::CARD, GUI::Visibility{false});
}
