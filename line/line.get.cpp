// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

using LAUNCHER::LINE::Choice;

namespace LAUNCHER::LINE {
namespace {
auto found(const String &leaf) -> const Choice * {
  for (const Choice &choice : LAUNCHER::LINE::choices)
    if (choice.leaf == leaf) return &choice;
  return nullptr;
}
}  // namespace
}  // namespace LAUNCHER::LINE

auto LAUNCHER::LINE::GET::line(const String &leaf) -> String {
  const Choice *choice = found(leaf);
  if (choice != nullptr && SOURCES::GET::taken(leaf, choice->line))
    return choice->line;
  if (SOURCES::GET::taken(leaf, followed)) return followed;
  return STABLE;
}

auto LAUNCHER::LINE::GET::tip(const String &leaf) -> String {
  const Choice *choice = found(leaf);
  return choice == nullptr ? String() : choice->tip;
}

auto LAUNCHER::LINE::GET::lines(const String &leaf) -> Vector<String> {
  Vector<String> words(std::begin(LINES), std::end(LINES));
  for (const SOURCES::Source *source : SOURCES::GET::named(leaf))
    words.push_back(source->branch);
  return words;
}

auto LAUNCHER::LINE::GET::word(STRING::Hot door, const String &line) -> String {
  if (refused != door || !STORE::GET::reaching()) return line;
  return STORE::REACHING;
}
