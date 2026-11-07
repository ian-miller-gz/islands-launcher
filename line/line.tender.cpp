// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

namespace LAUNCHER::LINE {
namespace {
constexpr Char SPACE = ' ';

void shown(Flag on) {
  GUI::set(document, IDS::OFFER, GUI::Visibility{on});
}

void renew() {
  shown(false);
  if (STORE::GET::running()) return;
  STORE::run(offer, GET::renewals());
}

void skip() {
  shown(false);
  skipped = offered;
  write();
  offer.state.clear();
}
}  // namespace
}  // namespace LAUNCHER::LINE

void LAUNCHER::LINE::tender(const String &current, const String &wanted) {
  offered = wanted;
  GUI::set(document, IDS::OFFERLINE, GUI::Text{followed + SPACE + wanted});
  GUI::set(document, IDS::OFFERNOTE, GUI::Text{String(INSTALLED) + SPACE + current});
  shown(true);
}

auto LAUNCHER::LINE::answered(const String &id) -> Flag {
  if (id == IDS::RENEW) return renew(), true;
  if (id == IDS::LATER) return shown(false), true;
  if (id == IDS::SKIP) return skip(), true;
  return false;
}
