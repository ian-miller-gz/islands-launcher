// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

namespace LAUNCHER::LINE {
namespace {
struct Click {
  STRING::Hot id;
  void (*run)();
};

constexpr Click CLICKS[] = {
  {LAUNCHER::IDS::LINE, [] { unfold(LAUNCHER::IDS::LINE); }},
  {LAUNCHER::IDS::SLINE, [] { unfold(LAUNCHER::IDS::SLINE); }},
  {LAUNCHER::IDS::PLINE,
   [] { unfold(LAUNCHER::IDS::PLINES, LAUNCHER::IDS::PLINE); }},
  {LAUNCHER::IDS::SOURCES,
   [] { LAUNCHER::SOURCES::CARD::ask(LAUNCHER::GET::leaf()); }},
  {LAUNCHER::IDS::SADD,
   [] { LAUNCHER::SOURCES::CARD::ask(LAUNCHER::LINE::LAUNCHER); }},
  {LAUNCHER::IDS::SETTLE,
   [] {
     GUI::set(LAUNCHER::document, LAUNCHER::IDS::SETUP, GUI::Visibility{false});
     fold();
   }},
  {LAUNCHER::IDS::SOURCE::SIDE, LAUNCHER::SOURCES::CARD::flip},
  {LAUNCHER::IDS::SOURCE::FIND, LAUNCHER::SOURCES::CARD::find},
  {LAUNCHER::IDS::SOURCE::ADD, LAUNCHER::SOURCES::CARD::add},
  {LAUNCHER::IDS::SOURCE::DROP, LAUNCHER::SOURCES::CARD::remove},
  {LAUNCHER::IDS::SOURCE::CLOSE, LAUNCHER::SOURCES::CARD::drop}};

void add(Whole row) {
  const auto &offers = LAUNCHER::STORE::offers;
  if (row < offers.size()) LAUNCHER::SOURCES::CARD::ask(offers[row].leaf);
}

auto bank(
  const String &id, auto (*name)(Whole)->String, Whole count,
  void (*run)(Whole)) -> Flag {
  for (Whole at = 0; at < count; at += 1) {
    if (id != name(at)) continue;
    run(at);
    return true;
  }
  return false;
}
}  // namespace
}  // namespace LAUNCHER::LINE

auto LAUNCHER::LINE::click(const String &id) -> Flag {
  for (const Click &click : CLICKS) {
    if (id != click.id) continue;
    click.run();
    return true;
  }
  return bank(id, IDS::sline, ENTRIES, pick) ||
         bank(id, IDS::pline, ENTRIES, choose) ||
         bank(id, IDS::rline, STORE::ROWS, unfold) ||
         bank(id, IDS::radd, STORE::ROWS, add) ||
         bank(id, IDS::rentry, ENTRIES, take) ||
         bank(
           id, IDS::SOURCE::row, GUI::GET::window(document, IDS::SOURCE::FOUND),
           SOURCES::CARD::mark);
}
