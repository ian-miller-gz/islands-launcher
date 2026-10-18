// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

namespace LAUNCHER::STORE {
namespace {
constexpr STRING::Hot UNSEATED = "";
}  // namespace
}  // namespace LAUNCHER::STORE

void LAUNCHER::STORE::ask() {
  GUI::set(document, IDS::LINK, GUI::Text{String()});
  GUI::set(document, IDS::LINK, GUI::Caret{});
  GUI::set(document, IDS::NOTED, GUI::Text{String()});
  GUI::set(document, IDS::CARD, GUI::Visibility{true});
  GUI::edit(document, IDS::LINK);
}

void LAUNCHER::STORE::drop() {
  GUI::edit(document, UNSEATED);
  GUI::set(document, IDS::CARD, GUI::Visibility{false});
}

void LAUNCHER::STORE::link() {
  noted = keep(String(GUI::GET::text(document, IDS::LINK)));
  GUI::set(document, IDS::NOTED, GUI::Text{String(noted)});
  if (String(noted) == ADDED) return drop();
  GUI::edit(document, IDS::LINK);
}
