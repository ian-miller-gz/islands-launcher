// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

namespace LAUNCHER::STORE {
namespace {
constexpr Whole DETAILED = 22;

constexpr STRING::Hot COLUMNS[] = {
  LAUNCHER::IDS::CALLED,  LAUNCHER::IDS::VERSION,  LAUNCHER::IDS::DETAIL,
  LAUNCHER::IDS::INSTALL, LAUNCHER::IDS::STANDING, LAUNCHER::IDS::PROGRESS,
  LAUNCHER::IDS::UPDATE};

void show(Whole at, Flag standing) {
  for (STRING::Hot column : COLUMNS)
    GUI::set(
      LAUNCHER::document, LAUNCHER::IDS::cell(column, at).c_str(),
      GUI::Visibility{standing});
}

void cell(STRING::Hot column, Whole at, const String &text) {
  GUI::set(
    LAUNCHER::document, LAUNCHER::IDS::cell(column, at).c_str(),
    GUI::Text{text});
}

void dress(STRING::Hot column, Whole at, STRING::Hot style) {
  GUI::set(
    LAUNCHER::document, LAUNCHER::IDS::cell(column, at).c_str(),
    GUI::Style{style});
}

auto face(Flag settled) -> STRING::Hot {
  namespace STORE = LAUNCHER::STORE;
  if (settled) return STORE::CROSS;
  return STORE::installing == ISLANDS::SELECT::NONE ? STORE::OFFERED
                                                    : STORE::BUSY;
}

void gauge(Whole at) {
  namespace IDS = LAUNCHER::IDS;
  const Flag filling = LAUNCHER::STORE::installing == at;
  GUI::set(
    LAUNCHER::document, IDS::cell(IDS::PROGRESS, at).c_str(),
    GUI::Visibility{filling});
  if (filling)
    GUI::set(
      LAUNCHER::document, IDS::cell(IDS::PROGRESS, at).c_str(),
      GUI::Fraction{LAUNCHER::STORE::GET::progress()});
}

void raise(Whole at) {
  namespace IDS = LAUNCHER::IDS;
  namespace STORE = LAUNCHER::STORE;
  GUI::set(
    LAUNCHER::document, IDS::cell(IDS::UPDATE, at).c_str(),
    GUI::Visibility{STORE::GET::update(at) && !STORE::GET::running()});
}

void write(Whole at, const LAUNCHER::STORE::Offer &offer) {
  namespace IDS = LAUNCHER::IDS;
  namespace STORE = LAUNCHER::STORE;
  namespace STYLES = LAUNCHER::STYLES;
  const Flag settled = offer.installed && STORE::installing != at;
  cell(IDS::CALLED, at, LAUNCHER::GET::plain(offer.name));
  cell(IDS::VERSION, at, LAUNCHER::GET::plain(offer.release));
  cell(IDS::DETAIL, at, LAUNCHER::GET::shortened(offer.description, DETAILED));
  cell(IDS::STANDING, at, settled ? String(STORE::MARK) : offer.state);
  dress(IDS::STANDING, at, settled ? STYLES::MARK : STYLES::MUTED);
  cell(IDS::INSTALL, at, face(settled));
  dress(IDS::INSTALL, at, settled ? STYLES::DROP : STYLES::PLACE);
  gauge(at);
  raise(at);
}
}  // namespace
}  // namespace LAUNCHER::STORE

void LAUNCHER::STORE::refresh() {
  for (Offer &offer : offers) offer.held = held(offer);
  if (showing != IDS::OFFERS) return;
  GUI::set(document, IDS::LINKED, GUI::Text{String(noted)});
  for (Whole at = 0; at < ROWS; at += 1) {
    show(at, at < offers.size());
    if (at >= offers.size()) continue;
    installed(offers[at]);
    write(at, offers[at]);
  }
}
