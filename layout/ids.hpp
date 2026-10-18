// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once

namespace LAUNCHER::IDS {
constexpr STRING::Hot ABOUT = "about";
constexpr STRING::Hot NOTICE = "notice";
constexpr STRING::Hot NOTICELINE = "noticeline";
constexpr STRING::Hot DISMISS = "noticeclose";
constexpr STRING::Hot GENERAL = "general";
constexpr STRING::Hot CORE = "core";
constexpr STRING::Hot RAIL = "rail";
constexpr STRING::Hot LIBRARY = "library";
constexpr STRING::Hot SETTINGS = "settings";
constexpr STRING::Hot STORE = "store";
constexpr STRING::Hot MAIN = "main";
constexpr STRING::Hot OFFERS = "offers";
constexpr STRING::Hot BROWSER = "browser";
constexpr STRING::Hot SHEET = "preview";
constexpr STRING::Hot WATCH = "watch";
constexpr STRING::Hot NOTE = "note";
constexpr STRING::Hot BARE = "bare";
constexpr STRING::Hot QUIET = "quiet";
constexpr STRING::Hot LAUNCH = "launch";
constexpr STRING::Hot PUPDATE = "pupdate";
constexpr STRING::Hot GEAR = "gear";
constexpr STRING::Hot SETUP = "setup";
constexpr STRING::Hot SETTLE = "setupclose";
constexpr STRING::Hot TITLED = "setupname";
constexpr STRING::Hot BOX = "box";
constexpr STRING::Hot NAMED = "pname";
constexpr STRING::Hot SUMMARY = "psummary";
constexpr Whole FACTS = 3;
inline auto fact(Whole at) -> String { return "pfact" + std::to_string(at); }
constexpr STRING::Hot TREE = "tree";
constexpr STRING::Hot CREW = "crew";
constexpr STRING::Hot LINES = "lines";
constexpr STRING::Hot LINE = "line";
constexpr STRING::Hot LINESTATE = "linestate";
constexpr STRING::Hot SLINE = "sline";
constexpr STRING::Hot SLINES = "slines";
inline auto sline(Whole at) -> String { return SLINE + std::to_string(at); }
constexpr STRING::Hot SADD = "sadd";
constexpr STRING::Hot RLINE = "rline";
constexpr STRING::Hot RADD = "radd";
constexpr STRING::Hot RLINES = "rlines";
inline auto rline(Whole at) -> String { return RLINE + std::to_string(at); }
inline auto radd(Whole at) -> String { return RADD + std::to_string(at); }
inline auto rentry(Whole at) -> String { return "rentry" + std::to_string(at); }
constexpr STRING::Hot CAPTION = "setupline";
constexpr STRING::Hot PLINE = "pline";
constexpr STRING::Hot PLINES = "plines";
inline auto pline(Whole at) -> String { return PLINE + std::to_string(at); }
constexpr STRING::Hot SOURCES = "sources";
inline auto tick(Whole at) -> String { return "tick" + std::to_string(at); }
inline auto rule(Whole at) -> String { return "rule" + std::to_string(at); }
inline auto box(Whole at) -> String { return BOX + std::to_string(at); }
inline auto word(Whole at) -> String { return "boxword" + std::to_string(at); }
inline auto item(STRING::Hot list, Whole at) -> String {
  return String(list) + "." + std::to_string(at);
}
inline auto note(Whole at) -> String { return "note" + std::to_string(at); }
constexpr STRING::Hot CALLED = "name";
constexpr STRING::Hot VERSION = "release";
constexpr STRING::Hot DETAIL = "description";
constexpr STRING::Hot INSTALL = "install";
constexpr STRING::Hot UPDATE = "update";
constexpr STRING::Hot STANDING = "state";
constexpr STRING::Hot PROGRESS = "progress";
constexpr STRING::Hot LINK = "link";
constexpr STRING::Hot JOIN = "linkadd";
constexpr STRING::Hot CARD = "linkcard";
constexpr STRING::Hot KEEP = "linkkeep";
constexpr STRING::Hot LEAVE = "linkclose";
constexpr STRING::Hot NOTED = "linknote";
constexpr STRING::Hot LINKED = "linkstate";
inline auto cell(STRING::Hot column, Whole at) -> String {
  return String(column) + std::to_string(at);
}
}  // namespace LAUNCHER::IDS

namespace LAUNCHER::IDS::SOURCE {
constexpr STRING::Hot CARD = "sourcecard";
constexpr STRING::Hot TITLE = "sourcename";
constexpr STRING::Hot LIST = "sourcelist";
constexpr STRING::Hot ORIGIN = "source";
constexpr STRING::Hot SIDE = "side";
constexpr STRING::Hot FIND = "find";
constexpr STRING::Hot FOUND = "found";
inline auto row(Whole at) -> String { return item(FOUND, at); }
constexpr STRING::Hot STATE = "sourcestate";
constexpr STRING::Hot DROP = "sourcedrop";
constexpr STRING::Hot ADD = "sourceadd";
constexpr STRING::Hot CLOSE = "sourceclose";
}  // namespace LAUNCHER::IDS::SOURCE
