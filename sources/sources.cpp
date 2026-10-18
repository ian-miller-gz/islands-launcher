// SPDX-License-Identifier: AGPL-3.0-or-later
#include <common.hpp>
#include <filesystem>

#include "../cartridge.hpp"

namespace fs = std::filesystem;
using LAUNCHER::SOURCES::Source;

namespace LAUNCHER::SOURCES {
namespace {
constexpr STRING::Hot ITEM = "  - ";
constexpr STRING::Hot INDENT = "    ";
constexpr STRING::Hot TRUTH = "true";
constexpr STRING::Hot FALSITY = "false";

auto file() -> String {
  const fs::path path = fs::path(COMMON::STATE) / LAUNCHER::SOURCES::PATH;
  std::error_code slip;
  fs::create_directories(path.parent_path(), slip);
  return path.string();
}

void dress(Source &source, const String &key, const String &value) {
  namespace KEYS = LAUNCHER::SOURCES::KEYS;
  if (key == KEYS::REPOSITORY) source.repository = value;
  if (key == KEYS::BRANCH) source.branch = value;
  if (key == KEYS::LOCAL) source.local = value == TRUTH;
}

void shape(Source &source) {
  for (String *field : {&source.leaf, &source.repository, &source.branch})
    *field = STRING::trim(*field);
}
}  // namespace
}  // namespace LAUNCHER::SOURCES

void LAUNCHER::SOURCES::read() {
  Vector<Source> entries;
  IO::STREAMS::Input in(file());
  String line, key, value;
  while (std::getline(in, line)) {
    if (!STRING::pair(LAUNCHER::GET::bare(line), key, value)) continue;
    if (key == KEYS::LEAF)
      entries.push_back({.leaf = value});
    else if (!entries.empty())
      dress(entries.back(), key, value);
  }
  sources.clear();
  for (Source &entry : entries) {
    shape(entry);
    if (String(GET::refusal(entry)) == KEPT) sources.push_back(entry);
  }
}

void LAUNCHER::SOURCES::write() {
  IO::STREAMS::Output out(file());
  if (!out) return;
  out << KEY << ":\n";
  for (const Source &source : sources)
    out << ITEM << KEYS::LEAF << ": " << source.leaf << "\n"
        << INDENT << KEYS::REPOSITORY << ": " << source.repository << "\n"
        << INDENT << KEYS::BRANCH << ": " << source.branch << "\n"
        << INDENT << KEYS::LOCAL << ": " << (source.local ? TRUTH : FALSITY)
        << "\n";
}

auto LAUNCHER::SOURCES::keep(Source &source) -> STRING::Hot {
  shape(source);
  const STRING::Hot word = GET::refusal(source);
  if (String(word) != KEPT) return word;
  sources.push_back(source);
  write();
  return KEPT;
}

void LAUNCHER::SOURCES::drop(const String &leaf, const String &branch) {
  std::erase_if(sources, [&](const Source &source) {
    return source.leaf == leaf && source.branch == branch;
  });
  write();
}
