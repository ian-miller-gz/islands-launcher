// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once

namespace LAUNCHER::STORE {
auto fitted(Offer &offer) -> Flag;

namespace GET {
auto stated(const Offer &offer) -> String;
auto above(const String &stated) -> Flag;
}  // namespace GET
}  // namespace LAUNCHER::STORE
