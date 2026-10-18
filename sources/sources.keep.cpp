// SPDX-License-Identifier: AGPL-3.0-or-later
#include <string>

#include "../cartridge.hpp"

using LAUNCHER::SOURCES::Source;

void LAUNCHER::SOURCES::CARD::add() {
  if (GET::finding()) return;
  if (marks.empty() || sought.leaf != leaf) {
    noted = NONE;
    return;
  }
  Whole kept = 0;
  Flag full = false;
  for (const String &branch : marks) {
    Source source = sought;
    source.branch = branch;
    const String word = keep(source);
    kept += word == KEPT ? 1 : 0;
    full = full || word == FULL;
  }
  marks.clear();
  std::erase_if(
    found, [](const String &name) { return GET::taken(leaf, name); });
  noted = full ? String(FULL) : String(KEPT) + " " + std::to_string(kept);
}

void LAUNCHER::SOURCES::CARD::remove() {
  const Vector<const Source *> named = GET::named(leaf);
  const Whole at = GUI::GET::cursor(document, IDS::SOURCE::LIST);
  if (at >= named.size() || STORE::GET::running()) return;
  if (STORE::GET::reaching()) {
    noted = STORE::REACHING;
    return;
  }
  const String branch = named[at]->branch;
  const Flag current = LINE::GET::line(leaf) == branch;
  SOURCES::drop(leaf, branch);
  noted.clear();
  if (!current) return;
  STORE::resurvey();
  STORE::survey();
}
