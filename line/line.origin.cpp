// SPDX-License-Identifier: AGPL-3.0-or-later
#include <common.hpp>
#include <filesystem>

#include "../cartridge.hpp"

namespace fs = std::filesystem;

namespace LAUNCHER::LINE {
namespace {
constexpr STRING::Hot GITDIR = "gitdir:";
constexpr STRING::Hot SHARED = "commondir";
constexpr STRING::Hot CONFIG = "config";
constexpr STRING::Hot SECTION = "[remote \"origin\"]";
constexpr STRING::Hot URL = "url";
constexpr Char OPEN = '[';
constexpr Char EQUALS = '=';

auto first(const fs::path &path) -> String {
  IO::STREAMS::Input in(path.string());
  String line;
  std::getline(in, line);
  return STRING::trim(line);
}

auto beside(const fs::path &from, const String &to) -> fs::path {
  const fs::path path = to;
  return path.is_relative() ? from / path : path;
}

auto common() -> fs::path {
  const fs::path dot = fs::path(LAUNCHER::LINE::SEAT) / LAUNCHER::LINE::CLONE;
  std::error_code slip;
  if (fs::is_directory(dot, slip)) return dot;
  const String line = first(dot);
  if (!line.starts_with(GITDIR)) return {};
  const fs::path dir = beside(
    LAUNCHER::LINE::SEAT, STRING::trim(line.substr(String(GITDIR).size())));
  const String shared = first(dir / SHARED);
  return shared.empty() ? dir : beside(dir, shared);
}
}  // namespace
}  // namespace LAUNCHER::LINE

auto LAUNCHER::LINE::GET::origin() -> String {
  const fs::path dir = common();
  if (dir.empty()) return String();
  IO::STREAMS::Input in((dir / CONFIG).string());
  String line;
  Flag inside = false;
  while (std::getline(in, line)) {
    const String text = STRING::trim(line);
    if (text.starts_with(OPEN)) inside = text == SECTION;
    const auto cut = text.find(EQUALS);
    if (!inside || cut == String::npos) continue;
    if (STRING::trim(text.substr(0, cut)) == URL)
      return STRING::trim(text.substr(cut + 1));
  }
  return String();
}
