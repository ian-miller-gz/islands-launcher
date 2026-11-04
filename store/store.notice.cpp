// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

namespace LAUNCHER::STORE {
namespace {
constexpr STRING::Hot FAULT = "error";
constexpr STRING::Hot DASH = " \u2014 ";
constexpr STRING::Hot COLON = ": ";
constexpr Char SPACE = ' ';
constexpr Char SLASH = '/';

auto pared(const String &line) -> String {
  const String text = STRING::trim(line);
  const auto cut = text.rfind(SLASH, text.find(SPACE));
  return cut == String::npos ? text : text.substr(cut + 1);
}

auto first(const Vector<String> &lines) -> String {
  for (const String &line : lines)
    if (line.find(FAULT) != String::npos) return pared(line);
  return lines.empty() ? String{} : pared(lines.back());
}

auto told(const Offer &offer) -> String {
  return offer.leaf + COLON + offer.failure;
}
}  // namespace
}  // namespace LAUNCHER::STORE

void LAUNCHER::STORE::fail(
  Offer &offer, STRING::Hot step, const Vector<String> &lines) {
  const String line = first(lines);
  offer.failure = String(FAILED) + " " + step;
  if (!line.empty()) offer.failure += DASH + line;
  GUI::set(document, IDS::NOTICELINE, GUI::Text{told(offer)});
  GUI::set(document, IDS::NOTICE, GUI::Visibility{true});
}

auto LAUNCHER::STORE::GET::failure(const String &leaf) -> String {
  if (leaf.empty()) return {};
  for (const Offer &offer : offers)
    if (offer.leaf == leaf && !offer.failure.empty()) return told(offer);
  return {};
}
