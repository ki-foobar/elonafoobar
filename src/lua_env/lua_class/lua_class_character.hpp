#pragma once
#include "../../status_ailment.hpp"
#include "../lua_api/lua_api_common.hpp"

namespace elona
{
namespace lua
{

namespace LuaCharacter
{
void damage_hp(Character&, int);
void damage_hp_source(Character&, int, const enum_string&);
void damage_hp_chara(Character&, int, lua_character_handle handle);

void apply_ailment(Character&, const enum_string&, int);

bool recruit_as_ally(Character&);

void set_flag(Character&, const enum_string&, bool);

void gain_skill(Character&, int, int);
void gain_skill_stock(Character&, int, int, int);

void gain_skill_exp(Character&, int, int);

void modify_trait(Character&, int, int);

void modify_resistance(Character&, const enum_string&, int);

void modify_sanity(Character&, int);

void modify_karma(Character&, int);

void modify_corruption(Character&, int);

void make_pregnant(Character&);

void eat_rotten_food(Character&);


void bind(sol::state&);
} // namespace LuaCharacter

} // namespace lua
} // namespace elona
