// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once

namespace LAUNCHER::SOURCES {

struct Source {
  String leaf;
  String repository;
  String branch;
  Flag local = false;
};

constexpr STRING::Hot PATH = "configs/sources.yaml";
constexpr STRING::Hot KEY = "sources";
constexpr STRING::Hot EMPTY = "empty";
constexpr STRING::Hot TAKEN = "taken";
constexpr STRING::Hot FULL = "full";
constexpr STRING::Hot KEPT = "kept";
constexpr STRING::Hot FINDING = "finding";
constexpr STRING::Hot FOUND = "found";
constexpr STRING::Hot NONE = "none";
constexpr STRING::Hot UNREACHED = "unreached";
constexpr STRING::Hot LOCAL = "local";
constexpr STRING::Hot REMOTE = "remote";
constexpr STRING::Hot TITLE = "Sources";

namespace KEYS {
constexpr STRING::Hot LEAF = "leaf";
constexpr STRING::Hot REPOSITORY = "repository";
constexpr STRING::Hot BRANCH = "branch";
constexpr STRING::Hot LOCAL = "local";
}  // namespace KEYS

inline Vector<Source> sources;
inline String noted;
inline Source sought;
inline Vector<String> found;
inline Vector<String> marks;

void read();
void write();
auto keep(Source &source) -> STRING::Hot;
void drop(const String &leaf, const String &branch);
void find(const Source &source);
void poll();

namespace CARD {
inline Flag local = false;
inline String leaf;
void ask(const String &leaf);
void drop();
void find();
void mark(Whole seat);
void add();
void remove();
void flip();
void refresh();
}  // namespace CARD

namespace GET {
auto refusal(const Source &source) -> STRING::Hot;
auto named(const String &leaf) -> Vector<const Source *>;
auto source(const String &leaf, const String &branch) -> const Source *;
auto taken(const String &leaf, const String &name) -> Flag;
auto listed(const String &leaf) -> String;
auto finding() -> Flag;
}  // namespace GET
}  // namespace LAUNCHER::SOURCES
