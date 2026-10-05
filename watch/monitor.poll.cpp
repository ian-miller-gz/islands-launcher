// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cartridge/relations.hpp>

#include "../monitor/monitor.hpp"

#include "../cartridge.hpp"

static const String BEGIN = "begin\n";
static const String END = "end\n";
static const String STATE = "state ";
static constexpr Float PERIOD = 1.0f;
static constexpr Whole MENDS = 3;

static void latch(pid_t pid) {
  for (auto &child : LAUNCHER::children)
    if (child.pid == pid) child.running = false;
}

static void ingest(const String &block) {
  Whole count = 0;
  for (Whole from = 0; from < block.size();) {
    const auto newline = block.find('\n', from);
    const String line = block.substr(from, newline - from);
    from = newline == String::npos ? block.size() : newline + 1;
    if (!line.starts_with(STATE)) continue;
    const String rest = line.substr(STATE.size());
    const auto first = rest.find(' ');
    const auto second = rest.find(' ', first + 1);
    if (first == String::npos || second == String::npos) continue;
    ++count;
    if (rest.substr(first + 1, second - first - 1) == "stopped")
      latch(static_cast<pid_t>(STRING::number<Integer>(rest.substr(0, first))));
  }
  LAUNCHER::WATCH::tracked = count;
}

static void drain() {
  if (!NETWORK::SESSIONS::receive(
        LAUNCHER::WATCH::session, LAUNCHER::WATCH::buffer))
    return LAUNCHER::WATCH::detach();
  auto &buffer = LAUNCHER::WATCH::buffer;
  const auto end = buffer.rfind(END);
  if (end == String::npos) return;
  const auto begin = buffer.rfind(BEGIN, end);
  if (begin != String::npos) ingest(buffer.substr(begin, end - begin));
  buffer.erase(0, end + END.size());
}

// The dial is a glance per frame, never a wait: the session opens when the
// monitor answers, or closes when it refuses or the dial's patience runs
// out, and the frame goes on either way.
static void settle() {
  namespace WATCH = LAUNCHER::WATCH;
  switch (NETWORK::SESSIONS::settle(WATCH::session)) {
    case NETWORK::SESSIONS::State::OPEN: WATCH::ready = true; return;
    case NETWORK::SESSIONS::State::CLOSED: WATCH::session = NETWORK::NONE; return;
    case NETWORK::SESSIONS::State::DIALING: return;
  }
}

void LAUNCHER::WATCH::attach() {
  session = NETWORK::dial(MONITOR::NAME).handle;
  ready = false;
  if (session != NETWORK::NONE) ::settle();
}

void LAUNCHER::WATCH::detach() {
  if (session != NETWORK::NONE) NETWORK::SESSIONS::destroy(session);
  session = NETWORK::NONE;
  ready = false;
}

// A lost monitor is mended by starting its reef again and dialing anew —
// three times in a run, then the launcher rests on its own reaping: a
// monitor that cannot be reached here is not reached by dialing forever.
static void mend() {
  namespace WATCH = LAUNCHER::WATCH;
  if (WATCH::mended >= MENDS) return;
  WATCH::mended += 1;
  RELATIONS::spawn(REEF_NAME, WATCH::BUNDLE);
  WATCH::attach();
}

void LAUNCHER::WATCH::poll() {
  if (session != NETWORK::NONE && !ready) ::settle();
  if (ready) drain();
  const Float now = CLOCK::GET::elapsed();
  if (now - asked < PERIOD) return;
  asked = now;
  if (ready) {
    if (!NETWORK::SESSIONS::push(session, "poll\n")) detach();
    return;
  }
  if (session == NETWORK::NONE) ::mend();
}

void LAUNCHER::WATCH::announce(const LAUNCHER::Child &child) {
  if (!ready) return;
  NETWORK::SESSIONS::push(
    session, "watch " + std::to_string(child.pid) + " " + child.name + "\n");
}

void LAUNCHER::WATCH::release(pid_t pid) {
  if (!ready) return;
  NETWORK::SESSIONS::push(session, "forget " + std::to_string(pid) + "\n");
}
