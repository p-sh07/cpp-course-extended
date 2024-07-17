#pragma once

#include <cstdlib>

#include "image.h"

namespace detail {

inline int Sign(int value) {
    return (0 < value) - (value < 0);
}

/**
 * Рисует крутой отрезок (для которого |to.y - from.x| >= |to.x - from.x|).
 */
inline void DrawSteepLine(Image& image, Point from, Point to, char color) {
    const int delta_x = std::abs(to.x - from.x);
    const int delta_y = std::abs(to.y - from.y);

    assert(delta_y >= delta_x);

    if (from.y > to.y) {  // Крутые отрезки рисуем сверху вниз.
        std::swap(from, to);
    }

    const int step_x = Sign(to.x - from.x);  // Шаг по оси X (-1, 0 или 1).
    const int error_threshold = delta_y + 1;  // Порог ошибки вычисления координаты X.
    const int delta_err = delta_x + 1;  // Шаг накопления ошибки.

    // Когда начальное значение ошибки начинается не с 0, а с delta_err/2,
    // отрезок получается красивее.
    int error = delta_err / 2;

    for (Point p = from; p.y <= to.y; ++p.y) {
        image.SetPixel({p.x, p.y}, color);
        assert((p.y != to.y) || (p.x == to.x));

        error += delta_err;  // Накапливаем ошибку вычисления координаты X.

        if (error >= error_threshold) {  // Если вышли за пределы текущей координаты X
            p.x += step_x;  // Смещаемся к следующей координате X
            error -= error_threshold;  // Сбрасываем ошибку
        }
    }
}

/**
 * Рисует пологий отрезок (для которого |to.y - from.x| >= |to.y - from.y|).
 */
inline void DrawSlopeLine(Image& image, Point from, Point to, char color) {
    const int delta_x = std::abs(to.x - from.x);
    const int delta_y = std::abs(to.y - from.y);

    assert(delta_x >= delta_y);

    if (from.x > to.x) {  // Пологие отрезки рисуем слева направо.
        std::swap(from, to);
    }

    // Пологие отрезки рисуются по аналогии с крутыми.

    const int step_y = Sign(to.y - from.y);
    const int error_threshold = delta_x + 1;
    const int delta_err = delta_y + 1;

    int error = delta_err / 2;

    for (Point p = from; p.x <= to.x; ++p.x) {
        image.SetPixel({p.x, p.y}, color);
        assert((p.x != to.x) || (p.y == to.y));

        error += delta_err;

        if (error >= error_threshold) {
            p.y += step_y;
            error -= error_threshold;
        }
    }
}

}  // namespace detail

/**
 * Рисует отрезок прямой линии между точками from и to цветом color на изображении Image.
 *
 * Для рисования используется алгоритм Брезенхэма.
 * (https://ru.wikipedia.org/wiki/Алгоритм_Брезенхэма)
 */
inline void DrawLine(Image& image, Point from, Point to, char color) {
    const int delta_x = std::abs(to.x - from.x);
    const int delta_y = std::abs(to.y - from.y);

    if (delta_y > delta_x) {  // Отрезок крутой.
        detail::DrawSteepLine(image, from, to, color);
    } else {  // Отрезок пологий.
        detail::DrawSlopeLine(image, from, to, color);
    }
}