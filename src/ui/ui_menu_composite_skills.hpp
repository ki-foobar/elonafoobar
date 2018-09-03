#pragma once
#include "ui_menu_composite.hpp"

namespace elona
{
namespace ui
{

struct UIMenuSkillsResult
{
    int effect_id;
};

struct UIMenuSpellsResult
{
    int effect_id;
};

typedef boost::variant<UIMenuSkillsResult, UIMenuSpellsResult>
    UIMenuCompositeSkillsResult;

class UIMenuCompositeSkills
    : public UIMenuComposite<UIMenuCompositeSkillsResult>
{
public:
    enum Index : size_t
    {
        spells = 0,
        skills = 1
    };

    UIMenuCompositeSkills(size_t selected)
        : UIMenuComposite(selected)
    {
    }

protected:
    virtual void add_menus();
};

} // namespace ui
} // namespace elona
