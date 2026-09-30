// SPDX-License-Identifier: AGPL-3.0-or-later
#include <filesystem>

#include <cartridge.hpp>

#include "../cartridge.hpp"

namespace fs = std::filesystem;
using LAUNCHER::STORE::Offer;

namespace LAUNCHER::STORE {
namespace {

auto address(const String &repository, const String &revision) -> String {
  namespace STORE = LAUNCHER::STORE;
  const String forge = STORE::FORGE;
  if (repository.starts_with(forge))
    return STORE::RAW + repository.substr(forge.size()) + "/" + revision + "/" +
           LAUNCHER::MANIFEST;
  return (fs::path(repository) / LAUNCHER::MANIFEST).string();
}

auto fetched(const Offer &offer, String &report) -> Flag {
  namespace STORE = LAUNCHER::STORE;
  if (offer.repository.find(STORE::SCHEME) == String::npos) {
    IO::STREAMS::Input in(address(offer.repository, STORE::REVISION));
    if (!in) return false;
    report.assign(std::istreambuf_iterator<Char>(in), {});
    return true;
  }
  NETWORK::WEB::Result answer =
    NETWORK::WEB::get(address(offer.repository, LAUNCHER::LINE::followed));
  if (!answer.ok)
    answer = NETWORK::WEB::get(address(offer.repository, STORE::REVISION));
  report = answer.body;
  return answer.ok;
}

auto staging(const Offer &offer) -> String {
  return (fs::path(LAUNCHER::STORE::STAGING) / offer.leaf).string();
}
}  // namespace
}  // namespace LAUNCHER::STORE

auto LAUNCHER::STORE::report(Offer &offer) -> Flag {
  String text;
  offer.state.clear();
  std::error_code reached;
  if (offer.linked)
    return fs::exists(fs::path(staging(offer)) / LAUNCHER::MANIFEST, reached);
  if (!fetched(offer, text)) {
    offer.state = UNREACHED;
    return false;
  }
  const fs::path home = staging(offer);
  std::error_code slip;
  fs::create_directories(home, slip);
  IO::STREAMS::Output out((home / LAUNCHER::MANIFEST).string());
  out << text;
  return out.good();
}

auto LAUNCHER::STORE::verify(Offer &offer) -> Flag {
  const CARTRIDGE::Manifest manifest =
    CARTRIDGE::MANIFEST::read(staging(offer));
  if (
    manifest.name.empty() || manifest.entry.empty() ||
    manifest.version != CARTRIDGE::VERSION) {
    offer.state = REFUSED;
    return false;
  }
  const auto found = manifest.keys.find(RELEASED);
  offer.name = manifest.name;
  offer.entry = manifest.entry;
  offer.release = found != manifest.keys.end() ? found->second : String();
  return true;
}

void LAUNCHER::STORE::survey() {
  if (surveyed) return;
  surveyed = true;
  load();
  for (Offer &offer : offers) {
    if (offer.linked) {
      offer.state = REACHING;
      continue;
    }
    if (report(offer)) verify(offer);
  }
}

auto LAUNCHER::STORE::GET::staging(const Offer &offer) -> String {
  return LAUNCHER::STORE::staging(offer);
}
