// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

void LAUNCHER::LINE::rows() {
  const auto &offers = STORE::offers;
  for (Whole at = 0; at < std::min<Whole>(offers.size(), STORE::ROWS);
       at += 1) {
    const String id = IDS::rline(at);
    GUI::set(
      document, id.c_str(),
      GUI::Text{GET::word(id.c_str(), GET::line(offers[at].leaf))});
  }
}

void LAUNCHER::LINE::unfold(Whole row) {
  if (row >= STORE::offers.size()) return;
  const String door = IDS::rline(row);
  raised = row;
  dress(IDS::RLINES, IDS::rentry, GET::lines(STORE::offers[row].leaf));
  unfold(IDS::RLINES, door.c_str());
}

void LAUNCHER::LINE::take(Whole at) {
  fold();
  if (raised >= STORE::offers.size()) return;
  const String leaf = STORE::offers[raised].leaf;
  const Vector<String> named = GET::lines(leaf);
  if (at >= named.size() || STORE::GET::running()) return;
  if (STORE::GET::reaching()) {
    refused = IDS::rline(raised);
    return;
  }
  set(leaf, named[at]);
  STORE::resurvey();
  STORE::survey();
}
