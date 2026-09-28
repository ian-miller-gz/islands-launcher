// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once

namespace LAUNCHER::STORE {

struct Offer {
  String repository;
  String description;
  String name;
  String release;
  String entry;
  String leaf;
  String state;
  Flag installed = false;
  Flag linked = false;
};

constexpr Whole ROWS = 8;
constexpr STRING::Hot LIST = "cartridges/.core/launcher/assets/store.yaml";
constexpr STRING::Hot STAGING = "cartridges/.store";
constexpr STRING::Hot LINKS = "configs/links.yaml";
constexpr STRING::Hot LINKED = "links";
constexpr STRING::Hot SUFFIX = ".git";
constexpr STRING::Hot PROBE = "git ls-remote --exit-code ";
constexpr STRING::Hot HEAD = " HEAD";
constexpr STRING::Hot REPOSITORY = "repository";
constexpr STRING::Hot DESCRIBED = "description";
constexpr STRING::Hot RELEASED = "release";
constexpr STRING::Hot FORGE = "https://github.com/";
constexpr STRING::Hot RAW = "https://raw.githubusercontent.com/";
constexpr STRING::Hot REVISION = "/HEAD/";
constexpr STRING::Hot SCHEME = "://";
constexpr STRING::Hot PREFIX = "islands-";
constexpr STRING::Hot OFFERED = "Install";
constexpr STRING::Hot MARK = "\u2713";
constexpr STRING::Hot CROSS = "\u2717";
constexpr STRING::Hot BUSY = "busy";
constexpr STRING::Hot UNREACHED = "unreached";
constexpr STRING::Hot REFUSED = "refused";
constexpr STRING::Hot FAILED = "failed";
constexpr STRING::Hot REPORTING = "reporting";
constexpr STRING::Hot REACHING = "reaching";
constexpr STRING::Hot FULL = "full";
constexpr STRING::Hot VERIFYING = "verifying";
constexpr STRING::Hot FETCHING = "fetching";
constexpr STRING::Hot CHECKING = "checking";
constexpr STRING::Hot BUILDING = "building";
constexpr STRING::Hot PLACING = "placing";
constexpr STRING::Hot CONFIRMING = "confirming";

struct Step {
  STRING::Hot word;
  auto (*call)(const Offer &offer) -> String;
  auto (*held)(Offer &offer) -> Flag;
};

inline Vector<Offer> offers;
inline Flag surveyed = false;
inline Whole installing = ISLANDS::SELECT::NONE;

void load();
void survey();
auto report(Offer &offer) -> Flag;
auto verify(Offer &offer) -> Flag;
void install(Whole row);
void poll();
void reach();
void link(const String &repository);
void remove(Whole row);
void call(Whole row);
void installed(Offer &offer);
void refresh();

namespace GET {
auto list() -> String;
auto home() -> String;
auto leaf(const String &repository) -> String;
auto steps() -> const Vector<Step> &;
auto progress() -> Float;
auto links() -> Vector<String>;
auto staging(const Offer &offer) -> String;
}  // namespace GET
}  // namespace LAUNCHER::STORE
