// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

namespace LAUNCHER::LINE {
namespace {
constexpr STRING::Hot ROW[] = {
  LAUNCHER::IDS::CAPTION, LAUNCHER::IDS::PLINE, LAUNCHER::IDS::SOURCES};
}  // namespace
}  // namespace LAUNCHER::LINE

void LAUNCHER::LINE::options() {
  const String leaf = LAUNCHER::GET::leaf();
  for (STRING::Hot id : ROW)
    GUI::set(document, id, GUI::Visibility{!leaf.empty()});
  if (leaf.empty()) GUI::set(document, IDS::PLINES, GUI::Visibility{false});
  GUI::set(
    document, IDS::PLINE, GUI::Text{GET::word(IDS::PLINE, GET::line(leaf))});
  dress(IDS::PLINES, IDS::pline, GET::lines(leaf));
}

void LAUNCHER::LINE::choose(Whole at) {
  fold();
  const String leaf = LAUNCHER::GET::leaf();
  const Vector<String> named = GET::lines(leaf);
  if (leaf.empty() || at >= named.size() || STORE::GET::running()) return;
  if (STORE::GET::reaching()) {
    refused = IDS::PLINE;
    return;
  }
  set(leaf, named[at]);
  STORE::resurvey();
  STORE::survey();
}
