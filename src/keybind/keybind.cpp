#include "keybind.hpp"
#include "../audio.hpp"
#include "../config/config.hpp"
#include "../enums.hpp"
#include "../gdata.hpp"
#include "../thirdparty/ordered_map/ordered_map.h"
#include "../variables.hpp"

using namespace std::literals::string_literals;

namespace elona
{

KeybindManager keybind_manager;

namespace
{

tsl::ordered_map<std::string, Action> actions;

#include "key_names.cpp"

} // namespace

void add_action(Action action)
{
    actions.emplace(action.id, action);
}

void init_actions()
{
    using namespace snail;

    // TODO
    // categories.
    // popup:              [default, selection]
    // menu:               [default, selection, menu]
    // menu_with_shortcut: [default, selection, menu, shortcut]
    // game:               [default, shortcut, game, (wizard)]
    //
    // [game, wizard] and [selection, menu] are mutually exclusive. Every other
    // group can conflict with one another.
    //
    // The only exception seems to be do_look_command(), where key_target is
    // used in conjunction with actions from the menu category. So if "target"
    // conflicts with an action in "menu", it wouldn't be detected as a
    // conflict.
    //
    // Some of the keybindings have different functions depending on whether or
    // not wizard mode is enabled, so the input context would have to be
    // recreated if the wizard mode status changes.

    // clang-format off
    add_action(Action{"escape",     "default", {{Key::escape, ModKey::none}}});
    add_action(Action{"cancel",     "default", {{Key::shift,  ModKey::none}}});
    add_action(Action{"enter",      "default", {{Key::enter,  ModKey::none}, {Key::keypad_enter, ModKey::none}}});
    add_action(Action{"north",      "default", {{Key::up,        ModKey::none}, {Key::keypad_8, ModKey::none}}});
    add_action(Action{"south",      "default", {{Key::down,      ModKey::none}, {Key::keypad_2, ModKey::none}}});
    add_action(Action{"east",       "default", {{Key::left,      ModKey::none}, {Key::keypad_4, ModKey::none}}});
    add_action(Action{"west",       "default", {{Key::right,     ModKey::none}, {Key::keypad_6, ModKey::none}}});
    add_action(Action{"northwest",  "default", {{Key::pageup,    ModKey::none}, {Key::keypad_7, ModKey::none}}});
    add_action(Action{"northeast",  "default", {{Key::home,      ModKey::none}, {Key::keypad_9, ModKey::none}}});
    add_action(Action{"southwest",  "default", {{Key::pagedown,  ModKey::none}, {Key::keypad_1, ModKey::none}}});
    add_action(Action{"southeast",  "default", {{Key::end,       ModKey::none}, {Key::keypad_3, ModKey::none}}});

    add_action(Action{"shortcut_1",  "shortcut", {{Key::key_1, ModKey::none}}});
    add_action(Action{"shortcut_2",  "shortcut", {{Key::key_2, ModKey::none}}});
    add_action(Action{"shortcut_3",  "shortcut", {{Key::key_3, ModKey::none}}});
    add_action(Action{"shortcut_4",  "shortcut", {{Key::key_4, ModKey::none}}});
    add_action(Action{"shortcut_5",  "shortcut", {{Key::key_5, ModKey::none}}});
    add_action(Action{"shortcut_6",  "shortcut", {{Key::key_6, ModKey::none}}});
    add_action(Action{"shortcut_7",  "shortcut", {{Key::key_7, ModKey::none}}});
    add_action(Action{"shortcut_8",  "shortcut", {{Key::key_8, ModKey::none}}});
    add_action(Action{"shortcut_9",  "shortcut", {{Key::key_9, ModKey::none}}});
    add_action(Action{"shortcut_10", "shortcut", {{Key::key_0, ModKey::none}}});
    add_action(Action{"shortcut_11", "shortcut", {{Key::key_1, ModKey::ctrl}, {Key::key_1, ModKey::shift}}});
    add_action(Action{"shortcut_12", "shortcut", {{Key::key_2, ModKey::ctrl}, {Key::key_2, ModKey::shift}}});
    add_action(Action{"shortcut_13", "shortcut", {{Key::key_3, ModKey::ctrl}, {Key::key_3, ModKey::shift}}});
    add_action(Action{"shortcut_14", "shortcut", {{Key::key_4, ModKey::ctrl}, {Key::key_4, ModKey::shift}}});
    add_action(Action{"shortcut_15", "shortcut", {{Key::key_5, ModKey::ctrl}, {Key::key_5, ModKey::shift}}});
    add_action(Action{"shortcut_16", "shortcut", {{Key::key_6, ModKey::ctrl}, {Key::key_6, ModKey::shift}}});
    add_action(Action{"shortcut_17", "shortcut", {{Key::key_7, ModKey::ctrl}, {Key::key_7, ModKey::shift}}});
    add_action(Action{"shortcut_18", "shortcut", {{Key::key_8, ModKey::ctrl}, {Key::key_8, ModKey::shift}}});
    add_action(Action{"shortcut_19", "shortcut", {{Key::key_9, ModKey::ctrl}, {Key::key_9, ModKey::shift}}});
    add_action(Action{"shortcut_20", "shortcut", {{Key::key_0, ModKey::ctrl}, {Key::key_0, ModKey::shift}}});

    add_action(Action{"select_1",  "selection", {{Key::key_a, ModKey::none}}});
    add_action(Action{"select_2",  "selection", {{Key::key_b, ModKey::none}}});
    add_action(Action{"select_3",  "selection", {{Key::key_c, ModKey::none}}});
    add_action(Action{"select_4",  "selection", {{Key::key_d, ModKey::none}}});
    add_action(Action{"select_5",  "selection", {{Key::key_e, ModKey::none}}});
    add_action(Action{"select_6",  "selection", {{Key::key_f, ModKey::none}}});
    add_action(Action{"select_7",  "selection", {{Key::key_g, ModKey::none}}});
    add_action(Action{"select_8",  "selection", {{Key::key_h, ModKey::none}}});
    add_action(Action{"select_9",  "selection", {{Key::key_i, ModKey::none}}});
    add_action(Action{"select_10", "selection", {{Key::key_j, ModKey::none}}});
    add_action(Action{"select_11", "selection", {{Key::key_k, ModKey::none}}});
    add_action(Action{"select_12", "selection", {{Key::key_l, ModKey::none}}});
    add_action(Action{"select_13", "selection", {{Key::key_m, ModKey::none}}});
    add_action(Action{"select_14", "selection", {{Key::key_n, ModKey::none}}});
    add_action(Action{"select_15", "selection", {{Key::key_o, ModKey::none}}});
    add_action(Action{"select_16", "selection", {{Key::key_p, ModKey::none}}});
    add_action(Action{"select_17", "selection", {{Key::key_q, ModKey::none}}});
    add_action(Action{"select_18", "selection", {{Key::key_r, ModKey::none}}});
    add_action(Action{"select_19", "selection", {{Key::key_s, ModKey::none}}});

    add_action(Action{"next_page",      "menu", {{Key::keypad_plus,  ModKey::none}}});
    add_action(Action{"previous_page",  "menu", {{Key::keypad_minus, ModKey::none}}});
    add_action(Action{"next_menu",      "menu", {{Key::tab,    ModKey::none}}});
    add_action(Action{"previous_menu",  "menu", {{Key::tab,    ModKey::ctrl}}});
    add_action(Action{"switch_mode",    "menu", {{Key::key_z,      ModKey::none}}});
    add_action(Action{"switch_mode_2",  "menu", {{Key::key_x,      ModKey::none}}});

    add_action(Action{"wait",             "game", {{Key::period,           ModKey::none}, {Key::keypad_5, ModKey::none}}});
    add_action(Action{"quick_menu",       "game", {{Key::key_z,                ModKey::none}}});
    add_action(Action{"zap",              "game", {{Key::key_z,                ModKey::shift}}});
    add_action(Action{"inventory",        "game", {{Key::key_x,                ModKey::none}}});
    add_action(Action{"quick_inventory",  "game", {{Key::key_x,                ModKey::shift}}});
    add_action(Action{"get",              "game", {{Key::key_g,                ModKey::none}, {Key::keypad_0, ModKey::none}}});
    add_action(Action{"drop",             "game", {{Key::key_d,                ModKey::none}}});
    add_action(Action{"chara_info",       "game", {{Key::key_c,                ModKey::none}}});
    add_action(Action{"eat",              "game", {{Key::key_e,                ModKey::none}}});
    add_action(Action{"wear",             "game", {{Key::key_w,                ModKey::none}}});
    add_action(Action{"cast",             "game", {{Key::key_v,                ModKey::none}}});
    add_action(Action{"drink",            "game", {{Key::key_q,                ModKey::none}}});
    add_action(Action{"read",             "game", {{Key::key_r,                ModKey::none}}});
    add_action(Action{"fire",             "game", {{Key::key_f,                ModKey::none}}});
    add_action(Action{"go_down",          "game", {{Key::greater,          ModKey::none}}});
    add_action(Action{"go_up",            "game", {{Key::less,             ModKey::none}}});
    add_action(Action{"save",             "game", {{Key::key_s,                ModKey::shift}}});
    add_action(Action{"search",           "game", {{Key::key_s,                ModKey::none}}});
    add_action(Action{"interact",         "game", {{Key::key_i,                ModKey::none}}});
    add_action(Action{"skill",            "game", {{Key::key_a,                ModKey::none}}});
    add_action(Action{"close",            "game", {{Key::key_c,                ModKey::shift}}});
    add_action(Action{"rest",             "game", {{Key::key_r,                ModKey::shift}}});
    add_action(Action{"target",           "game", {{Key::keypad_asterisk,  ModKey::none}}});
    add_action(Action{"dig",              "game", {{Key::key_d,                ModKey::shift}}});
    add_action(Action{"pray",             "game", {{Key::key_p,                ModKey::none}}});
    add_action(Action{"offer",            "game", {{Key::key_o,                ModKey::shift}}});
    add_action(Action{"journal",          "game", {{Key::key_j,                ModKey::none}}});
    add_action(Action{"material",         "game", {{Key::key_m,                ModKey::none}}});
    add_action(Action{"trait",            "game", {{Key::key_f,                ModKey::shift}}});
    add_action(Action{"look",             "game", {{Key::key_l,                ModKey::none}}});
    add_action(Action{"give",             "game", {{Key::key_g,                ModKey::shift}}});
    add_action(Action{"throw",            "game", {{Key::key_t,                ModKey::shift}}});
    add_action(Action{"ammo",             "game", {{Key::key_a,                ModKey::shift}}});
    add_action(Action{"autodig",          "game", {{Key::key_h,                ModKey::shift}}});
    add_action(Action{"quicksave",        "game", {{Key::f1,               ModKey::none}}});
    add_action(Action{"quickload",        "game", {{Key::f2,               ModKey::none}}});
    add_action(Action{"help",             "game", {{Key::question,         ModKey::none}}});
    add_action(Action{"message_log",      "game", {{Key::slash,            ModKey::none}}});
    add_action(Action{"chat_box",         "game", {{Key::tab,              ModKey::none}}});
    add_action(Action{"tcg",               "game", {{Key::f3,               ModKey::none}}});
    add_action(Action{"update_screen",     "game", {{Key::f9,               ModKey::none}}});
    add_action(Action{"dump_player_info",  "game", {{Key::f11,              ModKey::none}}});
    add_action(Action{"enable_voldemort",  "game", {{Key::f12,              ModKey::none}}});
    add_action(Action{"reload_autopick",   "game", {{Key::backspace,        ModKey::shift}}});
    add_action(Action{"auto_action",       "game", {{Key::enter,            ModKey::none}}});

    // TODO: prioritize over other categories
    add_action(Action{"wizard_mewmewmew",    "wizard", {{Key::f3,               ModKey::none}}});
    add_action(Action{"wizard_wish",         "wizard", {{Key::f5,               ModKey::none}}});
    add_action(Action{"wizard_advance_time", "wizard", {{Key::f6,               ModKey::none}}});
    add_action(Action{"wizard_delete_map",   "wizard", {{Key::f7,               ModKey::none}}});
    // clang-format on

    for (const auto& pair : actions)
    {
        const auto& action = pair.second;

        keybind_manager.register_binding(action.id);
        auto& binding = keybind_manager.binding(action.id);
        for (const auto& keybind : action.default_keybinds)
        {
            if (keybind_is_bindable_key(keybind.main))
            {
                if (binding.primary.empty())
                {
                    binding.primary = keybind;
                }
                else if (binding.alternate.empty())
                {
                    binding.alternate = keybind;
                }
                else
                {
                    assert(false);
                }
            }
            else
            {
                std::cerr << "Permanent: " << keybind.to_string() << std::endl;
                binding.permanent = keybind;
            }
        }
    }
}


KeybindManager::GroupedMapType KeybindManager::create_category_to_action_list()
{
    GroupedMapType result;

    for (const auto& pair : actions)
    {
        const auto& action = pair.second;
        result.emplace(action.category, action.id);
    }

    return result;
}


std::string Keybind::to_string() const
{
    if (empty())
    {
        return "";
    }

    const auto it = key_names.find(main);
    if (it == key_names.end())
    {
        return "<unbindable key "s + static_cast<int>(main) + ">";
    }

    std::string mod_key_string = "";
    if ((modifiers & snail::ModKey::ctrl) == snail::ModKey::ctrl)
    {
        mod_key_string += "Ctrl+";
    }
    if ((modifiers & snail::ModKey::shift) == snail::ModKey::shift)
    {
        mod_key_string += "Shift+";
    }
    if ((modifiers & snail::ModKey::alt) == snail::ModKey::alt)
    {
        mod_key_string += "Alt+";
    }
    if ((modifiers & snail::ModKey::gui) == snail::ModKey::gui)
    {
        mod_key_string += "Gui+";
    }

    return mod_key_string + it->second;
}

bool InputContext::matches(
    const std::string& action_id,
    snail::Key key,
    snail::ModKey modifiers)
{
    if (available_actions.find(action_id) == available_actions.end())
    {
        return false;
    }

    // TODO
    const auto& action = actions.at(action_id);

    for (const auto& keybind : action.default_keybinds)
    {
        if (keybind.main == key /* && keybind.modifiers == modifiers */)
        {
            return true;
        }
    }

    return false;
}

optional<std::string> InputContext::action_for_key(const Keybind& keybind)
{
    // The set is sorted by insertion order, so entries from categories inserted
    // earlier will receive priority.
    for (const auto& action_id : available_actions)
    {
        const auto& binding = keybind_manager.binding(action_id);
        if (binding.matches(keybind))
        {
            return action_id;
        }
    }

    return none;
}

optional<std::string> InputContext::check_movement_action(
    const std::set<snail::Key>& keys,
    snail::ModKey modifiers)
{
    StickKey input = StickKey::none;
    bool wait = false;

    for (const auto& key : keys)
    {
        // TODO: is keywait still needed after input_context.reset()?
        if (matches("escape", key, modifiers) && keywait == 0)
        {
            return "cancel"s;
        }

        if (matches("north", key, modifiers))
        {
            input |= StickKey::up;
        }
        else if (matches("south", key, modifiers))
        {
            input |= StickKey::down;
        }
        else if (matches("east", key, modifiers))
        {
            input |= StickKey::left;
        }
        else if (matches("west", key, modifiers))
        {
            input |= StickKey::right;
        }
        else if (matches("northwest", key, modifiers))
        {
            input = StickKey::up | StickKey::left;
        }
        else if (matches("northeast", key, modifiers))
        {
            input = StickKey::up | StickKey::right;
        }
        else if (matches("southwest", key, modifiers))
        {
            input = StickKey::down | StickKey::left;
        }
        else if (matches("southeast", key, modifiers))
        {
            input = StickKey::down | StickKey::right;
        }
        else if (matches("wait", key, modifiers))
        {
            input = StickKey::none;
            wait = true;
        }
        else if (!is_modifier(key))
        {
            // Encountered non-movement key, prioritize it over movement.
            return none;
        }
    }

    if ((modifiers & snail::ModKey::shift) == snail::ModKey::shift)
    {
        std::cerr << "run" << std::endl;
        // Has to be modified globally, since scroll speed is determined by
        // keybd_wait. See @ref ui_scroll_screen()
        keybd_wait = 100000;
        if (keywait == 0)
        {
            keywait = 1;
            return "cancel"s;
        }
    }
    else
    {
        keywait = 0;
    }


    if ((modifiers & snail::ModKey::alt) != snail::ModKey::alt)
    {
        if (input == StickKey::left)
        {
            return "west"s;
        }
        if (input == StickKey::up)
        {
            return "north"s;
        }
        if (input == StickKey::right)
        {
            return "east"s;
        }
        if (input == StickKey::down)
        {
            return "south"s;
        }
    }
    if (input == (StickKey::up | StickKey::left))
    {
        return "northwest"s;
    }
    if (input == (StickKey::up | StickKey::right))
    {
        return "northeast"s;
    }
    if (input == (StickKey::down | StickKey::left))
    {
        return "southwest"s;
    }
    if (input == (StickKey::down | StickKey::right))
    {
        return "southeast"s;
    }
    if (wait)
    {
        return "wait"s;
    }

    return none;
}

bool InputContext::is_nonmovement_key(const snail::Key& k)
{
    return (keybind_is_bindable_key(k) || k == snail::Key::enter)
        && excluded_keys.find(k) == excluded_keys.end();
}

optional<Keybind> InputContext::check_key()
{
    const auto& keys = snail::Input::instance().pressed_keys();
    auto modifiers = snail::Input::instance().modifiers();

    // Pick the first nonmovement key out of the ones that were held.
    // The only actions for which holding multiple keys makes sense is for the
    // movement keys.
    const auto it =
        std::find_if(keys.begin(), keys.end(), [this](const snail::Key& k) {
            return is_nonmovement_key(k);
        });

    if (it != keys.end())
    {
        return Keybind(*it, modifiers);
    }

    return none;
}

InputContext make_input_context(const std::string& category)
{
    InputContext result;
    result.add_actions_from_category("default");
    if (game_data.wizard)
    {
        result.add_actions_from_category("wizard");
    }
    result.add_actions_from_category(category);

    // TODO exclude movement keys

    return result;
}

void InputContext::add_actions_from_category(const std::string& category)
{
    for (const auto& pair : actions)
    {
        if (pair.second.category == category)
        {
            add_action(pair.second.id);
        }
    }
}

void InputContext::add_action(const std::string& action)
{
    available_actions.insert(action);
}

std::string InputContext::delay_movement_action(
    const std::string& action,
    snail::ModKey modifiers,
    KeyWaitDelay delay_type)
{
    if (keybd_wait >= 100000)
    {
        if ((modifiers & snail::ModKey::shift) != snail::ModKey::shift)
        {
            keybd_wait = 1000;
        }
    }

    if (delay_type == KeyWaitDelay::none)
    {
        return action;
    }

    if (delay_type == KeyWaitDelay::walk_run)
    {
        if (keybd_attacking != 0)
        {
            if (keybd_wait % Config::instance().attackwait != 0)
            {
                return ""s;
            }
        }
        else if (Config::instance().scroll == 0)
        {
            if (keybd_wait
                < Config::instance().walkwait * Config::instance().startrun)
            {
                std::cerr << "WALK" << std::endl;
                if (keybd_wait % Config::instance().walkwait != 0)
                {
                    return ""s;
                }
            }
            else
            {
                std::cerr << "RUN1 " << Config::instance().startrun << " "
                          << Config::instance().runwait << std::endl;
                running = 1;
                if (keybd_wait < 100000)
                {
                    if (keybd_wait % Config::instance().runwait != 0)
                    {
                        return ""s;
                    }
                }
            }
        }
        // else if (input == StickKey::none)
        else if (action == "wait"s)
        {
            if (keybd_wait < 20)
            {
                if (keybd_wait != 0)
                {
                    return ""s;
                }
            }
        }
        else if (keybd_wait > Config::instance().startrun)
        {
            std::cerr << "RUN2 " << Config::instance().startrun << " "
                      << Config::instance().runwait << std::endl;
            if (Config::instance().runscroll == 0)
            {
                if (keybd_wait % Config::instance().runwait != 0)
                {
                    return ""s;
                }
            }
            running = 1;
        }
    }
    else if (
        keybd_wait
        < Config::instance().select_fast_start * Config::instance().select_wait)
    {
        if (keybd_wait % Config::instance().select_wait != 0)
        {
            return ""s;
        }
    }
    else if (keybd_wait < 1000)
    {
        if (keybd_wait % Config::instance().select_fast_wait != 0)
        {
            return ""s;
        }
    }

    return action;
}

static bool
_is_keypress_delayed(int held_frames, int keywait, int initial_keywait)
{
    if (held_frames < initial_keywait)
    {
        if (held_frames == 0)
        {
            return false;
        }
    }
    else if (held_frames % keywait == 0)
    {
        return false;
    }

    return true;
}

std::string InputContext::delay_normal_action(const std::string& action)
{
    if (action == "" || last_action_ != action)
    {
        last_action_held_frames_ = 0;
    }

    bool delayed = _is_keypress_delayed(last_action_held_frames_, 1, 20);

    last_action_held_frames_++;

    if (last_action_ != action)
    {
        std::cerr << "set action " << action << std::endl;
        last_action_ = action;
    }

    if (delayed)
    {
        return ""s;
    }

    return action;
} // namespace elona

std::string InputContext::check_for_command(KeyWaitDelay delay_type)
{
    const auto& keys = snail::Input::instance().pressed_keys();
    auto modifiers = snail::Input::instance().modifiers();

    if (const auto action = check_movement_action(keys, modifiers))
    {
        last_action_ = ""s;

        // Movement keys have special key delay behavior, so handle them.
        auto result = delay_movement_action(*action, modifiers, delay_type);
        ++keybd_wait;

        return result;
    }
    else
    {
        keybd_wait = 0;
        keybd_attacking = 0;
        running = 0;
    }

    if (const auto keybind = check_key())
    {
        if (const auto action = action_for_key(*keybind))
        {
            return delay_normal_action(*action);
        }
        else
        {
            last_action_ = "";
        }
    }
    else
    {
        last_action_ = "";
    }

    return ""s;
}

std::string InputContext::check_for_command_with_list(int& list_index)
{
    auto command = check_for_command(KeyWaitDelay::always);
    if (command == "north"s)
    {
        snd(5);
        --cs;
        if (cs < 0)
        {
            cs = keyrange - 1;
            if (cs < 0)
            {
                cs = 0;
            }
        }
    }
    if (command == "south"s)
    {
        snd(5);
        ++cs;
        if (cs >= keyrange)
        {
            cs = 0;
        }
    }
    if (command == "west"s)
    {
        command = "previous_page"s;
    }
    if (command == "east"s)
    {
        command = "next_page"s;
    }
    if (cs >= keyrange)
    {
        cs_bk = -1;
        cs = keyrange - 1;
        if (cs < 0)
        {
            cs = 0;
        }
    }
    if (command == "enter"s)
    {
        list_index = cs;
    }
    else if (keybind_action_has_category(command, "selection"s))
    {
        list_index = keybind_id_number(command);
    }
    else
    {
        list_index = -1;
    }
    return command;
}

void InputContext::reset()
{
    snail::Input::instance().clear_pressed_keys();
    last_action_held_frames_ = 0;
    last_action_ = ""s;
}

bool keybind_is_joystick_key(snail::Key key)
{
    return false;
}

bool keybind_is_bindable_key(snail::Key key)
{
    return key_names.find(key) != key_names.end();
}

bool keybind_action_has_category(
    const std::string& action_id,
    const std::string& category)
{
    if (action_id == ""s)
    {
        return false;
    }

    return actions.at(action_id).category == category;
}

int keybind_id_number(const std::string& action_id)
{
    auto underscore_pos = action_id.find("_");
    assert(underscore_pos != std::string::npos);

    auto index_string = action_id.substr(underscore_pos + 1);

    // Assumption is the ID will be at least 1.
    // elona::stoi returns 0 on failure.
    int result = elona::stoi(index_string);
    assert(result >= 1);

    // Adjust the result to be 0-indexed.
    return result - 1;
}

} // namespace elona
