// SPDX-License-Identifier: AGPL-3.0-or-later
#include <csignal>
#include <filesystem>

#include <cartridge/relations.hpp>
#include <common/platform/selection.hpp>

#include "../cartridge.hpp"

namespace fs = std::filesystem;
using LAUNCHER::STORE::Offer;

namespace LAUNCHER::LINE {
namespace {
constexpr STRING::Hot GIT = "git -C ";
constexpr STRING::Hot FETCH = " fetch --depth 1 origin ";
constexpr STRING::Hot CHECKOUT = " checkout -q -B ";
constexpr STRING::Hot TIP = " FETCH_HEAD";
constexpr STRING::Hot BUILD = "bash cartridges/build.sh ";

auto fetching(const Offer &offer) -> String {
  const String seat = SHELL::OS::quote(offer.repository);
  return String(GIT) + seat + FETCH + LAUNCHER::LINE::followed + " && " + GIT +
         seat + CHECKOUT + LAUNCHER::LINE::followed + TIP;
}

auto building(const Offer &offer) -> String {
  return String(BUILD) + SHELL::OS::quote(offer.repository);
}

auto restarted(Offer &offer) -> Flag {
  if (RELATIONS::start(ISLAND_NAME, offer.repository) != 0) return false;
  std::raise(SIGTERM);
  return true;
}
}  // namespace
}  // namespace LAUNCHER::LINE

auto LAUNCHER::LINE::GET::cloned() -> Flag {
#if SR_PLATFORM == SR_WINDOWS
  // A Windows prefix ships the launcher built; with no git and no compiler
  // the line cannot be followed there, so it stands pinned.
  return false;
#else
  std::error_code slip;
  return fs::is_directory(fs::path(SEAT) / CLONE, slip);
#endif
}

auto LAUNCHER::LINE::GET::steps() -> const Vector<STORE::Step> & {
  static const Vector<STORE::Step> walk = {
    {STORE::FETCHING, fetching, STORE::ended},
    {STORE::BUILDING, building, STORE::ended},
    {RESTARTING, STORE::quiet, restarted}};
  return walk;
}

void LAUNCHER::LINE::follow() {
  offer = {.repository = SEAT, .leaf = SEAT};
  if (!GET::cloned()) {
    offer.state = PINNED;
    return;
  }
  STORE::run(offer, GET::steps());
}
