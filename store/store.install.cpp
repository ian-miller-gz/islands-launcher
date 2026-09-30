// SPDX-License-Identifier: AGPL-3.0-or-later
#include <logger.hpp>

#include "../cartridge.hpp"

namespace STORE = LAUNCHER::STORE;
using LAUNCHER::STORE::Offer;
using LAUNCHER::STORE::Step;

namespace LAUNCHER::STORE {
namespace {
constexpr STRING::Hot CATEGORY = "~/launcher/store";
constexpr STRING::Hot DONE = "[store] done";
constexpr Whole KEEP = 12;
constexpr Whole REFUSAL = 1;

SHELL::OS::Process shell;
Offer *subject = nullptr;
const Vector<Step> *walk = nullptr;
Whole at = 0;
Flag opened = false;
Flag awaiting = false;
Vector<String> lines;

void rest() {
  SHELL::OS::PROCESS::stop(shell);
  STORE::installing = ISLANDS::SELECT::NONE;
  subject = nullptr;
  walk = nullptr;
}

void sank(Offer &offer, STRING::Hot word, Whole code) {
  offer.state = String(STORE::FAILED) + " " + word + " " + std::to_string(code);
  LOGGER::Category &logger = LOGGER::get(CATEGORY);
  logger.error("%s: %s", offer.leaf.c_str(), offer.state.c_str());
  for (const String &line : lines) logger.error("  %s", line.c_str());
  rest();
}

void open(Offer &offer) {
  const Step &step = (*walk)[at];
  offer.state = step.word;
  opened = true;
  lines.clear();
  const String command = step.call(offer);
  if (command.empty()) return;
  SHELL::OS::PROCESS::feed(shell, command + "; echo \"" + DONE + " $?\"");
  awaiting = true;
}

auto code() -> Whole {
  if (!awaiting) return 0;
  const String head = String(DONE) + " ";
  for (const String &line : SHELL::OS::PROCESS::harvest(shell)) {
    if (line.starts_with(head))
      return STRING::number<Whole>(line.substr(head.size()));
    lines.push_back(line);
  }
  if (lines.size() > KEEP) lines.erase(lines.begin(), lines.end() - KEEP);
  return ISLANDS::SELECT::NONE;
}
}  // namespace
}  // namespace LAUNCHER::STORE

void LAUNCHER::STORE::run(Offer &offer, const Vector<Step> &steps) {
  if (subject != nullptr || steps.empty()) return;
  SHELL::OS::PROCESS::spawn(shell);
  if (!SHELL::OS::PROCESS::running(shell)) return;
  subject = &offer;
  walk = &steps;
  at = 0;
  opened = false;
  awaiting = false;
}

void LAUNCHER::STORE::install(Whole row) {
  if (subject != nullptr) return;
  if (row >= offers.size() || offers[row].installed) return;
  run(offers[row], GET::steps());
  if (subject != nullptr) installing = row;
}

void LAUNCHER::STORE::poll() {
  if (subject == nullptr) return;
  Offer &offer = *subject;
  const Step &step = (*walk)[at];
  if (!SHELL::OS::PROCESS::running(shell))
    return sank(offer, step.word, REFUSAL);
  if (!opened) return open(offer);
  const Whole status = code();
  if (status == ISLANDS::SELECT::NONE) return;
  if (status || !step.held(offer))
    return sank(offer, step.word, status ? status : REFUSAL);
  at += 1;
  opened = false;
  awaiting = false;
  if (at >= walk->size()) rest();
}

auto LAUNCHER::STORE::GET::running() -> Flag { return subject != nullptr; }
