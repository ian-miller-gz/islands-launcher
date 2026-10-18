// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once

namespace LAUNCHER::STORE {
constexpr STRING::Hot REF = "refs/heads/";
constexpr STRING::Hot QUIET = " >/dev/null 2>&1";
constexpr STRING::Hot MUTE = " 2>/dev/null";
constexpr STRING::Hot PROMPTLESS = "GIT_TERMINAL_PROMPT=0 ";
}  // namespace LAUNCHER::STORE

namespace LAUNCHER::STORE::GET {
auto branch(const Offer &offer) -> String;
auto origin(const Offer &offer) -> String;
auto named(const Offer &offer) -> Flag;
auto reached(const Offer &offer) -> Flag;
auto reaching() -> Flag;
auto probe(const Offer &offer) -> String;
auto clone(const Offer &offer, const String &home) -> String;
}  // namespace LAUNCHER::STORE::GET
