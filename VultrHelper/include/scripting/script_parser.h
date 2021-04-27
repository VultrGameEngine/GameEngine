#pragma once

#include "script_scanner.h"
#include <helpers/directory.h>
#include <list>

namespace Vultr
{
struct VVariable
{
    std::string m_Type;
    std::string m_Identifier;
};

struct VClass
{
    std::string m_ClassName;
    File m_file;
    std::list<VVariable> m_Variables;
};

class ScriptParser
{
  public:
    ScriptParser(std::vector<Token> &tokens, File &file)
        : m_Tokens(tokens), m_FullFilepath(file)
    {
    }

    std::string GenerateHeaderFile();
    std::string InitFile();
    void DebugPrint();
    void Parse();

    static std::string GetFilenameAsClassName(std::string filename);

  private:
    VClass ParseClass();
    VVariable ParseVariable();
    const Token &Expect(TokenType type);
    bool Match(TokenType type);

  private:
    inline Token GenerateErrorToken()
    {
        return Token{-1, -1, TokenType::ERROR_TYPE, ""};
    }

  private:
    int m_CurrentToken;
    std::vector<Token>::iterator m_CurrentIter;
    const File &m_FullFilepath;

    std::vector<Token> &m_Tokens;
    std::vector<VClass> m_components;
    std::vector<VClass> m_systems;
};
} // namespace Vultr
