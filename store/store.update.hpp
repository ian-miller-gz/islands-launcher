// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once

namespace LAUNCHER::STORE {
void installed(Offer &offer);
auto held(const Offer &offer) -> String;
void update(Whole row);
void update(const String &leaf);

namespace GET {
auto update(Whole row) -> Flag;
auto update(const String &leaf) -> Flag;
}  // namespace GET
}  // namespace LAUNCHER::STORE
