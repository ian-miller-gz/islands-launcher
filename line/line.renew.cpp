// SPDX-License-Identifier: AGPL-3.0-or-later
#include <csignal>

#include <common/platform/paths.hpp>

#include "../cartridge.hpp"

using LAUNCHER::STORE::Offer;

namespace LAUNCHER::LINE {
namespace {
auto updating() -> String {
  namespace LINE = LAUNCHER::LINE;
  return String(LINE::DETACHED) + LINE::RUNNER + LINE::INSTALLER +
         LINE::UPDATE + LINE::CHANNEL + LINE::followed + LINE::WAIT +
         std::to_string(COMMON::PLATFORM::identity()) + LINE::RESTART +
         LINE::BACKGROUND;
}

auto launching(Offer &) -> Whole {
  return static_cast<Whole>(SHELL::OS::run(updating()));
}

auto left(Offer &) -> Flag {
  std::raise(SIGTERM);
  return true;
}
}  // namespace
}  // namespace LAUNCHER::LINE

auto LAUNCHER::LINE::GET::renewals() -> const Vector<STORE::Step> & {
  static const Vector<STORE::Step> walk = {
    {UPDATING, STORE::quiet, left, launching}};
  return walk;
}
