#include "Quadtree.h"
//#include <stdio.h>
//#include <stdlib.h>
#include <assert.h>


uint32_t Quadtree::m_next_id = 0;

Quadtree::Quadtree(const Quadtree *const parent, float x, float y, float size):
     m_ID(m_next_id++),
     m_parent(parent),
     m_POSITION(x, y),
     m_SIZE(size),
     m_is_splited(false) {
    //printf("New %d\n", GetID());
    if (m_SIZE <= 0) {
        perror("Invalid quad size");
        exit(1);
    }
    m_point = nullptr;
}

Quadtree::Quadtree(const Quadtree *const parent, const Vector2f &pos, float size):
    Quadtree(parent, pos.x, pos.y, size){
}

Quadtree::~Quadtree() {
    //printf("Del %d\n", GetID());
    Clear();
}

const Quadtree *Quadtree::GetParts(Parts part) const {
    switch (part) {
    case Quadtree::Parts::NW:
        return NW;
    case Quadtree::Parts::NE:
        return NE;
    case Quadtree::Parts::SW:
        return SW;
    case Quadtree::Parts::SE:
        return SE;
    default:
        return nullptr;
    }
}

const Vector2i *const Quadtree::GetPoint(const Vector2i &pos) const {
    if (!IsBelongMe(pos)) return nullptr;
    if (m_is_splited) {
        for (uint32_t part = (uint32_t)Parts::NW; part != (uint32_t)Parts::AMOUNT; part++) {
            const Quadtree *other = GetParts((Parts)part);
            if (other->IsBelongMe(pos)) {
                return other->GetPoint(pos);
            }
        }
        return nullptr;
    } else {
        return m_point;
    }
}

bool Quadtree::IsBelongMe(const Vector2i *point) const {
    return point->x >= m_POSITION.x && point->x < m_POSITION.x + m_SIZE &&
           point->y >= m_POSITION.y && point->y < m_POSITION.y + m_SIZE;
}

bool Quadtree::IsBelongMe(const Vector2i &point) const {
    return point.x >= m_POSITION.x && point.x < m_POSITION.x + m_SIZE &&
           point.y >= m_POSITION.y && point.y < m_POSITION.y + m_SIZE;
}

bool Quadtree::Insert(const Vector2i *point) {
    //printf("HERE %d\n", GetID());
    if (!IsBelongMe(point)) return false;

    if (m_is_splited) {
        return InsertToQuads(point);
    } else if (this->m_point != nullptr) {
        if (*point == *m_point) {
            //this->m_point = nullptr;
            //puts("test");
            return false;
        }
        Split();
        if (!InsertToQuads(this->m_point)) {
            // Point was not added
            //Unsplit();
            return false;
        }

        this->m_point = nullptr;
        return InsertToQuads(point);
    } else {
        this->m_point = point;
    }

    return true;
}

std::vector<const Vector2i*> Quadtree::Query(const Vector2i &position, const Vector2i &size) {
    std::vector<const Vector2i*> points;
    if (position.x + size.x < m_POSITION.x || m_POSITION.x + m_SIZE < position.x ||
        position.y + size.y < m_POSITION.y || m_POSITION.y + m_SIZE < position.y) {
        return points;
    }
    if (m_is_splited) {
        std::vector<const Vector2i*> tmp;
        tmp = NW->Query(position, size);
        points.insert(points.end(), tmp.begin(), tmp.end());
        tmp = NE->Query(position, size);
        points.insert(points.end(), tmp.begin(), tmp.end());
        tmp = SW->Query(position, size);
        points.insert(points.end(), tmp.begin(), tmp.end());
        tmp = SE->Query(position, size);
        points.insert(points.end(), tmp.begin(), tmp.end());
    } else if (m_point != nullptr) {
        if (m_point->x >= position.x && m_point->y >= position.y &&
            m_point->x < position.x + size.x && m_point->y < position.y + size.y) {
            points.push_back(m_point);
        }
    }
    return points;
}

void Quadtree::Clear() {
    if (m_is_splited) {
        UndoSplit();
    } else {
        m_point = nullptr;
    }
}

void Quadtree::Split() {
    assert(!m_is_splited);
    m_is_splited = true;
    float h_size = m_SIZE / 2.f;
    NW = new Quadtree(this, m_POSITION.x         , m_POSITION.y,          h_size);
    NE = new Quadtree(this, m_POSITION.x + h_size, m_POSITION.y,          h_size);
    SW = new Quadtree(this, m_POSITION.x         , m_POSITION.y + h_size, h_size);
    SE = new Quadtree(this, m_POSITION.x + h_size, m_POSITION.y + h_size, h_size);
}

void Quadtree::UndoSplit() {
    if (!m_is_splited) return;
    m_is_splited = false;
    delete NW;
    delete NE;
    delete SW;
    delete SE;
    NW = nullptr;
    NE = nullptr;
    SW = nullptr;
    SE = nullptr;
}

bool Quadtree::InsertToQuads(const Vector2i *point) {
    return NW->Insert(point) || NE->Insert(point) || SW->Insert(point) || SE->Insert(point);
}
