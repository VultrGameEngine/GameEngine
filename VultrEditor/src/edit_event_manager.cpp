#include <editor.h>
#include <core/models/event.h>

using namespace Vultr;
void on_edit(void *_editor, EditEvent *e)
{
    auto *editor = static_cast<Editor *>(_editor);
    auto &m = editor->edit_event_manager;
    s32 stack_size = m.event_stack.size();
    if (m.event_index < stack_size - 1)
    {
        for (int i = m.event_index + 1; i < m.event_stack.size(); i++)
        {
            delete m.event_stack[i];
        }
        m.event_stack.erase(m.event_stack.begin() + m.event_index + 1, m.event_stack.end());
    }
    m.event_index++;
    m.event_stack.push_back(e);
}

void undo(Engine *e, Editor *editor)
{
    auto &m = editor->edit_event_manager;
    if (m.event_stack.size() == 0)
        return;
    if (m.event_index <= -1)
        return;
    auto *event = m.event_stack[m.event_index];
    event->Undo(e);
    m.event_index--;
}

void redo(Engine *e, Editor *editor)
{
    auto &m = editor->edit_event_manager;
    if (m.event_stack.size() == 0)
        return;
    s32 stack_size = m.event_stack.size();
    if (m.event_index + 1 >= stack_size)
        return;
    m.event_index++;
    auto *event = m.event_stack[m.event_index];
    event->Redo(e);
}
