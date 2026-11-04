// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once

namespace LAUNCHER::STORE {
void fail(Offer &offer, STRING::Hot step, const Vector<String> &lines);

namespace GET {
auto failure(const String &leaf) -> String;
}  // namespace GET
}  // namespace LAUNCHER::STORE
