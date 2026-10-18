// SPDX-License-Identifier: AGPL-3.0-or-later
#include <island/graphics/backend/passes.hpp>
#include <island/graphics/windows.hpp>
#include <island/gui/dialog.hpp>
#include <island/input.hpp>
#include <island/window/backend.hpp>

#include "../cartridge.hpp"

static auto banked(const String &id, STRING::Hot bank, Whole &at) -> Flag {
  const String prefix = bank;
  if (!id.starts_with(prefix)) return false;
  at = STRING::number<Whole>(id.substr(prefix.size()));
  return true;
}

static void settle(Whole at) {
  if (at >= LAUNCHER::children.size()) return;
  const LAUNCHER::Child &child = LAUNCHER::children[at];
  if (child.running)
    LAUNCHER::stop(child.pid);
  else
    LAUNCHER::dismiss(child.pid);
}

struct Click {
  STRING::Hot id;
  void (*run)();
};

static const Click CLICKS[] = {
  {LAUNCHER::IDS::DISMISS,
   [] {
     GUI::set(
       LAUNCHER::document, LAUNCHER::IDS::NOTICE, GUI::Visibility{false});
   }},
  {LAUNCHER::IDS::ABOUT,
   [] {
     GUI::set(
       LAUNCHER::document, LAUNCHER::IDS::NOTICE,
       GUI::Visibility{!LAUNCHER::GET::noticing()});
   }},
  {LAUNCHER::IDS::LAUNCH, [] { LAUNCHER::start(); }},
  {LAUNCHER::IDS::GEAR,
   [] {
     GUI::set(
       LAUNCHER::document, LAUNCHER::IDS::SETUP,
       GUI::Visibility{!LAUNCHER::GET::setting()});
   }},
  {LAUNCHER::IDS::JOIN, [] { LAUNCHER::STORE::ask(); }},
  {LAUNCHER::IDS::KEEP, [] { LAUNCHER::STORE::link(); }},
  {LAUNCHER::IDS::LEAVE, [] { LAUNCHER::STORE::drop(); }},
  {LAUNCHER::IDS::PUPDATE, [] { LAUNCHER::UPDATE::run(); }}};

static void act(const String &id) {
  Whole at = 0;
  for (const Click &click : CLICKS)
    if (id == click.id) return click.run();
  if (LAUNCHER::LINE::click(id)) return;
  if (::banked(id, LAUNCHER::IDS::BOX, at)) return LAUNCHER::tick(at);
  if (::banked(id, LAUNCHER::IDS::INSTALL, at))
    return LAUNCHER::STORE::call(at);
  if (::banked(id, LAUNCHER::IDS::UPDATE, at))
    return LAUNCHER::STORE::update(at);
  for (const LAUNCHER::Scene &scene : LAUNCHER::SCENES)
    if (id == scene.call) return LAUNCHER::enter(scene.call);
  for (const LAUNCHER::Area &area : LAUNCHER::AREAS)
    if (id == area.id) return LAUNCHER::visit(area.id);
}

static void land(const String &id) {
  if (id == LAUNCHER::IDS::TREE)
    LAUNCHER::choose(GUI::GET::cursor(LAUNCHER::document, id.c_str()));
}

static void take(const String &id) {
  namespace IDS = LAUNCHER::IDS;
  const auto cursor = [&] {
    return GUI::GET::cursor(LAUNCHER::document, id.c_str());
  };
  if (id == IDS::TREE) return LAUNCHER::open(cursor());
  if (id == IDS::CREW) return ::settle(cursor());
}

static void react() {
  for (const auto &event : GUI::GET::events(LAUNCHER::document)) {
    if (event.kind == GUI::Event::CLICKED) ::act(event.id);
    if (event.kind == GUI::Event::SELECTED) ::land(event.id);
    if (event.kind == GUI::Event::ACTIVATED) ::take(event.id);
    if (event.kind == GUI::Event::COMMITTED) LAUNCHER::commit(event.id);
  }
}

void LAUNCHER::update() {
  poll();
  const GFX::Viewport viewport = GFX::WINDOWS::MAIN::viewport();
  GUI::place(document, viewport);
  if (!prompt(viewport)) {
    GUI::focus(document, true);
    GUI::poll();
    react();
  }
  refresh();
  GFX::Pass pass = {
    .clear = GUI::GET::style(document, STYLES::SHEET).color,
    .viewport = viewport};
  IDLE::paint(pass);
  GUI::flush(document, pass);
  GUI::DIALOG::flush(pass);
  GFX::PASSES::submit(std::move(pass));
}
