#pragma once

#include <bitset>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../util/range.hpp"
#include "ability.hpp"
#include "activity.hpp"
#include "body_part.hpp"
#include "buff.hpp"
#include "consts.hpp"
#include "data/types/type_character.hpp"
#include "eobject/eobject.hpp"
#include "god.hpp"
#include "lua_env/wrapped_function.hpp"
#include "position.hpp"
#include "spact.hpp"
#include "spell.hpp"
#include "time.hpp"
#include "trait.hpp"



#define ELONA_MAX_CHARACTERS 245
#define ELONA_MAX_PARTY_CHARACTERS 57
#define ELONA_MAX_OTHER_CHARACTERS 188



namespace elona
{

struct Item;
struct Inventory;
using InventoryRef = Inventory*;



struct CombatStyle
{
public:
    bool unarmed() const noexcept
    {
        return _inner == 0;
    }


    bool shield() const noexcept
    {
        return _inner & 1;
    }


    bool two_hand() const noexcept
    {
        return _inner & 2;
    }


    bool dual_wield() const noexcept
    {
        return _inner & 4;
    }



    void reset() noexcept
    {
        _inner = 0;
    }


    void set_unarmed() noexcept
    {
        reset();
    }


    void set_shield() noexcept
    {
        _inner |= 1;
    }


    void set_two_hand() noexcept
    {
        _inner |= 2;
    }


    void set_dual_wield() noexcept
    {
        _inner |= 4;
    }



private:
    int _inner{};
};



struct Character
{
    enum class State : int
    {
        empty = 0,
        alive = 1,
        villager_dead = 2,
        adventurer_in_other_map = 3,
        adventurer_dead = 4,
        adventurer_empty = 5,
        pet_dead = 6,
        pet_waiting = 7,
        pet_in_other_map =
            8, // Ally failed to be placed/not participating in arena
        pet_moving_to_map = 9, // Set on pets before leaving map, restored to
                               // "alive" after initialize
        servant_being_selected = 10,
    };

    Character();

    // NOTE: Don't add new fields unless you add them to serialization, which
    // will break save compatibility.

    // Index of this character into the global cdata array.
    // Used for communicating with legacy code that takes integer index
    // arguments. New code should pass Character& instead. Not serialized; set
    // on creation and load.
    int index = -1;

    ObjId obj_id;

private:
    Character::State state_ = Character::State::empty;

public:
    Position position;
    Position next_position;
    time::Instant revival_time{};
    int vision_flag = 0;
    int image = 0;
    int sex = 0;
    int relationship = 0;
    int turn_cost = 0;
    int current_speed = 0;
    OptionalItemRef ai_item;
    std::string portrait;
    int interest = 0;
    time::Instant interest_renewal_time{};
    int personality = 0;
    int impression = 0;
    int talk_type = 0;
    int height = 0;
    int weight = 0;

    /// Birthday. Because Date class does not have default constructor (both
    /// month of 0th and day of 0th are invalid), a dummy value is assgiend
    /// until character initialization completes. The dummy value is the
    /// birthday of the green-haired Elea.
    /// http://elonagather.wiki.fc2.com/wiki/%E4%B8%BB%E8%A6%81%E3%81%AA%E4%BA%BA%E7%89%A9
    time::Date birthday{493, 12, 26};

    int nutrition = 0;
    int can_talk = 0;
    Quality quality = Quality::none;
    int turn = 0;
    CharaId id = CharaId::none;
    int vision_distance = 0;
    int enemy_id = 0;
    int gold = 0;
    int platinum_coin = 0;
    CombatStyle combat_style;
    int melee_attack_type = 0;
    int fame = 0;
    int experience = 0;
    int required_experience = 0;
    int speed_percentage = 0;
    int level = 0;
    int speed_percentage_in_next_turn = 0;
    int skill_bonus = 0;
    int total_skill_bonus = 0;
    int inventory_weight = 0;
    int max_inventory_weight = 0;
    int inventory_weight_type = 0;
    int max_level = 0;
    int karma = 0;
    int hp = 0;
    int max_hp = 0;
    int sp = 0;
    int max_sp = 0;
    int mp = 0;
    int max_mp = 0;
    int heal_value_per_nether_attack = 0;
    GodId god_id;
    int piety_point = 0;
    int praying_point = 0;
    int sum_of_equipment_weight = 0;
    int special_attack_type = 0;
    int rate_to_pierce = 0;
    int rate_of_critical_hit = 0;
    int speed_correction_value = 0;
    int original_relationship = 0;
    int pv = 0;
    int dv = 0;
    int hit_bonus = 0;
    int damage_bonus = 0;
    int pv_correction_value = 0;
    int dv_correction_value = 0;
    int damage_reaction_info = 0;
    int emotion_icon = 0;
    int current_map = 0;
    int current_dungeon_level = 0;
    int related_quest_id = 0;
    int direction = 0;
    time::Instant hire_limit_time{};
    int hire_count = 0;
    int insanity = 0;
    int curse_power = 0;
    int extra_attack = 0;
    int extra_shot = 0;
    int decrease_physical_damage = 0;
    int nullify_damage = 0;
    int cut_counterattack = 0;
    int anorexia_count = 0;
    Activity activity;
    int quality_of_performance = 0;
    int tip_gold = 0;
    Role role = Role::none;
    int shop_rank = 0;
    int activity_target = 0;
    int shop_store_id = 0;
    time::Instant shop_restock_time{};
    int cnpc_id = 0;
    Position initial_position;
    int hate = 0;
    int ai_calm = 0;
    int ai_move = 0;
    int ai_dist = 0;
    int ai_act_sub_freq = 0;
    int ai_heal = 0;
    int element_of_unarmed_attack = 0;
    int poisoned = 0;
    int sleep = 0;
    int paralyzed = 0;
    int blind = 0;
    int confused = 0;
    int fear = 0;
    int dimmed = 0;
    int drunk = 0;
    int bleeding = 0;
    int wet = 0;
    int insane = 0;
    int sick = 0;
    int gravity = 0;
    int choked = 0;
    int furious = 0;
    std::vector<int> growth_buffs;
    BodyPartList body_parts;
    std::vector<int> normal_actions;
    std::vector<int> special_actions;
    BuffList buffs;
    std::vector<int> attr_adjs;
    std::bitset<sizeof(int) * 8 * 50> _flags;
    int _156 = 0;
    int _203 = 0;
    Position target_position;


    /// Death count
    lua_int death_count;

    /// Kill count
    lua_int kill_count;


    std::string name;
    std::string alias;
    data::InstanceId race;
    data::InstanceId class_;
    std::string talk;

    InventoryRef inventory();



private:
    SkillData _skills;

    /// Spell stocks
    SpellStockTable _spell_stocks;

    /// Spacts
    SpactTable _spacts;

    /// All traits (feats, ether diseases and mutations) the character has.
    TraitLevelTable _traits;


public:
    Ability& get_skill(int id)
    {
        return _skills.get(id);
    }


    const Ability& get_skill(int id) const
    {
        return _skills.get(id);
    }


    SpellStockTable& spell_stocks() noexcept
    {
        return _spell_stocks;
    }


    const SpellStockTable& spell_stocks() const noexcept
    {
        return _spell_stocks;
    }


    SpactTable& spacts() noexcept
    {
        return _spacts;
    }


    const SpactTable& spacts() const noexcept
    {
        return _spacts;
    }



    TraitLevelTable& traits() noexcept
    {
        return _traits;
    }


    const TraitLevelTable& traits() const noexcept
    {
        return _traits;
    }



    void clear();
    void clear_flags();


    // for identifying the type of a Lua reference
    static std::string lua_type()
    {
        return "LuaCharacter";
    }

    bool is_dead()
    {
        return state_ == Character::State::empty ||
            state_ == Character::State::pet_dead ||
            state_ == Character::State::villager_dead ||
            state_ == Character::State::adventurer_dead;
    }

    Character::State state() const
    {
        return state_;
    }

    /**
     * Sets the state of this character. If the state transitions from "empty"
     * to any other state, the corresponding Lua handle will be created or
     * regenerated.
     */
    void set_state(Character::State);

    /**
     * Sets the state of this character without causing any handles to be
     * modified. Only to be used in internal character management functions
     * (relocation) where handles are manually copied or relocated.
     */
    void set_state_raw(Character::State new_state)
    {
        state_ = new_state;
    }


    data::InstanceId new_id() const
    {
        return *the_character_db.get_id_from_integer(charaid2int(this->id));
    }



    bool is_player() const;
    bool is_ally() const;
    bool is_player_or_ally() const;
    bool is_adventurer() const;
    bool is_global() const;
    bool is_map_local() const;



    ELONA_CHARACTER_DEFINE_FLAG_ACCESSORS


    static void copy(const Character& from, Character& to)
    {
        const auto index_save = to.index;
        to = from;
        to.index = index_save;
    }



    bool operator==(const Character& other) const noexcept
    {
        return this == &other;
    }


    bool operator!=(const Character& other) const noexcept
    {
        return this != &other;
    }



private:
    Character(const Character&) = default;
    Character(Character&&) = default;
    Character& operator=(const Character&) = default;
    Character& operator=(Character&&) = default;
};



struct CDataSlice
{
    using iterator = std::vector<Character>::iterator;

    CDataSlice(const iterator& begin, const iterator& end)
        : _begin(begin)
        , _end(end)
    {
    }

    iterator begin()
    {
        return _begin;
    }

    iterator end()
    {
        return _end;
    }

private:
    const iterator _begin;
    const iterator _end;
};



struct CData
{
    CData();


    Character& operator[](int index)
    {
        return storage[index];
    }


    Character& player()
    {
        return (*this)[0];
    }


    Character& tmp()
    {
        return (*this)[56];
    }



    CDataSlice all()
    {
        return {std::begin(storage), std::end(storage)};
    }


    CDataSlice allies()
    {
        return {std::begin(storage) + 1, std::begin(storage) + 16};
    }


    CDataSlice player_and_allies()
    {
        return {std::begin(storage), std::begin(storage) + 16};
    }


    CDataSlice adventurers()
    {
        return {std::begin(storage) + 16, std::begin(storage) + 55};
    }


    CDataSlice others()
    {
        return {std::begin(storage) + 57, std::end(storage)};
    }



private:
    std::vector<Character> storage;
};


extern CData cdata;

optional_ref<Character> chara_create(int slot, int chara_id, int x, int y);
void initialize_character(Character& chara);
bool chara_place(Character& chara);


enum class CharaRelocationMode
{
    normal,
    change,
};


/**
 * Relocate `source` to `destination_slot`. `source` character will be
 * destroyed.
 * @param source The relocated character.
 * @param destination_slot The slot of the character relocated from `source`. If
 * you specify `none`, find an empty slot in cdata.others().
 */
void chara_relocate(
    Character& source,
    optional_ref<Character> destination_slot,
    CharaRelocationMode mode = CharaRelocationMode::normal);

void chara_refresh(Character& chara);


/**
 * Copy `source` character to a new slot.
 * @param source The character copied from.
 * @return the character slot copied to if `source` was successfully copied;
 * otherwise, -1.
 */
int chara_copy(const Character& source);

void chara_delete(Character& chara);
void chara_vanquish(Character& chara);
void chara_killed(Character&);

enum class CharaFindLocation
{
    allies,
    others
};

optional_ref<Character> chara_find(data::InstanceId chara_id);
int chara_find_ally(int id);
int chara_get_free_slot();
int chara_get_free_slot_ally();
bool chara_unequip(const ItemRef& item);
bool chara_custom_talk(Character& speaker, int talk_type);
int chara_impression_level(int = 0);
void chara_modify_impression(Character& chara, int delta);
void chara_set_ai_item(Character& chara, ItemRef item);
int chara_armor_class(const Character& chara);
int chara_breed_power(const Character&);

void chara_add_quality_parens(Character& chara);

bool belong_to_same_team(const Character& c1, const Character& c2);

TurnResult proc_movement_event(Character& chara);
void chara_clear_status_effects(Character& chara);
void chara_clear_status_effects_b(Character& chara);
void revive_character(Character& chara);
void do_chara_revival(Character& chara);
void chara_set_revived_status(Character& chara);
void revive_player(Character& chara);
optional_ref<Character> new_ally_joins(Character& new_ally);
void refresh_burden_state();
void go_hostile();
void get_pregnant(Character& chara);
void chara_get_wet(Character& chara, int turns);
void chara_act_hostile_action(Character& attacker, Character& target);
void turn_aggro(Character& chara, Character& target, int hate);
void ride_begin(int = 0);
void ride_end();
void make_sound(
    Character& source_chara,
    int distance_threshold,
    int waken,
    bool may_make_angry);
void incognitobegin();
void incognitoend();
void initialize_pc_character();
void lovemiracle(Character& chara);
void monster_respawn();
void move_character(Character& chara);
void proc_negative_enchantments(Character& chara);
void proc_pregnant(Character& chara);
void wake_up();
bool try_to_perceive_npc(const Character& chara, const Character& enemy);
int relation_between(const Character& a, const Character& b);

} // namespace elona



#undef ELONA_CHARACTER_DEFINE_FLAG_ACCESSORS
#undef ELONA_CHARACTER_DEFINE_FLAG_ACCESSOR
