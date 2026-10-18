// SPDX-License-Identifier: AGPL-3.0-or-later
#include <common.hpp>
#include <algorithm>
#include <filesystem>
#include <sstream>

#include "../cartridge.hpp"

namespace fs = std::filesystem;
using LAUNCHER::LINE::Choice;

namespace LAUNCHER::LINE {
namespace {
constexpr STRING::Hot LINED = "line";
constexpr STRING::Hot TIPPED = "tip";
constexpr Char OPEN = '{';
constexpr Char CLOSE = '}';
constexpr Char COMMA = ',';

auto file() -> String {
  const fs::path path = fs::path(COMMON::STATE) / LAUNCHER::LINE::CARTRIDGES;
  std::error_code slip;
  fs::create_directories(path.parent_path(), slip);
  return path.string();
}

auto entry(const String &leaf) -> Choice * {
  for (Choice &choice : LAUNCHER::LINE::choices)
    if (choice.leaf == leaf) return &choice;
  LAUNCHER::LINE::choices.push_back({.leaf = leaf});
  return &LAUNCHER::LINE::choices.back();
}

void dress(Choice &choice, const String &text) {
  if (!text.starts_with(OPEN)) {
    choice.line = text;
    return;
  }
  std::istringstream fields(text.substr(1, text.find(CLOSE) - 1));
  String field, key, value;
  while (std::getline(fields, field, COMMA)) {
    if (!STRING::pair(field, key, value)) continue;
    if (key == LINED) choice.line = value;
    if (key == TIPPED) choice.tip = value;
  }
}

void record() {
  IO::STREAMS::Output out(file());
  if (!out) return;
  std::ranges::sort(LAUNCHER::LINE::choices, {}, &Choice::leaf);
  for (const Choice &choice : LAUNCHER::LINE::choices) {
    if (choice.tip.empty() && choice.line.empty()) continue;
    if (choice.tip.empty()) {
      out << choice.leaf << ": " << choice.line << "\n";
      continue;
    }
    out << choice.leaf << ": " << OPEN;
    if (!choice.line.empty()) out << LINED << ": " << choice.line << ", ";
    out << TIPPED << ": " << choice.tip << CLOSE << "\n";
  }
}
}  // namespace
}  // namespace LAUNCHER::LINE

void LAUNCHER::LINE::load() {
  choices.clear();
  IO::STREAMS::Input in(file());
  String text, key, value;
  while (std::getline(in, text))
    if (STRING::pair(text, key, value) && !key.empty())
      dress(*entry(key), value);
}

void LAUNCHER::LINE::set(const String &leaf, const String &line) {
  entry(leaf)->line = line;
  record();
}

void LAUNCHER::LINE::mark(const String &leaf, const String &tip) {
  if (tip == GET::tip(leaf)) return;
  entry(leaf)->tip = tip;
  record();
}
