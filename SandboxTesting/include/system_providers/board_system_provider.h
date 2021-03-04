#pragma once
#include <Vultr.hpp>
#include <game_of_life.h>
#include <mqueue.h>
#include <components/cell_component.h>

struct QueuedCellUpdate
{
    Entity entity;
    CellComponent component;

    QueuedCellUpdate();
    QueuedCellUpdate(Entity p_entity, CellComponent p_component)
        : entity(p_entity), component(p_component)
    {
    }
};

class BoardSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<BoardSystemProvider> Get();
    std::queue<QueuedCellUpdate> update_queue;
    Entity m_entities[ROWS][COLS][ZOLS];

    static Entity GetCell(int row, int col, int zol);
    bool update = false;

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};
