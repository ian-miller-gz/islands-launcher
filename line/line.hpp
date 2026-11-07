// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <common/platform/selection.hpp>

namespace LAUNCHER::LINE {

constexpr STRING::Hot STABLE = "stable";
constexpr STRING::Hot LATEST = "latest";
constexpr STRING::Hot LINES[] = {STABLE, LATEST};
constexpr STRING::Hot PATH = "configs/release.yaml";
constexpr STRING::Hot KEY = "release";
constexpr STRING::Hot SEAT = "cartridges/.core/launcher";
constexpr STRING::Hot CLONE = ".git";
constexpr STRING::Hot RESTARTING = "restarting";
constexpr STRING::Hot SKIPPED = "skipped";
constexpr STRING::Hot RECORD = "installed.yaml";
constexpr STRING::Hot REPORT = "logs/launcher.check";
constexpr STRING::Hot INSTALLED = "installed";
constexpr STRING::Hot CURRENT = "current";
constexpr STRING::Hot UPDATE = "update";
constexpr STRING::Hot UPDATING = "updating";
constexpr Whole AHEAD = 10;
#if SR_PLATFORM == SR_WINDOWS
constexpr STRING::Hot RUNNER = "powershell -NoProfile -ExecutionPolicy Bypass -File ";
constexpr STRING::Hot INSTALLER = "installer\\install.ps1 ";
constexpr STRING::Hot CHANNEL = " -Channel ";
constexpr STRING::Hot WAIT = " -Wait ";
constexpr STRING::Hot RESTART = " -Restart";
constexpr STRING::Hot DETACHED = "start \"\" /b ";
constexpr STRING::Hot BACKGROUND = "";
#else
constexpr STRING::Hot RUNNER = "sh ";
constexpr STRING::Hot INSTALLER = "installer/install.sh ";
constexpr STRING::Hot CHANNEL = " --channel ";
constexpr STRING::Hot WAIT = " --wait ";
constexpr STRING::Hot RESTART = " --restart";
constexpr STRING::Hot DETACHED = "setsid nohup ";
constexpr STRING::Hot BACKGROUND = " >/dev/null 2>&1 &";
#endif
constexpr Whole NAMED = 6;
constexpr Whole ENTRIES = std::size(LINES) + NAMED;
constexpr STRING::Hot CARTRIDGES = "configs/lines.yaml";
constexpr STRING::Hot LAUNCHER = "launcher";

struct Choice {
  String leaf;
  String line;
  String tip;
};

inline String followed = STABLE;
inline String skipped;
inline String offered;
inline STORE::Offer offer;
inline Vector<Choice> choices;
inline String refused;
inline Whole raised = ISLANDS::SELECT::NONE;

void read();
void write();
void unfold(STRING::Hot card, STRING::Hot door);
void unfold(STRING::Hot door);
void unfold(Whole row);
void dress(
  STRING::Hot card, auto (*entry)(Whole)->String, const Vector<String> &names);
void fold();
void pick(Whole at);
void take(Whole at);
void rows();
void options();
void choose(Whole at);
auto click(const String &id) -> Flag;
void follow();
void check();
void tender(const String &current, const String &wanted);
auto answered(const String &id) -> Flag;
void refresh();
void load();
void set(const String &leaf, const String &line);
void mark(const String &leaf, const String &tip);

namespace GET {
auto placed(const String &name) -> Whole;
auto cloned() -> Flag;
auto installed() -> Flag;
auto steps() -> const Vector<STORE::Step> &;
auto checks() -> const Vector<STORE::Step> &;
auto renewals() -> const Vector<STORE::Step> &;
auto line(const String &leaf) -> String;
auto lines(const String &leaf) -> Vector<String>;
auto label(STRING::Hot door, const String &line) -> String;
auto tip(const String &leaf) -> String;
auto origin() -> String;
}  // namespace GET
}  // namespace LAUNCHER::LINE
