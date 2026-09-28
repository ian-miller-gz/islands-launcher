// SPDX-License-Identifier: AGPL-3.0-or-later
#include <common.hpp>
#include <filesystem>

#include "../cartridge.hpp"

namespace fs = std::filesystem;

namespace LAUNCHER::STORE {
namespace {
constexpr STRING::Hot ITEM = "  - ";
constexpr STRING::Hot BLANK = " \t\r\n";

auto file() -> String {
  const fs::path path = fs::path(COMMON::STATE) / LAUNCHER::STORE::LINKS;
  std::error_code slip;
  fs::create_directories(path.parent_path(), slip);
  return path.string();
}

void write(const Vector<String> &links) {
  IO::STREAMS::Output out(file());
  if (!out) return;
  out << LAUNCHER::STORE::LINKED << ":\n";
  for (const String &link : links) out << ITEM << link << "\n";
}

auto trimmed(const String &text) -> String {
  const auto first = text.find_first_not_of(BLANK);
  if (first == String::npos) return {};
  const auto last = text.find_last_not_of(BLANK);
  return text.substr(first, last - first + 1);
}

void note(STRING::Hot word) {
  GUI::set(LAUNCHER::document, LAUNCHER::IDS::LINKED, GUI::Text{word});
}
}  // namespace
}  // namespace LAUNCHER::STORE

auto LAUNCHER::STORE::GET::links() -> Vector<String> {
  Vector<String> links;
  IO::STREAMS::Input in(file());
  String line;
  while (std::getline(in, line)) {
    if (!line.starts_with(ITEM)) continue;
    const String link = trimmed(line.substr(String(ITEM).size()));
    if (!link.empty()) links.push_back(link);
  }
  return links;
}

void LAUNCHER::STORE::link(const String &text) {
  const String repository = trimmed(text);
  GUI::set(document, IDS::LINK, GUI::Text{String()});
  if (repository.empty()) return;
  for (const Offer &offer : offers)
    if (offer.repository == repository) return note("");
  if (offers.size() >= ROWS) return note(FULL);
  Vector<String> links = GET::links();
  links.push_back(repository);
  write(links);
  offers.push_back(
    {.repository = repository,
     .leaf = GET::leaf(repository),
     .state = REACHING,
     .linked = true});
  note("");
}
