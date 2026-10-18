// SPDX-License-Identifier: AGPL-3.0-or-later
#include <common.hpp>
#include <filesystem>
#include <logger.hpp>

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

auto LAUNCHER::STORE::keep(const String &text) -> STRING::Hot {
  const String repository = trimmed(text);
  if (repository.empty()) return EMPTY;
  for (const Offer &offer : offers)
    if (GET::root(offer.repository) == GET::root(repository)) return LISTED;
  if (offers.size() >= ROWS) return FULL;
  Vector<String> links = GET::links();
  links.push_back(repository);
  write(links);
  offers.push_back(
    {.repository = repository,
     .leaf = GET::leaf(repository),
     .state = REACHING,
     .linked = true});
  return ADDED;
}

void LAUNCHER::STORE::argued() {
  const Vector<String> &words = CARTRIDGE::GET::arguments();
  if (words.size() != 2) return;
  if (!surveyed) load();
  if (words[0] == WORD) keep(words[1]);
  if (words[0] != WANT || keep(words[1]) == FULL) return;
  wanted = trimmed(words[1]);
  LAUNCHER::enter(IDS::STORE);
}

void LAUNCHER::STORE::claim() {
  if (wanted.empty() || installing != ISLANDS::SELECT::NONE) return;
  for (Whole row = 0; row < offers.size(); row += 1) {
    Offer &offer = offers[row];
    if (GET::root(offer.repository) != GET::root(wanted)) continue;
    if (offer.state == REACHING) return;
    if (offer.state == UNREACHED || offer.state == REFUSED) {
      LOGGER::get("~/launcher/store")
        .error(
          "%s: %s, not installed", offer.leaf.c_str(), offer.state.c_str());
      wanted.clear();
      return;
    }
    if (offer.name.empty()) return;
    wanted.clear();
    installed(offer);
    if (!offer.installed) install(row);
    return;
  }
}
