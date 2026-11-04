// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <cctype>

#include "../cartridge.hpp"

using LAUNCHER::STORE::Offer;

namespace LAUNCHER::STORE {
namespace {
constexpr STRING::Hot NEEDS = "needs engine ";
constexpr Char DOT = '.';
constexpr Whole PARTS = 3;

auto digits(const String &part) -> Flag {
  return !part.empty() && std::ranges::all_of(part, [](Char glyph) {
    return std::isdigit(static_cast<unsigned char>(glyph)) != 0;
  });
}

auto numbers(const String &text) -> Vector<Whole> {
  Vector<Whole> parts;
  for (const String &part : STRING::split(STRING::trim(text), DOT)) {
    if (!digits(part)) return {};
    parts.push_back(STRING::number<Whole>(part));
  }
  return parts.size() == PARTS ? parts : Vector<Whole>{};
}
}  // namespace
}  // namespace LAUNCHER::STORE

auto LAUNCHER::STORE::GET::stated(const Offer &offer) -> String {
  return offer.engine;
}

auto LAUNCHER::STORE::GET::above(const String &stated) -> Flag {
  const Vector<Whole> needed = numbers(stated);
  const Vector<Whole> own = numbers(ENGINE::VERSION);
  return !needed.empty() && !own.empty() && needed > own;
}

auto LAUNCHER::STORE::fitted(Offer &offer) -> Flag {
  if (!GET::above(GET::stated(offer))) return true;
  offer.state = String(NEEDS) + GET::stated(offer);
  return false;
}
