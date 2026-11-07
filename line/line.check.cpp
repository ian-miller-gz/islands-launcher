// SPDX-License-Identifier: AGPL-3.0-or-later
#include <atomic>
#include <filesystem>
#include <memory>
#include <thread>


#include "../cartridge.hpp"

namespace fs = std::filesystem;
using LAUNCHER::STORE::Offer;

namespace LAUNCHER::LINE {
namespace {
constexpr STRING::Hot CHECK = "check";
constexpr STRING::Hot INTO = " > ";
constexpr STRING::Hot BOTH = " 2>&1";
constexpr Char SPACE = ' ';

struct Probe {
  String command;
  Status status = 0;
  std::atomic<Flag> done = false;
};

std::shared_ptr<Probe> probe;
Status code = 0;

void carry(std::shared_ptr<Probe> run) {
  run->status = SHELL::OS::run(run->command);
  run->done.store(true, std::memory_order_release);
}

auto checking() -> String {
  namespace LINE = LAUNCHER::LINE;
  return String(LINE::RUNNER) + LINE::INSTALLER + CHECK + LINE::CHANNEL +
         LINE::followed + INTO + SHELL::OS::quote(LINE::REPORT) + BOTH;
}

auto probing(Offer &) -> Whole {
  if (!probe) {
    std::error_code slip;
    fs::create_directories(fs::path(LAUNCHER::LINE::REPORT).parent_path(), slip);
    probe = std::make_shared<Probe>();
    probe->command = checking();
    std::thread(carry, probe).detach();
    return ISLANDS::SELECT::NONE;
  }
  if (!probe->done.load(std::memory_order_acquire)) return ISLANDS::SELECT::NONE;
  code = probe->status;
  probe.reset();
  return 0;
}

auto reported(Offer &offer) -> Flag {
  namespace LINE = LAUNCHER::LINE;
  IO::STREAMS::Input in(LINE::REPORT);
  String line, current, wanted;
  while (std::getline(in, line)) {
    const auto cut = line.find(SPACE);
    if (cut == String::npos) continue;
    const String key = line.substr(0, cut), value = line.substr(cut + 1);
    if (key == LINE::INSTALLED) current = value;
    if (key == LINE::followed) wanted = value;
  }
  offer.state.clear();
  if (code == 0) offer.state = LINE::CURRENT;
  if (code != LINE::AHEAD || wanted.empty() || wanted == LINE::skipped)
    return true;
  offer.state = String(LINE::UPDATE) + SPACE + wanted;
  LINE::tender(current, wanted);
  return true;
}

}  // namespace
}  // namespace LAUNCHER::LINE

auto LAUNCHER::LINE::GET::installed() -> Flag {
  std::error_code slip;
  return fs::is_regular_file(RECORD, slip);
}

auto LAUNCHER::LINE::GET::checks() -> const Vector<STORE::Step> & {
  static const Vector<STORE::Step> walk = {
    {STORE::CHECKING, STORE::quiet, reported, probing}};
  return walk;
}

void LAUNCHER::LINE::check() {
  if (!GET::installed() || !STORE::wanted.empty()) return;
  STORE::run(offer, GET::checks());
}
