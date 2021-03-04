#pragma once
#include <Vultr.hpp>
#include <system_providers/board_system_provider.h>

class BoardSystem
{
  public:
    static void RegisterSystem();
    static void Init(int rows, int cols, int zols);
    static void StartUpdate();
    static void Update();
    static void PrepareBoardForDisplay();
    static void OnCreateEntity(Entity entity);
    static void OnDestroyEntity(Entity entity);

  private:
    static void CommitQueue();
    static int GetNeighborsCount(const CellComponent &component);
    static bool IsAlive(int row, int col, int zol);
    static BoardSystemProvider &GetProvider()
    {
        return *BoardSystemProvider::Get();
    }
};
