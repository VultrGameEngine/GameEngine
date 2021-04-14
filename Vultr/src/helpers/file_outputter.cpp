#include <helpers/file_outputter.h>

namespace Vultr
{
FileOutputter::FileOutputter(File &p_file) : m_file(p_file)
{
    os = std::ofstream();
    os.open(m_file.GetPath());
}
FileOutputter::~FileOutputter()
{
    os.close();
}
} // namespace Vultr
