#include <Vultr.hpp>
#include <editor/editor.hpp>
#include <iostream>

using namespace Vultr;
int main(void)
{
    Engine *vultr = new Engine();

    float lastTime = 0;
    vultr->Init(true);
    vultr->LoadGame("/home/brandon/Dev/GameEngine/SandboxTesting/build/libGame.so");

    while (!vultr->should_close)
    {
        vultr->UpdateGame(lastTime);
        Vultr::Editor::Editor::Get()->Render();
        glfwSwapBuffers(vultr->window);
    }
    vultr->Destroy();

    // imgui_implopengl3_shutdown();
    // imgui_implglfw_shutdown();

    // ImGui::DestroyContext();
    // GL CALLBACK: ** GL ERROR ** type = 0x824c, severity = 0x9146, message = G
    // L_INVALID_OPERATION error generated. Function glColorPointer is deprecate
    // d and not available in preview contexts.
    // GL CALLBACK: ** GL ERROR ** type = 0x824c, severity = 0x9146, message = G
    // L_INVALID_ENUM error generated. Cannot enable <cap> in the current profil
    // e.
    // GL CALLBACK: ** GL ERROR ** type = 0x824c, severity = 0x9146, message = G
    // L_INVALID_OPERATION error generated. Cannot enable in the current profile
    // .
    // GL CALLBACK: ** GL ERROR ** type = 0x824c, severity = 0x9146, message = G
    // L_INVALID_OPERATION error generated. Function glTexCoordPointer is deprec
    // ated and not available in preview contexts.
    // GL CALLBACK: ** GL ERROR ** type = 0x824c, severity = 0x9146, message = G
    // L_INVALID_OPERATION error generated. Function glPopMatrix is deprecated a
    // nd not available in preview contexts.
    // 0.008392 ms
    // GL CALLBACK: ** GL ERROR ** type = 0x824c, severity = 0x9146, message = G
    // L_INVALID_OPERATION error generated. Function glPushMatrix is deprecated
    // and not available in preview contexts.
    // GL CALLBACK: ** GL ERROR ** type = 0x824c, severity = 0x9146, message = G
    // L_INVALID_OPERATION error generated. Function glTranslatef is deprecated
    // and not available in preview contexts.
    // GL CALLBACK: ** GL ERROR ** type = 0x824c, severity = 0x9146, message = G
    // L_INVALID_OPERATION error generated. Function glVertexPointer is deprecat
    // ed and not available in preview contexts.
    // GL CALLBACK: ** GL ERROR ** type = 0x824c, severity = 0x9146, message = G
    // L_INVALID_OPERATION error generated. Cannot enable in the current profile
    // .
}
