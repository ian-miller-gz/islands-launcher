// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once

namespace LAUNCHER::STORE {
constexpr STRING::Hot EMPTY = "empty";
constexpr STRING::Hot LISTED = "listed";
constexpr STRING::Hot FULL = "full";
constexpr STRING::Hot ADDED = "added";

inline STRING::Hot noted = "";

void ask();
void drop();
}  // namespace LAUNCHER::STORE
