#pragma once
#include <vector>
#include "Engine.h"
#include "Quadtree.h"


class PointList: public Vector2i{
public:
    PointList(const PointList *parent, int32_t x, int32_t y): m_parent(parent), Vector2i(x, y){}
    ~PointList() {
        delete next;
    }

    const PointList *GetFirst() const {
        if (m_parent == nullptr) return this;
        else return m_parent->GetFirst();
    }
    const PointList *GetLast() const {
        if (next == nullptr) return this;
        else return next->GetLast();
    }

    const PointList *New(int32_t x, int32_t y) {
        if (next == nullptr) {
            next = new PointList(this, x, y);
            return next;
        } else {
            return next->New(x, y);
        }
    }

    bool Pop() {
        if (next->next == nullptr) {
            delete next;
            next = nullptr;
            return true;
        } else {
            return next->Pop();
        }
        return false;
    }

    uint32_t Size() const {
        if (next == nullptr) {
            return 1;
        } else {
            return next->Size() + 1;
        }
    }

    PointList *next = nullptr;
private:
    const PointList *m_parent = nullptr;
};

class Game : public Engine {
public:
    Game();
    ~Game();

public:
    bool OnUserCreate() override;
    bool OnUserDestroy() override;
    bool OnUserUpdate(float fElapsedTime) override;

private:
    void DrawQuad(const Quadtree *quad);
    void RebuildQueue();

    Quadtree quadtree;
    PointList * pointsList;
    const Vector2i *selected = nullptr;

    std::vector<const Vector2i*> points_in_range;
    const Vector2i range;

    bool draw_quads;

    void EventProcess(float dt);
    void Update(float dt);
    void Render();

};
