// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>

#include "../cartridge.hpp"

using LAUNCHER::SOURCES::Source;
using LAUNCHER::STORE::Offer;

namespace LAUNCHER::STORE {
namespace {
constexpr STRING::Hot SWEEP = "rm -rf ";
constexpr STRING::Hot CLONE = "git clone --depth 1 ";
constexpr STRING::Hot LINED = " --branch ";

auto source(const Offer &offer) -> const Source * {
  return LAUNCHER::SOURCES::GET::source(
    offer.leaf, LAUNCHER::LINE::GET::line(offer.leaf));
}
}  // namespace
}  // namespace LAUNCHER::STORE

auto LAUNCHER::STORE::GET::named(const Offer &offer) -> Flag {
  return source(offer) != nullptr;
}

auto LAUNCHER::STORE::GET::reached(const Offer &offer) -> Flag {
  return offer.linked || named(offer);
}

auto LAUNCHER::STORE::GET::reaching() -> Flag {
  return std::ranges::any_of(
    offers, [](const Offer &offer) { return offer.state == REACHING; });
}

auto LAUNCHER::STORE::GET::branch(const Offer &offer) -> String {
  const Source *line = source(offer);
  return line != nullptr ? line->branch : LINE::GET::line(offer.leaf);
}

auto LAUNCHER::STORE::GET::origin(const Offer &offer) -> String {
  const Source *line = source(offer);
  return line != nullptr ? line->repository : offer.repository;
}

auto LAUNCHER::STORE::GET::probe(const Offer &offer) -> String {
  const String from = SHELL::OS::quote(origin(offer));
  if (!named(offer)) return String(PROMPTLESS) + PROBE + from + HEAD + QUIET;
  return String(PROMPTLESS) + PROBE + from + " " +
         SHELL::OS::quote(String(REF) + branch(offer)) + MUTE;
}

auto LAUNCHER::STORE::GET::clone(const Offer &offer, const String &home)
  -> String {
  const String from = SHELL::OS::quote(origin(offer));
  const String to = SHELL::OS::quote(home);
  const String lined = String(CLONE) + LINED + SHELL::OS::quote(branch(offer)) +
                       " " + from + " " + to;
  if (named(offer)) return String(SWEEP) + to + " && " + lined;
  return String(SWEEP) + to + " && (" + lined + " || (" + SWEEP + to + " && " +
         CLONE + from + " " + to + "))";
}
