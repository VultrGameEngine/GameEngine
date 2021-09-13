#pragma once
#include <map>
#include <string>
#include <vector>
#include <filesystem/file.h>

namespace Vultr
{
    enum TokenType
    {
        PROPERTY,
        FUNCTION_PROP,
        COMPONENT_PROP,
        SYSTEM_PROP,
        STRUCT_PROP,
        STRUCT_KW,
        CLASS_KW,
        NAMESPACE_KW,
        COLON,

        LEFT_PAREN,
        RIGHT_PAREN,
        HASHTAG,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        SEMICOLON,
        EQUAL,
        PLUS_EQUAL,
        MINUS_EQUAL,
        TIMES_EQUAL,
        DIV_EQUAL,
        MODULO_EQUAL,
        CARET_EQUAL,
        AMPERSAND_EQUAL,
        BAR_EQUAL,
        RIGHT_SHIFT_EQUAL,
        LEFT_SHIFT_EQUAL,
        EQUAL_EQUAL,
        BANG_EQUAL,
        LESS_THAN_EQUAL,
        GREATER_THAN_EQUAL,

        CONST_KW,
        IDENTIFIER,
        AUTO_KW,

        STAR,
        REF,
        LEFT_ANGLE_BRACKET,
        RIGHT_ANGLE_BRACKET,

        STRING_LITERAL,
        NUMBER,
        TRUE_KW,
        FALSE_KW,

        END_OF_FILE,
        ERROR_TYPE
    };

    struct Token
    {
        int m_Line;
        int m_Column;
        TokenType m_Type;
        std::string m_Lexeme;
        // void* m_Literal;
    };

    class ScriptScanner
    {
      public:
        ScriptScanner(const HeaderAndSourceFile &file);

        std::vector<Token> ScanTokens();

      private:
        Token ScanToken();
        Token PropertyIdentifier();
        Token Number();
        Token String();

        char Advance();
        char Peek();
        char PeekNext();
        char PeekNextNext();
        bool Match(char expected);

      private:
        inline bool IsDigit(char c) const
        {
            return c >= '0' && c <= '9';
        }

        inline bool IsAlphaNumeric(char c) const
        {
            return IsAlpha(c) || IsDigit(c);
        }

        inline bool IsAlpha(char c) const
        {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        }

        inline bool AtEnd() const
        {
            return m_Cursor == m_FileContents.size();
        }

        inline Token GenerateToken(TokenType m_Type, std::string lexeme)
        {
            return Token{m_Line, m_Column, m_Type, lexeme};
        }

        inline Token GenerateErrorToken()
        {
            return Token{-1, -1, TokenType::ERROR_TYPE, ""};
        }

      private:
        const std::map<std::string, TokenType> keywords = {{"VPROPERTY", TokenType::PROPERTY},    {"VSYSTEM", TokenType::SYSTEM_PROP},     {"VCOMPONENT", TokenType::COMPONENT_PROP},
                                                           {"VSTRUCT", TokenType::STRUCT_PROP},   {"VFUNCTION", TokenType::FUNCTION_PROP}, {"auto", TokenType::AUTO_KW},
                                                           {"const", TokenType::CONST_KW},        {"class", TokenType::CLASS_KW},          {"struct", TokenType::STRUCT_KW},
                                                           {"namespace", TokenType::NAMESPACE_KW}};

        std::string m_FileContents;
        HeaderAndSourceFile m_Filepath;

        int m_Cursor = 0;
        int m_Line = 1;
        int m_Column = 0;
        int m_Start = 0;
    };
} // namespace Vultr
