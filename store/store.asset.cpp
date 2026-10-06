// SPDX-License-Identifier: AGPL-3.0-or-later
#include <atomic>
#include <filesystem>
#include <logger.hpp>
#include <memory>
#include <thread>

#include "../cartridge.hpp"

namespace fs = std::filesystem;
using LAUNCHER::STORE::Offer;

namespace LAUNCHER::STORE::ASSET {
namespace {
constexpr STRING::Hot CATEGORY = "~/launcher/store";
constexpr Whole REFUSAL = 1;

// One transfer at a time. The worker owns a share of the record and the
// frame owns another: a transfer the frame abandons (the install sank or
// the launcher closed) runs out on its own and is dropped at the end,
// since a web transfer cannot be interrupted mid-stream and a frame must
// never wait on one.
struct Transfer {
  String url;
  String into;
  String error;
  std::atomic<Flag> done = false;
};

std::shared_ptr<Transfer> current;

void carry(std::shared_ptr<Transfer> transfer) {
  const NETWORK::WEB::Result answer = NETWORK::WEB::get(transfer->url);
  if (!answer.ok || answer.status != LAUNCHER::STORE::SERVED) {
    transfer->error = answer.error.empty()
                        ? "HTTP " + std::to_string(answer.status)
                        : answer.error;
  } else {
    std::error_code slip;
    fs::remove_all(transfer->into, slip);
    fs::create_directories(transfer->into, slip);
    LAUNCHER::STORE::ARCHIVE::unpack(
      answer.body, transfer->into, transfer->error);
  }
  transfer->done.store(true, std::memory_order_release);
}
}  // namespace
}  // namespace LAUNCHER::STORE::ASSET

// <repository>/releases/download/<release>/<leaf>-<release>-windows-x64.zip
auto LAUNCHER::STORE::ASSET::address(const Offer &offer) -> String {
  return GET::root(offer.repository) + RELEASES + offer.release + "/" +
         offer.leaf + "-" + offer.release + ASSET_TAIL;
}

auto LAUNCHER::STORE::ASSET::wait(Offer &offer) -> Whole {
  LOGGER::Category &logger = LOGGER::get(CATEGORY);
  if (!current) {
    if (offer.release.empty()) {
      logger.error("%s: the manifest names no release", offer.leaf.c_str());
      return REFUSAL;
    }
    current = std::make_shared<Transfer>();
    current->url = address(offer);
    current->into = GET::staging(offer);
    logger.info("%s: fetching %s", offer.leaf.c_str(), current->url.c_str());
    std::thread(carry, current).detach();
    return ISLANDS::SELECT::NONE;
  }
  if (!current->done.load(std::memory_order_acquire))
    return ISLANDS::SELECT::NONE;
  const String error = current->error;
  current.reset();
  if (error.empty()) {
    logger.debug("%s: fetched and unpacked", offer.leaf.c_str());
    return 0;
  }
  logger.error("%s: %s", offer.leaf.c_str(), error.c_str());
  return REFUSAL;
}

void LAUNCHER::STORE::ASSET::rest() { current.reset(); }
