// SPDX-License-Identifier: AGPL-3.0-or-later
#include <filesystem>

#include "../cartridge.hpp"

namespace fs = std::filesystem;
using LAUNCHER::STORE::Offer;

namespace LAUNCHER::STORE {
namespace {
auto row(const String &leaf) -> Whole {
  namespace STORE = LAUNCHER::STORE;
  for (Whole at = 0; at < STORE::offers.size(); at += 1)
    if (STORE::offers[at].leaf == leaf) return at;
  return ISLANDS::SELECT::NONE;
}
}  // namespace
}  // namespace LAUNCHER::STORE

auto LAUNCHER::STORE::held(const Offer &offer) -> String {
  const fs::path home = fs::path(GET::home()) / offer.leaf;
  for (const Entry &entry : LAUNCHER::entries)
    if (fs::path(entry.directory) == home) return entry.release;
  return {};
}

auto LAUNCHER::STORE::GET::update(Whole row) -> Flag {
  if (row >= offers.size()) return false;
  const Offer &offer = offers[row];
  return offer.installed && !offer.held.empty() && !offer.release.empty() &&
         offer.held != offer.release;
}

auto LAUNCHER::STORE::GET::update(const String &leaf) -> Flag {
  return update(row(leaf));
}

void LAUNCHER::STORE::update(Whole row) {
  if (GET::running() || !GET::update(row)) return;
  run(offers[row], GET::steps());
  if (GET::running()) installing = row;
}

void LAUNCHER::STORE::update(const String &leaf) { update(row(leaf)); }
