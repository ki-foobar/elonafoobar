#pragma once

#include <string>
#include "../lib/noncopyable.hpp"
#include "blend_mode.hpp"
#include "color.hpp"
#include "detail/sdl.hpp"
#include "font.hpp"
#include "image.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "window.hpp"


namespace elona
{
namespace snail
{



class Renderer : public lib::noncopyable
{
public:
    enum Flag
    {
        none = 0,
        software = SDL_RENDERER_SOFTWARE,
        accelerated = SDL_RENDERER_ACCELERATED,
        present_vsync = SDL_RENDERER_PRESENTVSYNC,
        target_texture = SDL_RENDERER_TARGETTEXTURE,
    };


    enum class TextAlignment
    {
        left,
        center,
        right,
    };


    enum class TextBaseline
    {
        top,
        middle,
        bottom,
    };

    ~Renderer();

    TextAlignment text_alignment() const noexcept
    {
        return _text_alignment;
    }


    void set_text_alignment(TextAlignment text_alignment)
    {
        _text_alignment = text_alignment;
    }


    TextBaseline text_baseline() const noexcept
    {
        return _text_baseline;
    }


    void set_text_baseline(TextBaseline text_baseline)
    {
        _text_baseline = text_baseline;
    }


    bool has_font()
    {
        return _font.ptr() != nullptr;
    }


    const Font& font() const noexcept
    {
        return _font;
    }


    void set_font(const Font& font)
    {
        _font = font;
    }


    BlendMode blend_mode() const noexcept
    {
        return _blend_mode;
    }

    void set_blend_mode(BlendMode blend_mode);

    void set_draw_color(const Color&);

    ::SDL_Texture* render_target();

    void set_render_target(::SDL_Texture* texture);


    ::SDL_Renderer* ptr();


    Renderer(Window& target_window, int flag);


    void clear();
    void present();
    void render_point(int x, int y);
    void fill_rect(int x, int y, int width, int height);
    Rect render_text(
        const std::string& text,
        int x,
        int y,
        const Color& text_color = palette::black,
        double scale = 1.0);
    Rect render_text_with_shadow(
        const std::string& text,
        int x,
        int y,
        const Color& text_color = palette::white,
        const Color& shadow_color = palette::black,
        double scale = 1.0);
    Rect render_multiline_text(
        const std::string& text,
        int x,
        int y,
        const Color& text_color = palette::black);
    Size calculate_text_size(const std::string& text);
    void render_line(int start_x, int start_y, int end_x, int end_y);
    void render_image(ImageBase& image, int dst_x, int dst_y);
    void render_image(
        ImageBase& image,
        int dst_x,
        int dst_y,
        int dst_width,
        int dst_height);
    void render_image(
        ImageBase& image,
        int src_x,
        int src_y,
        int src_width,
        int src_height,
        int dst_x,
        int dst_y);
    void render_image(
        ImageBase& image,
        int src_x,
        int src_y,
        int src_width,
        int src_height,
        int dst_x,
        int dst_y,
        int dst_width,
        int dst_height);
    void render_image(::SDL_Texture* image, int dst_x, int dst_y);
    void render_image(
        ::SDL_Texture* image,
        int dst_x,
        int dst_y,
        int dst_width,
        int dst_height);
    void render_image(
        ::SDL_Texture* image,
        int src_x,
        int src_y,
        int src_width,
        int src_height,
        int dst_x,
        int dst_y);
    void render_image(
        ::SDL_Texture* image,
        int src_x,
        int src_y,
        int src_width,
        int src_height,
        int dst_x,
        int dst_y,
        int dst_width,
        int dst_height);


    void render_image_crop(
        ImageBase& image,
        int src_x,
        int src_y,
        int src_width,
        int src_height,
        int dst_x,
        int dst_y);
    void render_image_crop(
        ::SDL_Texture* image,
        int src_x,
        int src_y,
        int src_width,
        int src_height,
        int dst_x,
        int dst_y);

private:
    TextAlignment _text_alignment = TextAlignment::left;
    TextBaseline _text_baseline = TextBaseline::top;
    Font _font;
    BlendMode _blend_mode = BlendMode::blend;
};



} // namespace snail
} // namespace elona
