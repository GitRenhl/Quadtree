#pragma once
#include "Vector2d.h"
#include <vector>


class Quadtree {
    static uint32_t m_next_id;
public:
    enum class Parts: uint32_t{
        NW = 0,
        NE,
        SW,
        SE,
        AMOUNT
    };

    Quadtree(const Quadtree *const parent, float x, float y, float size);
    Quadtree(const Quadtree *const parent, const Vector2f &pos, float size);
    ~Quadtree();

    const Vector2f &GetPosition() const { return m_POSITION; }
    float GetSize() const { return m_SIZE; }
    const Quadtree *GetParts(Parts part) const;
    const Vector2i *const GetPoint(const Vector2i &pos) const;
    
    bool IsSplited() const { return m_is_splited; }
    bool IsBelongMe(const Vector2i *point) const;
    bool IsBelongMe(const Vector2i &point) const;


    bool Insert(const Vector2i *point);
    std::vector<const Vector2i*> Query(const Vector2i &position, const Vector2i &size);

    void Clear();

private:
    void Split();
    void UndoSplit();
    bool InsertToQuads(const Vector2i *point);

private:
    const uint32_t m_ID;
    const Quadtree *const m_parent;
    const Vector2f m_POSITION;
    const float m_SIZE;

    Quadtree *NW = nullptr;
    Quadtree *NE = nullptr;
    Quadtree *SW = nullptr;
    Quadtree *SE = nullptr;

    bool m_is_splited;
    const Vector2i *m_point;
};
