// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

namespace LAUNCHER::LINE {
namespace {
constexpr Float PITCH = 34;
constexpr Float RIM = 4;
constexpr STRING::Hot CARDS[] = {
  LAUNCHER::IDS::RELEASE::CARD, LAUNCHER::IDS::RLINES, LAUNCHER::IDS::PLINES};
}  // namespace
}  // namespace LAUNCHER::LINE

void LAUNCHER::LINE::fold() {
  for (STRING::Hot card : CARDS)
    GUI::set(document, card, GUI::Visibility{false});
}

void LAUNCHER::LINE::unfold(STRING::Hot card, STRING::Hot door) {
  const Flag on = GUI::GET::visibility(document, card) &&
                  GUI::GET::seat(document, card).beside == door;
  fold();
  if (on) return;
  GUI::set(document, card, GUI::Seat{.beside = door});
  GUI::set(document, card, GUI::Visibility{true});
}

void LAUNCHER::LINE::unfold(STRING::Hot door) {
  dress(IDS::RELEASE::CARD, IDS::RELEASE::entry, GET::lines(LAUNCHER));
  unfold(IDS::RELEASE::CARD, door);
}

void LAUNCHER::LINE::dress(
  STRING::Hot card, auto (*entry)(Whole)->String, const Vector<String> &names) {
  for (Whole at = 0; at < ENTRIES; at += 1) {
    const String id = entry(at);
    const Flag shown = at < names.size();
    GUI::set(document, id.c_str(), GUI::Visibility{shown});
    if (shown) GUI::set(document, id.c_str(), GUI::Text{names[at]});
  }
  auto extent = GUI::GET::extent(document, card);
  extent.h =
    RIM + PITCH * static_cast<Float>(std::min<Whole>(names.size(), ENTRIES));
  GUI::set(document, card, extent);
}

void LAUNCHER::LINE::pick(Whole at) {
  const String door = GUI::GET::seat(document, IDS::RELEASE::CARD).beside;
  fold();
  const Vector<String> names = GET::lines(LAUNCHER);
  if (at >= names.size() || STORE::GET::running()) return;
  if (STORE::GET::reaching()) {
    refused = door;
    return;
  }
  followed = names[at];
  write();
  STORE::resurvey();
  if (showing == IDS::OFFERS) STORE::survey();
  follow();
}
