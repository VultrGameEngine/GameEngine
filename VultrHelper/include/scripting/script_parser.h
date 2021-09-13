#pragma once

#include "script_scanner.h"
#include <filesystem/directory.h>
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
        std::vector<std::string> m_Namespaces;
        std::string m_ClassName;
        HeaderAndSourceFile m_file;
        std::list<VVariable> m_Variables;
    };

    struct Component
    {
        std::string name;
        HeaderAndSourceFile m_file;
    };

    class ScriptParser
    {
      public:
        ScriptParser(std::vector<Token> &tokens, HeaderAndSourceFile &file) : m_Tokens(tokens), m_FullFilepath(file)
        {
        }

        std::string GenerateHeaderFile();
        std::string GenerateSourceFile();
        std::string InitFile();
        void DebugPrint();
        void Parse();

        static std::string GetFilenameAsClassName(std::string filename);

      private:
        VClass ParseClass(std::vector<std::string> p_namespace);
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
        const HeaderAndSourceFile &m_FullFilepath;

        std::vector<Token> &m_Tokens;
        std::vector<VClass> m_components;
        std::vector<VClass> m_systems;
    };
} // namespace Vultr
