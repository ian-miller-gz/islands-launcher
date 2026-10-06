// SPDX-License-Identifier: AGPL-3.0-or-later
#include <filesystem>
#include <logger.hpp>

#include <cartridge.hpp>
#include <common/platform/selection.hpp>

#include "../cartridge.hpp"

namespace fs = std::filesystem;
using LAUNCHER::STORE::Offer;

namespace LAUNCHER::STORE {
namespace {
constexpr STRING::Hot CATEGORY = "~/launcher/store";
constexpr STRING::Hot SOURCE = "cartridge.cpp";
constexpr STRING::Hot BUILD = "bash cartridges/build.sh ";

auto staging(const Offer &offer) -> String {
  return (fs::path(LAUNCHER::STORE::GET::stage()) / offer.leaf).string();
}

auto placed(const Offer &offer) -> String {
  return (fs::path(LAUNCHER::STORE::GET::home()) / offer.leaf).string();
}

auto cloning(const Offer &offer) -> String {
  std::error_code stood;
  if (offer.linked && fs::exists(fs::path(staging(offer)) / SOURCE, stood))
    return {};
  return LAUNCHER::STORE::GET::clone(offer.repository, staging(offer));
}

auto building(const Offer &offer) -> String {
  return String(BUILD) + SHELL::OS::quote(staging(offer));
}

// A Windows install builds nothing, so the staged tree must already carry
// the entry the manifest names for this platform (the release asset's
// windows fold); on POSIX the build step lays it after this check.
auto delivered(const String &home, const CARTRIDGE::Manifest &manifest)
  -> Flag {
#if SR_PLATFORM == SR_WINDOWS
  std::error_code slip;
  return fs::exists(CARTRIDGE::MANIFEST::entry(home, manifest), slip);
#else
  (void)home, (void)manifest;
  return true;
#endif
}

auto checked(Offer &offer) -> Flag {
  const String home = staging(offer);
  const CARTRIDGE::Manifest manifest = CARTRIDGE::MANIFEST::read(home);
  std::error_code slip;
  return manifest.name == offer.name && manifest.entry == offer.entry &&
         manifest.version == CARTRIDGE::VERSION &&
         fs::exists(fs::path(home) / SOURCE, slip) && delivered(home, manifest);
}

auto placing(Offer &offer) -> Flag {
  const fs::path home = placed(offer);
  std::error_code slip;
  fs::remove_all(home, slip);
  fs::create_directories(home.parent_path(), slip);
  fs::rename(staging(offer), home, slip);
  if (slip) {
    fs::copy(staging(offer), home, fs::copy_options::recursive, slip);
    fs::remove_all(staging(offer), slip);
  }
  return fs::exists(home / LAUNCHER::MANIFEST, slip);
}

auto confirmed(Offer &offer) -> Flag {
  LAUNCHER::STORE::installed(offer);
  if (!offer.installed) return false;
  offer.state = LAUNCHER::STORE::MARK;
  LOGGER::get(CATEGORY).info(
    "%s: installed %s", offer.leaf.c_str(), placed(offer).c_str());
  LAUNCHER::scan();
  return true;
}
}  // namespace
}  // namespace LAUNCHER::STORE

auto LAUNCHER::STORE::quiet(const Offer &) -> String { return {}; }

auto LAUNCHER::STORE::ended(Offer &) -> Flag { return true; }

auto LAUNCHER::STORE::GET::steps() -> const Vector<Step> & {
#if SR_PLATFORM == SR_WINDOWS
  // No git and no compiler on a Windows desk: the fetch downloads the
  // release's windows asset in the launcher's own process and no build
  // step stands between the check and the placing.
  static const Vector<Step> walk = {
    {REPORTING, quiet, report},
    {VERIFYING, quiet, verify},
    {FETCHING, quiet, ended, ASSET::wait},
    {CHECKING, quiet, checked},
    {PLACING, quiet, placing},
    {CONFIRMING, quiet, confirmed},
  };
#else
  static const Vector<Step> walk = {
    {REPORTING, quiet, report},     {VERIFYING, quiet, verify},
    {FETCHING, cloning, ended},     {CHECKING, quiet, checked},
    {BUILDING, building, ended},    {PLACING, quiet, placing},
    {CONFIRMING, quiet, confirmed},
  };
#endif
  return walk;
}
