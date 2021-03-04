#include <system_providers/board_system_provider.h>
#include <systems/board_system.h>

std::shared_ptr<BoardSystemProvider> BoardSystemProvider::Get()
{
    return World::GetSystemProvider<BoardSystemProvider>();
}

Entity BoardSystemProvider::GetCell(int row, int col, int zol)
{
    if (row < 0 || row >= ROWS)
        return Entity(-1);
    if (col < 0 || col >= COLS)
        return Entity(-1);
    if (zol < 0 || zol >= ZOLS)
        return Entity(-1);
    return Get()->m_entities[row][col][zol];
}

void BoardSystemProvider::OnCreateEntity(Entity entity)
{
    BoardSystem::OnCreateEntity(entity);
}
void BoardSystemProvider::OnDestroyEntity(Entity entity)
{
    BoardSystem::OnDestroyEntity(entity);
}
