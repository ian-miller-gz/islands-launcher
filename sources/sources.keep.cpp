// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

using LAUNCHER::SOURCES::Source;

namespace LAUNCHER::SOURCES::CARD {
namespace {
auto follow(const Source &draft) -> String {
  namespace SOURCES = LAUNCHER::SOURCES;
  Source source = draft;
  source.branch = SOURCES::marked;
  const String kept = SOURCES::keep(source);
  if (kept != SOURCES::KEPT) return kept;
  LAUNCHER::LINE::set(source.leaf, source.branch);
  SOURCES::marked.clear();
  std::erase_if(SOURCES::found, [&](const String &name) {
    return SOURCES::GET::taken(source.leaf, name);
  });
  return kept;
}

auto known(const Source &draft) -> String {
  namespace STORE = LAUNCHER::STORE;
  if (LAUNCHER::SOURCES::marked.empty()) return STORE::LISTED;
  if (STORE::GET::running()) return STORE::BUSY;
  if (STORE::GET::reaching()) return STORE::REACHING;
  const String kept = follow(draft);
  if (kept != LAUNCHER::SOURCES::KEPT) return kept;
  STORE::resurvey();
  STORE::survey();
  return kept;
}

auto added(const Source &draft) -> String {
  namespace SOURCES = LAUNCHER::SOURCES;
  namespace STORE = LAUNCHER::STORE;
  if (
    !SOURCES::marked.empty() && draft.repository != SOURCES::sought.repository)
    return SOURCES::NONE;
  if (STORE::offers.empty()) STORE::load();
  const String row = STORE::keep(draft.repository);
  if (row == STORE::LISTED) return known(draft);
  if (row != STORE::ADDED || SOURCES::marked.empty()) return row;
  const String kept = follow(draft);
  return kept == SOURCES::KEPT ? row : kept;
}
}  // namespace
}  // namespace LAUNCHER::SOURCES::CARD

void LAUNCHER::SOURCES::CARD::add() {
  if (GET::finding()) return;
  noted = added(draft());
  GUI::edit(document, IDS::SOURCE::ORIGIN);
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
