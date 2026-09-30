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
constexpr STRING::Hot PINNED = "pinned";
constexpr STRING::Hot RESTARTING = "restarting";

inline String followed = STABLE;
inline Flag unfolded = false;
inline STORE::Offer offer;

void read();
void write();
void unfold(Flag on);
void pick(Whole at);
void follow();
void refresh();

namespace GET {
auto placed(const String &word) -> Whole;
auto cloned() -> Flag;
auto steps() -> const Vector<STORE::Step> &;
}  // namespace GET
}  // namespace LAUNCHER::LINE
