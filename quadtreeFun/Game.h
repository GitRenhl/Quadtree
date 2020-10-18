#pragma once
#include "Engine.h"
#include "Quadtree.h"
#include "ChainList.h"
#include <vector>


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
    Chain<Vector2i> pointsList;

    std::vector<const Vector2i*> points_in_range;
    const Vector2i range;

    bool draw_quads;

    void EventProcess(float dt);
    void Update(float dt);
    void Render();

};
