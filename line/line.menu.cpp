// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

namespace LAUNCHER::LINE {
namespace {
constexpr Float PITCH = 34;
constexpr Float RIM = 4;
constexpr STRING::Hot CARDS[] = {
  LAUNCHER::IDS::SLINES, LAUNCHER::IDS::RLINES, LAUNCHER::IDS::PLINES};
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
  dress(IDS::SLINES, IDS::sline, GET::lines(LAUNCHER));
  unfold(IDS::SLINES, door);
}

void LAUNCHER::LINE::dress(
  STRING::Hot card, auto (*entry)(Whole)->String, const Vector<String> &words) {
  for (Whole at = 0; at < ENTRIES; at += 1) {
    const String id = entry(at);
    const Flag shown = at < words.size();
    GUI::set(document, id.c_str(), GUI::Visibility{shown});
    if (shown) GUI::set(document, id.c_str(), GUI::Text{words[at]});
  }
  auto extent = GUI::GET::extent(document, card);
  extent.h =
    RIM + PITCH * static_cast<Float>(std::min<Whole>(words.size(), ENTRIES));
  GUI::set(document, card, extent);
}

void LAUNCHER::LINE::pick(Whole at) {
  const String door = GUI::GET::seat(document, IDS::SLINES).beside;
  fold();
  const Vector<String> words = GET::lines(LAUNCHER);
  if (at >= words.size() || STORE::GET::running()) return;
  if (STORE::GET::reaching()) {
    refused = door;
    return;
  }
  followed = words[at];
  write();
  STORE::resurvey();
  if (showing == IDS::OFFERS) STORE::survey();
  follow();
}
