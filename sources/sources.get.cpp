// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cartridge.hpp"

using LAUNCHER::SOURCES::Source;

auto LAUNCHER::SOURCES::GET::refusal(const Source &source) -> STRING::Hot {
  if (source.leaf.empty() || source.repository.empty() || source.branch.empty())
    return EMPTY;
  if (taken(source.leaf, source.branch)) return TAKEN;
  if (named(source.leaf).size() >= LINE::NAMED) return FULL;
  return KEPT;
}

auto LAUNCHER::SOURCES::GET::taken(const String &leaf, const String &name)
  -> Flag {
  return LINE::GET::placed(name) != ISLANDS::SELECT::NONE ||
         source(leaf, name) != nullptr;
}

auto LAUNCHER::SOURCES::GET::named(const String &leaf)
  -> Vector<const Source *> {
  Vector<const Source *> named;
  for (const Source &source : sources)
    if (source.leaf == leaf) named.push_back(&source);
  return named;
}

auto LAUNCHER::SOURCES::GET::source(const String &leaf, const String &branch)
  -> const Source * {
  for (const Source &source : sources)
    if (source.leaf == leaf && source.branch == branch) return &source;
  return nullptr;
}

auto LAUNCHER::SOURCES::GET::listed(const String &leaf) -> String {
  if (leaf == LINE::LAUNCHER) return LINE::GET::origin();
  if (STORE::offers.empty()) STORE::load();
  for (const STORE::Offer &offer : STORE::offers)
    if (offer.leaf == leaf) return offer.repository;
  return String();
}
