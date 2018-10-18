#include "map.hpp"
#include "area.hpp"
#include "building.hpp"
#include "calc.hpp"
#include "character.hpp"
#include "ctrl_file.hpp"
#include "data/types/type_item.hpp"
#include "elona.hpp"
#include "equipment.hpp"
#include "i18n.hpp"
#include "item.hpp"
#include "itemgen.hpp"
#include "map_cell.hpp"
#include "mef.hpp"
#include "position.hpp"
#include "variables.hpp"



namespace elona
{


MapData map_data;
CellData cell_data;



#define MDATA_PACK(x, ident) legacy_mdata(x) = ident;
#define MDATA_UNPACK(x, ident) ident = legacy_mdata(x);

#define SERIALIZE_ALL() \
    SERIALIZE(0, width); \
    SERIALIZE(1, height); \
    SERIALIZE(2, atlas_number); \
    SERIALIZE(3, next_regenerate_date); \
    SERIALIZE(4, stair_up_pos); \
    SERIALIZE(5, stair_down_pos); \
    SERIALIZE(6, type); \
    SERIALIZE(7, refresh_type); \
    SERIALIZE(8, designated_spawns); \
    SERIALIZE(9, turn_cost); \
    SERIALIZE(10, max_crowd_density); \
    SERIALIZE(11, current_dungeon_level); \
    SERIALIZE(12, tileset); \
    SERIALIZE(13, bgm); \
    SERIALIZE(14, indoors_flag); \
    SERIALIZE(15, user_map_flag); \
    SERIALIZE(16, next_restock_date); \
    SERIALIZE(17, should_regenerate); \
    SERIALIZE(18, max_item_count); \
    SERIALIZE(19, regenerate_count); \
    SERIALIZE(20, play_campfire_sound); \
    SERIALIZE(21, mefs_loaded_flag);


#define SERIALIZE MDATA_PACK
void MapData::pack_to(elona_vector1<int>& legacy_mdata)
{
    SERIALIZE_ALL();
}
#undef SERIALIZE

#define SERIALIZE MDATA_UNPACK
void MapData::unpack_from(elona_vector1<int>& legacy_mdata)
{
    SERIALIZE_ALL();
}
#undef SERIALIZE
#undef SERIALIZE_ALL


void MapData::clear()
{
    *this = {};
}


#define MAP_PACK(n, ident) legacy_map(x, y, n) = ident;
#define MAP_UNPACK(n, ident) ident = legacy_map(x, y, n);

#define SERIALIZE_ALL() \
    SERIALIZE(0, chip_id_actual); \
    SERIALIZE(1, chara_index_plus_one); \
    SERIALIZE(2, chip_id_memory); \
    /* 3 */ \
    SERIALIZE(4, item_appearances_actual); \
    SERIALIZE(5, item_appearances_memory); \
    SERIALIZE(6, feats); \
    SERIALIZE(7, blood_and_fragments); \
    SERIALIZE(8, mef_index_plus_one); \
    SERIALIZE(9, light);


#define SERIALIZE MAP_PACK
void Cell::pack_to(elona_vector3<int>& legacy_map, int x, int y)
{
    SERIALIZE_ALL();
}
#undef SERIALIZE

#define SERIALIZE MAP_UNPACK
void Cell::unpack_from(elona_vector3<int>& legacy_map, int x, int y)
{
    SERIALIZE_ALL();
}
#undef SERIALIZE



void Cell::clear()
{
    *this = {};
}



void CellData::init(int width, int height)
{
    cells.clear();
    width_ = width;
    height_ = height;

    cells.reserve(height_);

    for (int y = 0; y < height_; y++)
    {
        std::vector<Cell> column;
        column.reserve(width_);

        for (int x = 0; x < width_; x++)
        {
            column.emplace_back(Cell{});
        }
        cells.emplace_back(std::move(column));
    }
}



void CellData::pack_to(elona_vector3<int>& legacy_map)
{
    DIM4(legacy_map, map_data.width, map_data.height, 10);

    assert(legacy_map.i_size() == static_cast<size_t>(width_));
    assert(legacy_map.j_size() == static_cast<size_t>(height_));

    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            at(x, y).pack_to(legacy_map, x, y);
        }
    }
}



void CellData::unpack_from(elona_vector3<int>& legacy_map)
{
    init(legacy_map.i_size(), legacy_map.j_size());

    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            at(x, y).unpack_from(legacy_map, x, y);
        }
    }
}



void map_reload(const std::string& map_filename)
{
    fmapfile = (filesystem::dir::map() / map_filename).generic_string();
    ctrl_file(FileOperation::map_load_map_obj_files);

    for (int y = 0; y < map_data.height; ++y)
    {
        for (int x = 0; x < map_data.width; ++x)
        {
            cell_data.at(x, y).mef_index_plus_one = 0;
            cell_data.at(x, y).light = 0;
        }
    }

    mef_clear_all();

    for (const auto& i : items(-1))
    {
        if (inv[i].number() > 0)
        {
            if (inv[i].own_state == 1)
            {
                if (the_item_db[inv[i].id]->category == 57000)
                {
                    inv[i].remove();
                    cell_refresh(inv[i].position.x, inv[i].position.y);
                }
            }
        }
    }

    for (int i = 0; i < 400; ++i)
    {
        if (cmapdata(0, i) == 0)
            continue;
        const auto x = cmapdata(1, i);
        const auto y = cmapdata(2, i);
        if (cmapdata(4, i) == 0)
        {
            if (cell_data.at(x, y).item_appearances_actual == 0)
            {
                flt();
                int stat = itemcreate(-1, cmapdata(0, i), x, y, 0);
                if (stat != 0)
                {
                    inv[ci].own_state = cmapdata(3, i);
                }
            }
            cell_refresh(x, y);
        }
    }
}


// Used for huntex/conquer quests.
std::string map_get_custom_map_name(int map_id)
{
    if (area_data[map_id].id == mdata_t::MapId::vernis)
    {
        return u8"vernis"s;
    }
    if (area_data[map_id].id == mdata_t::MapId::port_kapul)
    {
        return u8"kapul"s;
    }
    if (area_data[map_id].id == mdata_t::MapId::yowyn)
    {
        return u8"yowyn"s;
    }
    if (area_data[map_id].id == mdata_t::MapId::derphy)
    {
        return u8"rogueden"s;
    }
    if (area_data[map_id].id == mdata_t::MapId::palmia)
    {
        return u8"palmia"s;
    }
    if (area_data[map_id].id == mdata_t::MapId::noyel)
    {
        return u8"noyel"s;
    }
    if (area_data[map_id].id == mdata_t::MapId::lumiest)
    {
        return u8"lumiest"s;
    }
    return u8"vernis"s;
}


bool map_is_town_or_guild()
{
    return map_data.type == mdata_t::MapType::town
        || map_data.type == mdata_t::MapType::guild;
}


bool map_should_reveal_fog()
{
    return game_data.current_map == mdata_t::MapId::pet_arena
        || game_data.current_map == mdata_t::MapId::arena || dbg_revealmap
        || map_data.type == mdata_t::MapType::town
        || map_data.type == mdata_t::MapType::world_map
        || map_data.type == mdata_t::MapType::player_owned
        || map_data.type == mdata_t::MapType::guild
        || game_data.current_map == mdata_t::MapId::shelter_
        || game_data.current_map == mdata_t::MapId::embassy
        || game_data.current_map == mdata_t::MapId::miral_and_garoks_workshop
        || game_data.current_map == mdata_t::MapId::show_house
        || (game_data.current_map == mdata_t::MapId::quest
            && game_data.executing_immediate_quest_type == 1009);
}


bool map_shows_floor_count_in_name()
{
    return area_data[game_data.current_map].type != mdata_t::MapType::town
        && (area_data[game_data.current_map].id == mdata_t::MapId::lesimas
            || area_data[game_data.current_map].id
                == mdata_t::MapId::random_dungeon
            || area_data[game_data.current_map].id == mdata_t::MapId::quest
            || mdata_t::is_nefia(map_data.type));
}


bool map_prevents_teleport()
{
    return game_data.current_map == mdata_t::MapId::pet_arena
        || map_data.type == mdata_t::MapType::world_map
        || game_data.current_map == mdata_t::MapId::pyramid
        || game_data.current_map == mdata_t::MapId::jail;
}


bool map_prevents_return()
{
    return map_data.type == mdata_t::MapType::temporary
        || game_data.current_map == mdata_t::MapId::shelter_
        || game_data.current_map == mdata_t::MapId::jail;
}


bool map_prevents_domination()
{
    return game_data.current_map == mdata_t::MapId::arena
        || game_data.current_map == mdata_t::MapId::pet_arena
        || game_data.current_map == mdata_t::MapId::the_void;
}


bool map_prevents_monster_ball()
{
    return game_data.current_map == mdata_t::MapId::arena
        || game_data.current_map == mdata_t::MapId::pet_arena
        || game_data.current_map == mdata_t::MapId::show_house;
}


bool map_prevents_building_shelter()
{
    return map_data.refresh_type == 0
        || game_data.current_map == mdata_t::MapId::quest
        || game_data.current_map == mdata_t::MapId::shelter_;
}


bool map_prevents_random_events()
{
    return game_data.current_map == mdata_t::MapId::shelter_
        || game_data.current_map != mdata_t::MapId::jail;
}


int map_random_site_amount()
{
    int amount = rnd(map_data.height * map_data.width / 400 + 3);
    if (map_data.type == mdata_t::MapType::world_map)
    {
        amount = rnd(40);
    }
    if (map_data.type == mdata_t::MapType::town)
    {
        amount = rnd(rnd(rnd(12) + 1) + 1);
    }
    if (map_data.type == mdata_t::MapType::guild)
    {
        amount = rnd(p + 1);
    }

    return amount;
}


void map_set_caravan_destinations()
{
    // '0' is "go back" choice index.
    p(0) = 0;
    p(1) = 0;
    p(2) = 0;

    if (game_data.current_map == mdata_t::MapId::north_tyris_south_border)
    {
        p(0) = static_cast<int>(mdata_t::MapId::south_tyris_north_border);
        p(1) = static_cast<int>(mdata_t::MapId::test_world_north_border);
        p(2) = 0;
    }
    if (game_data.current_map == mdata_t::MapId::south_tyris_north_border)
    {
        p(0) = static_cast<int>(mdata_t::MapId::north_tyris_south_border);
        p(1) = static_cast<int>(mdata_t::MapId::test_world_north_border);
        p(2) = 0;
    }
    if (game_data.current_map == mdata_t::MapId::test_world_north_border)
    {
        p(0) = static_cast<int>(mdata_t::MapId::north_tyris_south_border);
        p(1) = static_cast<int>(mdata_t::MapId::south_tyris_north_border);
        p(2) = 0;
    }
}


void map_calc_trade_goods_price()
{
    DIM2(trate, 8);
    for (int cnt = 0; cnt < 8; ++cnt)
    {
        trate(cnt) = 100;
    }
    if (game_data.current_map == mdata_t::MapId::vernis)
    {
        trate(0) = 130;
        trate(1) = 70;
        trate(3) = 110;
        trate(2) = 85;
        trate(6) = 150;
        trate(7) = 120;
    }
    if (game_data.current_map == mdata_t::MapId::port_kapul)
    {
        trate(0) = 65;
        trate(1) = 110;
        trate(4) = 125;
        trate(5) = 65;
        trate(2) = 70;
        trate(6) = 200;
        trate(7) = 150;
    }
    if (game_data.current_map == mdata_t::MapId::palmia)
    {
        trate(0) = 120;
        trate(2) = 75;
        trate(4) = 75;
        trate(1) = 125;
        trate(6) = 110;
        trate(7) = 80;
    }
    if (game_data.current_map == mdata_t::MapId::yowyn)
    {
        trate(0) = 120;
        trate(3) = 75;
        trate(2) = 120;
        trate(6) = 160;
        trate(7) = 100;
    }
    if (game_data.current_map == mdata_t::MapId::derphy)
    {
        trate(0) = 85;
        trate(3) = 70;
        trate(5) = 120;
        trate(2) = 130;
        trate(6) = 130;
        trate(7) = 90;
    }
    if (game_data.current_map == mdata_t::MapId::noyel)
    {
        trate(1) = 175;
        trate(0) = 170;
        trate(3) = 160;
        trate(5) = 130;
        trate(2) = 140;
        trate(6) = 75;
        trate(7) = 120;
    }
    if (game_data.current_map == mdata_t::MapId::lumiest)
    {
        trate(1) = 145;
        trate(0) = 130;
        trate(3) = 100;
        trate(5) = 120;
        trate(2) = 120;
        trate(6) = 100;
        trate(7) = 70;
    }
    randomize(game_data.date.hours() / 100);
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        trate(cnt) += rnd(15) - rnd(15);
    }
    randomize();
}


bool map_ai_makes_snowmen()
{
    return game_data.current_map == mdata_t::MapId::noyel
        || game_data.current_map == mdata_t::MapId::mansion_of_younger_sister;
}


bool map_can_use_bad_weather_in_study()
{
    return game_data.current_map != mdata_t::MapId::shelter_
        && map_data.indoors_flag == 1
        && (map_data.type == mdata_t::MapType::player_owned
            || map_is_town_or_guild());
}


void map_randsite(int prm_971, int prm_972)
{
    int f_at_m169 = 0;
    int found_x;
    int found_y;

    f_at_m169 = 0;
    for (int cnt = 0; cnt < 20; ++cnt)
    {
        if (prm_971 == 0)
        {
            found_x = rnd(map_data.width - 5) + 2;
            found_y = rnd(map_data.height - 5) + 2;
        }
        else
        {
            found_x = prm_971;
            found_y = prm_972;
        }
        if ((chipm(7, cell_data.at(found_x, found_y).chip_id_actual) & 4) == 0)
        {
            if (cell_data.at(found_x, found_y).feats == 0
                && cell_data.at(found_x, found_y).item_appearances_actual == 0)
            {
                f_at_m169 = 1;
                break;
            }
        }
    }
    if (map_data.type == mdata_t::MapType::world_map)
    {
        if ((264 <= cell_data.at(found_x, found_y).chip_id_actual
             && cell_data.at(found_x, found_y).chip_id_actual < 363)
            || (33 <= cell_data.at(found_x, found_y).chip_id_actual
                && cell_data.at(found_x, found_y).chip_id_actual < 66))
        {
            f_at_m169 = 0;
        }
    }
    if (f_at_m169 == 0)
    {
        return;
    }
    if (mdata_t::is_nefia(map_data.type))
    {
        if (map_data.next_regenerate_date == 0)
        {
            if (rnd(25) == 0)
            {
                flt();
                itemcreate(-1, 173, found_x, found_y, 0);
                inv[ci].own_state = 1;
                return;
            }
            if (rnd(100) == 0)
            {
                flt();
                itemcreate(-1, 172, found_x, found_y, 0);
                inv[ci].own_state = 1;
                inv[ci].param1 = choice(isetgod);
                return;
            }
        }
    }
    if (mdata_t::is_nefia(map_data.type))
    {
        if (rnd(14) == 0)
        {
            cell_featset(found_x, found_y, 243, 27);
            return;
        }
        if (rnd(13) == 0)
        {
            cell_featset(found_x, found_y, 244, 25);
            return;
        }
        if (rnd(12) == 0)
        {
            cell_featset(found_x, found_y, 245, 26);
            return;
        }
        if (rnd(11) == 0)
        {
            cell_featset(found_x, found_y, 246, 28);
            return;
        }
        cell_featset(found_x, found_y, tile_re + rnd(3), 24);
        return;
    }
    if (map_is_town_or_guild())
    {
        if (rnd(3) == 0)
        {
            flt();
            itemcreate(-1, 631, found_x, found_y, 0);
            return;
        }
        if (rnd(15) == 0)
        {
            flt();
            itemcreate(-1, 55, found_x, found_y, 0);
            return;
        }
        if (rnd(20) == 0)
        {
            flt();
            itemcreate(-1, 284, found_x, found_y, 0);
            return;
        }
        if (rnd(15) == 0)
        {
            flt();
            itemcreate(-1, 283, found_x, found_y, 0);
            return;
        }
        if (rnd(18) == 0)
        {
            flt(calcobjlv(rnd(cdata.player().level + 10)),
                calcfixlv(Quality::good));
            flttypemajor = choice(fsetwear);
            itemcreate(-1, 0, found_x, found_y, 0);
            return;
        }
        flt(10);
        flttypeminor = 64100;
        itemcreate(-1, 0, found_x, found_y, 0);
        return;
    }
}


static void _map_update_arena_random_seed()
{
    area_data[game_data.current_map].arena_random_seed = rnd(10000);
    area_data[game_data.current_map].time_of_next_update_of_arena_random_seed =
        game_data.date.hours() + 24;
}


static void _clear_material_spots()
{
    for (int cnt = 0, cnt_end = (map_data.height); cnt < cnt_end; ++cnt)
    {
        y = cnt;
        for (int cnt = 0, cnt_end = (map_data.width); cnt < cnt_end; ++cnt)
        {
            if (cell_data.at(cnt, y).feats != 0)
            {
                cell_featread(cnt, y);
                if (feat(1) >= 24 && feat(1) <= 28)
                {
                    cell_featclear(cnt, y);
                }
            }
        }
    }
}


static void _modify_items_on_regenerate()
{
    for (const auto& cnt : items(-1))
    {
        if (inv[cnt].number() == 0)
        {
            continue;
        }

        // Update tree of fruits.
        if (inv[cnt].id == 526)
        {
            if (inv[cnt].param1 < 10)
            {
                inv[cnt].param1 += 1;
                inv[cnt].image = 591;
                cell_refresh(inv[cnt].position.x, inv[cnt].position.y);
            }
        }

        // Clear player-owned items on the ground.
        if (map_is_town_or_guild())
        {
            if (inv[cnt].own_state < 0)
            {
                ++inv[cnt].own_state;
            }
            if (inv[cnt].own_state == 0)
            {
                inv[cnt].remove();
                cell_refresh(inv[cnt].position.x, inv[cnt].position.y);
            }
        }
    }
}


static void _modify_characters_on_regenerate()
{
    for (auto&& cnt : cdata.others())
    {
        rc = cnt.index;
        chara_clear_status_effects_b();
        if (cnt.state() != Character::State::alive)
        {
            continue;
        }
        if (cnt.is_temporary() == 1)
        {
            if (rnd(2))
            {
                cnt.set_state(Character::State::empty);
                cell_data.at(cnt.position.x, cnt.position.y)
                    .chara_index_plus_one = 0;
            }
        }
    }
}


static void _set_feats_on_regenerate()
{
    p = map_random_site_amount();
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        map_randsite();
    }
}


static void _map_regenerate()
{
    if (map_data.should_regenerate == 0)
    {
        if (map_data.next_regenerate_date != 0)
        {
            _clear_material_spots();
            _modify_items_on_regenerate();
        }

        _modify_characters_on_regenerate();

        _set_feats_on_regenerate();
    }
    map_data.next_regenerate_date = game_data.date.hours() + 120;
}


static void _grow_plants()
{
    for (int cnt = 0, cnt_end = (map_data.height); cnt < cnt_end; ++cnt)
    {
        y = cnt;
        for (int cnt = 0, cnt_end = (map_data.width); cnt < cnt_end; ++cnt)
        {
            if (cell_data.at(cnt, y).feats != 0)
            {
                cell_featread(cnt, y);
                x = cnt;
                if (feat(1) == 29)
                {
                    for (int cnt = 0, cnt_end = (renewmulti); cnt < cnt_end;
                         ++cnt)
                    {
                        if (feat >= tile_plant + 2)
                        {
                            break;
                        }
                        grow_plant(
                            chipm(0, cell_data.at(x, y).chip_id_actual) == 2
                                ? 1
                                : 0);
                    }
                    cell_featset(cnt, y, feat, feat(1), feat(2), feat(3));
                }
            }
        }
    }
}


static void _proc_generate_bard_items(Character& chara)
{
    if (itemfind(chara.index, 60005, 1) == -1)
    {
        if (rnd(150) == 0)
        {
            // Stradivarius
            flt();
            itemcreate(chara.index, 707, -1, -1, 0);
        }
        else
        {
            // Random instrument
            flt(calcobjlv(chara.level), calcfixlv());
            flttypeminor = 60005;
            itemcreate(chara.index, 0, -1, -1, 0);
        }
    }
}


static void _generate_bad_quality_item()
{
    flt(calcobjlv(cdata[rc].level), calcfixlv(Quality::bad));
    int stat = itemcreate(rc, 0, -1, -1, 0);
    if (stat != 0)
    {
        if (inv[ci].weight <= 0 || inv[ci].weight >= 4000)
        {
            inv[ci].remove();
        }
    }
}


static void _restock_character_inventories()
{
    for (auto&& cnt : cdata.others())
    {
        if (cnt.state() != Character::State::alive)
        {
            continue;
        }
        generatemoney(cnt.index);
        if (cnt.id == 326)
        {
            _proc_generate_bard_items(cnt);
        }
        rc = cnt.index;
        if (rnd(5) == 0)
        {
            supply_new_equipment();
        }
        if (rnd(2) == 0 && inv_sum(rc) < 8)
        {
            _generate_bad_quality_item();
        }
    }
}

static void _map_restock_regenerate()
{
    _grow_plants();

    if (map_is_town_or_guild()
        || game_data.current_map == mdata_t::MapId::your_home)
    {
        _restock_character_inventories();
    }
}


static void _map_restock()
{
    if (map_data.next_restock_date == 0)
    {
        renewmulti = 1;
    }
    else
    {
        renewmulti =
            (game_data.date.hours() - map_data.next_restock_date) / 24 + 1;
    }
    if (area_data[game_data.current_map].id == mdata_t::MapId::ranch)
    {
        update_ranch();
    }
    if (area_data[game_data.current_map].id == mdata_t::MapId::your_home)
    {
        for (auto&& cnt : cdata.others())
        {
            if (cnt.state() != Character::State::alive)
            {
                continue;
            }
            if (cnt.is_temporary() == 1)
            {
                cnt.set_state(Character::State::empty);
                cell_data.at(cnt.position.x, cnt.position.y)
                    .chara_index_plus_one = 0;
            }
        }
    }
    if (map_data.should_regenerate == 0)
    {
        _map_restock_regenerate();
    }
    map_data.next_restock_date = game_data.date.hours() + 24;
}


void map_proc_regen_and_update()
{
    if (game_data.date.hours() >= area_data[game_data.current_map]
                                      .time_of_next_update_of_arena_random_seed)
    {
        _map_update_arena_random_seed();
    }
    if (game_data.date.hours() >= map_data.next_regenerate_date)
    {
        _map_regenerate();
    }
    if (game_data.date.hours() >= map_data.next_restock_date)
    {
        _map_restock();
    }
}


void map_reload_noyel()
{
    for (const auto& cnt : items(-1))
    {
        if (inv[cnt].id == 555 || inv[cnt].id == 600)
        {
            continue;
        }
        inv[cnt].remove();

        cell_refresh(inv[cnt].position.x, inv[cnt].position.y);
    }
    if (area_data[game_data.current_map].christmas_festival == 1)
    {
        flt();
        int stat = itemcreate(-1, 763, 29, 16, 0);
        if (stat != 0)
        {
            inv[ci].own_state = 1;
        }
        {
            flt();
            int stat = itemcreate(-1, 686, 29, 16, 0);
            if (stat != 0)
            {
                inv[ci].own_state = 1;
            }
        }
        {
            flt();
            int stat = itemcreate(-1, 171, 29, 17, 0);
            if (stat != 0)
            {
                inv[ci].param1 = 6;
                inv[ci].own_state = 1;
            }
        }
        {
            flt();
            int stat = itemcreate(-1, 756, 29, 17, 0);
            if (stat != 0)
            {
                inv[ci].own_state = 5;
            }
        }
        {
            flt();
            int stat = chara_create(-1, 345, 48, 19);
            if (stat != 0)
            {
                cdata[rc].character_role = 3;
                cdata[rc].only_christmas() = true;
            }
        }
        {
            flt();
            int stat = chara_create(-1, 348, 30, 17);
            if (stat != 0)
            {
                cdata[rc].character_role = 3;
                cdata[rc].only_christmas() = true;
            }
        }
        {
            flt();
            int stat = chara_create(-1, 174, 38, 19);
            if (stat != 0)
            {
                cdata[rc].only_christmas() = true;
                cdata[rc].is_hung_on_sand_bag() = true;
                cdatan(0, rc) = i18n::s.get("core.locale.chara.job.fanatic");
            }
        }
        {
            flt();
            int stat = chara_create(-1, 347, 35, 19);
            if (stat != 0)
            {
                cdata[rc].only_christmas() = true;
            }
        }
        {
            flt();
            int stat = chara_create(-1, 347, 37, 18);
            if (stat != 0)
            {
                cdata[rc].only_christmas() = true;
            }
        }
        {
            flt();
            int stat = chara_create(-1, 347, 37, 21);
            if (stat != 0)
            {
                cdata[rc].only_christmas() = true;
            }
        }
        {
            flt();
            int stat = chara_create(-1, 347, 39, 20);
            if (stat != 0)
            {
                cdata[rc].only_christmas() = true;
            }
        }
        {
            flt();
            int stat = chara_create(-1, 347, 38, 21);
            if (stat != 0)
            {
                cdata[rc].only_christmas() = true;
            }
        }
        {
            flt();
            int stat = chara_create(-1, 70, 17, 8);
            if (stat != 0)
            {
                cdata[rc].ai_calm = 3;
                cdata[rc].only_christmas() = true;
                cdata[rc].character_role = 1002;
                cdata[rc].shop_rank = 10;
                cdatan(0, rc) = snfood(cdatan(0, rc));
            }
        }
        {
            flt();
            int stat = chara_create(-1, 239, 25, 8);
            if (stat != 0)
            {
                cdata[rc].ai_calm = 3;
                cdata[rc].relationship = 0;
                cdata[rc].original_relationship = 0;
                cdata[rc].only_christmas() = true;
                cdata[rc].character_role = 1018;
                cdata[rc].shop_rank = 30;
                cdatan(0, rc) = randomname();
                cdatan(0, rc) = i18n::s.get(
                    "core.locale.chara.job.souvenir_vendor", cdatan(0, rc));
            }
        }
        {
            flt();
            int stat = chara_create(-1, 271, 24, 22);
            if (stat != 0)
            {
                cdata[rc].ai_calm = 3;
                cdata[rc].relationship = 0;
                cdata[rc].original_relationship = 0;
                cdata[rc].only_christmas() = true;
                cdata[rc].character_role = 1018;
                cdata[rc].shop_rank = 30;
                cdatan(0, rc) = randomname();
                cdatan(0, rc) = i18n::s.get(
                    "core.locale.chara.job.souvenir_vendor", cdatan(0, rc));
            }
        }
        {
            flt();
            int stat = chara_create(-1, 1, 38, 12);
            if (stat != 0)
            {
                cdata[rc].ai_calm = 3;
                cdata[rc].character_role = 1007;
                cdata[rc].shop_rank = 10;
                cdatan(0, rc) = snblack(cdatan(0, rc));
                cdata[rc].only_christmas() = true;
            }
        }
        {
            flt();
            int stat = chara_create(-1, 271, 28, 9);
            if (stat != 0)
            {
                cdata[rc].ai_calm = 3;
                cdata[rc].relationship = 0;
                cdata[rc].original_relationship = 0;
                cdata[rc].only_christmas() = true;
                cdata[rc].character_role = 1022;
                cdata[rc].shop_rank = 30;
                cdatan(0, rc) = randomname();
                cdatan(0, rc) = i18n::s.get(
                    "core.locale.chara.job.street_vendor", cdatan(0, rc));
            }
        }
        {
            flt();
            int stat = chara_create(-1, 271, 29, 24);
            if (stat != 0)
            {
                cdata[rc].ai_calm = 3;
                cdata[rc].relationship = 0;
                cdata[rc].original_relationship = 0;
                cdata[rc].only_christmas() = true;
                cdata[rc].character_role = 1022;
                cdata[rc].shop_rank = 30;
                cdatan(0, rc) = randomname();
                cdatan(0, rc) = i18n::s.get(
                    "core.locale.chara.job.street_vendor2", cdatan(0, rc));
            }
        }
        for (int cnt = 0; cnt < 20; ++cnt)
        {
            flt();
            int stat = chara_create(-1, 349, -3, 0);
            if (stat != 0)
            {
                cdata[rc].only_christmas() = true;
            }
            flt();
            {
                int stat = chara_create(-1, 350, -3, 0);
                if (stat != 0)
                {
                    cdata[rc].only_christmas() = true;
                }
            }
        }
        for (int cnt = 0; cnt < 15; ++cnt)
        {
            flt();
            int stat = chara_create(-1, 326, -3, 0);
            if (stat != 0)
            {
                cdata[rc].only_christmas() = true;
            }
        }
        for (int cnt = 0; cnt < 7; ++cnt)
        {
            flt();
            int stat = chara_create(-1, 335, -3, 0);
            if (stat != 0)
            {
                cdata[rc].only_christmas() = true;
            }
            {
                flt();
                int stat = chara_create(-1, 185, -3, 0);
                if (stat != 0)
                {
                    cdata[rc].only_christmas() = true;
                }
            }
            {
                flt();
                int stat = chara_create(-1, 274, -3, 0);
                if (stat != 0)
                {
                    cdata[rc].only_christmas() = true;
                }
            }
            {
                flt();
                int stat = chara_create(-1, 174, -3, 0);
                if (stat != 0)
                {
                    cdata[rc].only_christmas() = true;
                }
            }
        }
        for (int cnt = 0; cnt < 3; ++cnt)
        {
            flt();
            int stat = chara_create(-1, 332, -3, 0);
            if (stat != 0)
            {
                cdata[rc].only_christmas() = true;
            }
            {
                flt();
                int stat = chara_create(-1, 185, -3, 0);
                if (stat != 0)
                {
                    cdata[rc].only_christmas() = true;
                }
            }
        }
    }
    else
    {
        for (auto&& cnt : cdata.others())
        {
            if (cnt.only_christmas() == 1)
            {
                chara_vanquish(cnt.index);
            }
        }
    }
}


} // namespace elona
