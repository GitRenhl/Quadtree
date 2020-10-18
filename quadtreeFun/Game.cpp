#include "Game.h"
#include "olcPixelGameEngine.h"
#include "Vector2d.h"
#include <assert.h>


enum Mouse: uint32_t {
    Left = 0,
    Right,
    Middle
};

Game::Game():
     Engine(),
     quadtree(nullptr, 0, 0, 512),
     range(30, 20),
     draw_quads(false){
    sAppName = "QuadTree";
}

Game::~Game() {
}

bool Game::OnUserCreate() {
    return true;
}

bool Game::OnUserDestroy() { return true; }

bool Game::OnUserUpdate(float fElapsedTime) {
    EventProcess(fElapsedTime);
    Update(fElapsedTime);
    Render();
    return true;
}

void Game::DrawQuad(const Quadtree *quad) {
    if (draw_quads) {
        DrawRect((int32_t)quad->GetPosition().x, (int32_t)quad->GetPosition().y,
                 (int32_t)quad->GetSize(), (int32_t)quad->GetSize(),
                 { 50, 50, 50 });
    }

    if (quad->IsSplited()) {
        DrawQuad(quad->GetParts(Quadtree::Parts::NW));
        DrawQuad(quad->GetParts(Quadtree::Parts::NE));
        DrawQuad(quad->GetParts(Quadtree::Parts::SW));
        DrawQuad(quad->GetParts(Quadtree::Parts::SE));
    } else {
        const Vector2i *point = quad->GetPoint(quad->GetPosition());
        if (point != nullptr) Draw(point->x, point->y);
    }
}

void Game::RebuildQueue() {
    quadtree.Clear();
    for (auto *pl = pointsList.GetFirst(); pl != nullptr; pl = pl->next) {
        quadtree.Insert(&pl->GetData());
    }
}

void Game::EventProcess(float dt) {
    if (GetKey(olc::ESCAPE).bPressed)
        olc_Terminate();

    if (GetKey(olc::F1).bReleased) {
        draw_quads = !draw_quads;
    }

    if (GetMouse(Mouse::Left).bPressed) {
        auto *link = pointsList.New({ GetMouseX(), GetMouseY() });
        if (!quadtree.Insert(&link->GetData())) {
            puts("error while adding point");
            auto *link = pointsList.Pop();
            if (link != nullptr) {
                delete link;
                RebuildQueue();
            }
        }
    }
    if (GetKey(olc::R).bPressed) {
        pointsList.Clear();
        quadtree.Clear();
    }
}

void Game::Update(float dt) {
    points_in_range = quadtree.Query({ GetMouseX() - range.x / 2, GetMouseY() - range.y / 2 }, range);
}

void Game::Render() {
    Clear(olc::BLACK);

    DrawQuad(&quadtree);

    // Draw rectangle around a mouse
    DrawRect(GetMouseX() - range.x / 2, GetMouseY() - range.y / 2, range.x, range.y);
    for (auto *point : points_in_range) {
        Draw(point->x, point->y, olc::RED);
    }

    // Draw how many points exists
    DrawStringDecal({ 2.f, 2.f }, std::to_string(pointsList.Size()));
    // Draw how many points are in range
    DrawStringDecal({ 2.f, 12.f }, std::to_string(points_in_range.size()));
}
