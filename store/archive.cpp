// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cstdint>
#include <filesystem>
#include <fstream>

#include "../cartridge.hpp"

namespace fs = std::filesystem;

// A release's windows asset is a zip whose entries are STORED (`zip -0`):
// the bundle's dll and assets gain little from deflate, and a reader that
// copies bytes needs no inflater. The walk reads each local file header in
// turn and stops at the central directory; a deflated entry, a streamed
// one (sizes in a trailing descriptor) or a name that leaves the seat is
// refused with the reason.
namespace LAUNCHER::STORE::ARCHIVE {
namespace {
constexpr std::uint32_t LOCAL = 0x04034b50;
constexpr std::uint32_t CENTRAL = 0x02014b50;
constexpr Whole HEADER = 30;
constexpr std::uint16_t STORED = 0;
constexpr std::uint16_t STREAMED = 1u << 3;
constexpr Char SLASH = '/';
constexpr STRING::Hot PARENT = "..";

auto half(const String &bytes, Whole at) -> std::uint16_t {
  return static_cast<std::uint16_t>(
    static_cast<unsigned char>(bytes[at]) |
    (static_cast<unsigned char>(bytes[at + 1]) << 8));
}

auto word(const String &bytes, Whole at) -> std::uint32_t {
  return static_cast<std::uint32_t>(half(bytes, at)) |
         (static_cast<std::uint32_t>(half(bytes, at + 2)) << 16);
}

auto contained(const String &name) -> Flag {
  if (name.empty() || name.front() == SLASH || name.find(':') != String::npos)
    return false;
  Whole start = 0;
  while (start <= name.size()) {
    const auto cut = name.find(SLASH, start);
    const String part =
      name.substr(start, cut == String::npos ? String::npos : cut - start);
    if (part == PARENT) return false;
    if (cut == String::npos) break;
    start = cut + 1;
  }
  return true;
}

auto laid(const fs::path &file, const String &bytes, Whole at, Whole size)
  -> Flag {
  std::error_code slip;
  fs::create_directories(file.parent_path(), slip);
  std::ofstream out(file, std::ios::binary | std::ios::trunc);
  if (!out) return false;
  out.write(bytes.data() + at, static_cast<std::streamsize>(size));
  return out.good();
}
}  // namespace
}  // namespace LAUNCHER::STORE::ARCHIVE

auto LAUNCHER::STORE::ARCHIVE::unpack(
  const String &bytes, const String &into, String &error) -> Flag {
  Whole at = 0;
  Whole files = 0;
  while (at + 4 <= bytes.size()) {
    const std::uint32_t signature = word(bytes, at);
    if (signature == CENTRAL) break;
    if (signature != LOCAL || at + HEADER > bytes.size()) {
      error = "not a zip archive";
      return false;
    }
    const std::uint16_t flags = half(bytes, at + 6);
    const std::uint16_t method = half(bytes, at + 8);
    const std::uint32_t packed = word(bytes, at + 18);
    const std::uint32_t size = word(bytes, at + 22);
    const std::uint16_t named = half(bytes, at + 26);
    const std::uint16_t extra = half(bytes, at + 28);
    const Whole data = at + HEADER + named + extra;
    if (data > bytes.size() || data + packed > bytes.size()) {
      error = "truncated zip archive";
      return false;
    }
    const String name = bytes.substr(at + HEADER, named);
    if (!contained(name)) {
      error = "zip entry leaves the seat: " + name;
      return false;
    }
    if (method != STORED || (flags & STREAMED) || packed != size) {
      error = "zip entry is not stored: " + name;
      return false;
    }
    if (!name.ends_with(SLASH)) {
      if (!laid(fs::path(into) / name, bytes, data, size)) {
        error = "cannot write " + name;
        return false;
      }
      files += 1;
    }
    at = data + packed;
  }
  if (files == 0) {
    error = "empty zip archive";
    return false;
  }
  return true;
}
