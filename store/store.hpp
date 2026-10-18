// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once

namespace LAUNCHER::STORE {

struct Offer {
  String repository;
  String description;
  String name;
  String release;
  String held;
  String entry;
  String leaf;
  String state;
  String tip;
  Flag installed = false;
  Flag linked = false;
};

constexpr Whole ROWS = 8;
constexpr STRING::Hot LIST = "cartridges/.core/launcher/assets/store.yaml";
constexpr STRING::Hot STAGING = "cartridges/.store";
constexpr STRING::Hot LINKS = "configs/links.yaml";
constexpr STRING::Hot LINKED = "links";
constexpr STRING::Hot WORD = "link";
constexpr STRING::Hot SUFFIX = ".git";
constexpr STRING::Hot PROBE = "git ls-remote --exit-code ";
constexpr STRING::Hot HEAD = " HEAD";
constexpr STRING::Hot REPOSITORY = "repository";
constexpr STRING::Hot DESCRIBED = "description";
constexpr STRING::Hot RELEASED = "release";
constexpr STRING::Hot FORGE = "https://github.com/";
constexpr STRING::Hot RAW = "https://raw.githubusercontent.com/";
constexpr STRING::Hot REVISION = "HEAD";
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
constexpr STRING::Hot VERIFYING = "verifying";
constexpr STRING::Hot FETCHING = "fetching";
constexpr STRING::Hot CHECKING = "checking";
constexpr STRING::Hot BUILDING = "building";
constexpr STRING::Hot PLACING = "placing";
constexpr STRING::Hot CONFIRMING = "confirming";
constexpr STRING::Hot WANT = "install";
constexpr STRING::Hot RELEASES = "/releases/download/";
constexpr STRING::Hot ASSET_TAIL = "-windows-x64.zip";
constexpr Whole SERVED = 200;

struct Step {
  STRING::Hot word;
  auto (*call)(const Offer &offer) -> String;
  auto (*held)(Offer &offer) -> Flag;
  auto (*wait)(Offer &offer) -> Whole = nullptr;
};

inline Vector<Offer> offers;
inline Flag surveyed = false;
inline Whole installing = ISLANDS::SELECT::NONE;
inline String wanted;

void load();
auto report(Offer &offer) -> Flag;
auto verify(Offer &offer) -> Flag;
void install(Whole row);
void run(Offer &offer, const Vector<Step> &walk);
void poll();
auto quiet(const Offer &offer) -> String;
auto ended(Offer &offer) -> Flag;
void reach();
void link();
auto keep(const String &repository) -> STRING::Hot;
void argued();
void remove(Whole row);
void call(Whole row);
void refresh();
void claim();

namespace ASSET {
auto address(const Offer &offer) -> String;
auto wait(Offer &offer) -> Whole;
void rest();
}  // namespace ASSET

namespace ARCHIVE {
auto unpack(const String &bytes, const String &into, String &error) -> Flag;
}  // namespace ARCHIVE

namespace GET {
auto list() -> String;
auto home() -> String;
auto stage() -> String;
auto leaf(const String &repository) -> String;
auto root(const String &repository) -> String;
auto steps() -> const Vector<Step> &;
auto running() -> Flag;
auto progress() -> Float;
auto links() -> Vector<String>;
auto staging(const Offer &offer) -> String;
}  // namespace GET
}  // namespace LAUNCHER::STORE
