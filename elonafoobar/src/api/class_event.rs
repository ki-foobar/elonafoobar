use anyhow::{format_err, Result};
use elonafoobar_gui::{Event as GuiEvent, EventExt, EventKind as GuiEventKind, Key as GuiKey};
use elonafoobar_log::trace;
use elonafoobar_lua::macros::lua_function;
use elonafoobar_lua::types::{LuaInt, LuaUserdata};
use elonafoobar_lua::{AsLuaInt, IntoLuaInt, Lua};

const MODULE_NAME: &str = "app.Event";

pub struct Event(pub(super) GuiEvent);

struct EventKind(GuiEventKind);

impl IntoLuaInt for EventKind {
    fn into_lua_int(self) -> Result<LuaInt> {
        Ok(self.0 as LuaInt)
    }
}

struct Key(GuiKey);

impl IntoLuaInt for Key {
    fn into_lua_int(self) -> Result<LuaInt> {
        Ok(self.0 as LuaInt)
    }
}

impl LuaUserdata for Event {
    const NAME: &'static str = "__native.app.Event";
}

pub fn bind(lua: &mut Lua) -> Result<()> {
    trace!("Bind {} class", Event::NAME);
    lua.bind_class::<Event, _>(|lua| -> Result<()> {
        lua.set_function("kind", lua_kind)?;
        lua.set_function("key", lua_key)?;
        Ok(())
    })
}

#[lua_function]
fn lua_kind(self_: &Event) -> AsLuaInt<EventKind> {
    AsLuaInt(EventKind(self_.0.kind()))
}

#[lua_function]
fn lua_key(self_: &Event) -> Result<AsLuaInt<Key>> {
    match self_.0.key() {
        Some(key) => Ok(AsLuaInt(Key(key))),
        None => Err(format_err!("TODO: null key")),
    }
}
