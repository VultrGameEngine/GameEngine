#include <engine.hpp>

namespace Vultr
{
    Engine &get_engine()
    {
        static Engine engine;
        return engine;
    }

    void Init(bool debug);
    void LoadGame(const std::string &path);
    void LoadGame(Game *game);
    void RegisterComponents();
    void InitSystems();
    void InitGame();
    void UpdateGame(float &t);
    void Destroy();
    double GetElapsedTime();
} // namespace Vultr
