// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once

namespace LAUNCHER::LINE {

constexpr STRING::Hot STABLE = "stable";
constexpr STRING::Hot LATEST = "latest";
constexpr STRING::Hot LINES[] = {STABLE, LATEST};
constexpr STRING::Hot PATH = "configs/release.yaml";
constexpr STRING::Hot KEY = "release";
constexpr STRING::Hot SEAT = "cartridges/.core/launcher";
constexpr STRING::Hot CLONE = ".git";
constexpr STRING::Hot RESTARTING = "restarting";
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
  STRING::Hot card, auto (*entry)(Whole)->String, const Vector<String> &words);
void fold();
void pick(Whole at);
void take(Whole at);
void rows();
void options();
void choose(Whole at);
auto click(const String &id) -> Flag;
void follow();
void refresh();
void load();
void set(const String &leaf, const String &line);
void mark(const String &leaf, const String &tip);

namespace GET {
auto placed(const String &word) -> Whole;
auto cloned() -> Flag;
auto steps() -> const Vector<STORE::Step> &;
auto line(const String &leaf) -> String;
auto lines(const String &leaf) -> Vector<String>;
auto word(STRING::Hot door, const String &line) -> String;
auto tip(const String &leaf) -> String;
auto origin() -> String;
}  // namespace GET
}  // namespace LAUNCHER::LINE
