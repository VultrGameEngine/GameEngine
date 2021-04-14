#include <scripting/script_parser.h>
#include <iostream>
#include <sstream>

namespace Vultr
{
static bool IsAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
static bool IsAlphaNumeric(char c)
{
    return IsAlpha(c) || (c >= '0' && c <= '9');
}

static bool IsVariableFriendly(char c)
{
    return IsAlphaNumeric(c) || c == '_';
}

std::string ScriptParser::GetFilenameAsClassName(std::string filename)
{
    for (int i = 0; i < filename.length(); i++)
    {
        if (i == 0 && !IsAlpha(filename[i]))
        {
            filename[i] = '_';
        }
        else if (!IsVariableFriendly(filename[i]))
        {
            filename[i] = '_';
        }
    }

    return filename;
}

std::string ScriptParser::GenerateHeaderFile()
{
    if (m_components.size() == 0 && m_systems.size() == 0)
        return "";
    std::ostringstream file;

    // Header guards
    file << "#pragma once\n\n";

    // Includes
    file << "#include <Vultr.hpp>\n\n";

    file << "#include \"../" << this->m_FullFilepath.GetName() << "\"\n\n";

    int i = 0;
    for (auto uclass : m_components)
    {
        file << "template <class Archive> void serialize(Archive &ar, "
             << uclass.m_ClassName << " &c)\n";
        file << "{\n";
        file << "\tar(";
        int j = 0;
        for (auto uvar : uclass.m_Variables)
        {
            file << "c." << uvar.m_Identifier;
            if (j < uclass.m_Variables.size() - 1)
                file << ", ";
            j++;
        }
        file << ");\n";
        file << "}\n";
        file << "template <> inline void RenderComponent<" << uclass.m_ClassName
             << ">(Entity entity)\n";
        file << "{\n";
        file << "\t" << uclass.m_ClassName
             << " *component = entity.GetComponentUnsafe<" << uclass.m_ClassName
             << ">();\n";
        file << "\tif(component == nullptr)\n";
        file << "\t\treturn;\n";
        file << "\tif(ImGui::CollapsingHeader(\"" << uclass.m_ClassName << "\"))\n";
        file << "\t{\n";
        for (auto uvar : uclass.m_Variables)
        {
            file << "\t\tImGui::PushID(\"" << uclass.m_ClassName << uvar.m_Identifier
                 << "\");\n";
            file << "\t\tRenderMember(\"" << uvar.m_Identifier << "\", "
                 << "component->" << uvar.m_Identifier << ");\n";
            file << "\t\tImGui::PopID();\n";
        }
        file << "\t\tif (ImGui::Button(\"Remove\"))\n";
        file << "\t\t{\n";
        file << "\t\t\tentity.RemoveComponent<" << uclass.m_ClassName << ">();\n";
        file << "\t\t}\n";
        file << "\t}\n";
        file << "}\n";

        j = 0;

        i++;
    }

    i = 0;
    for (auto system : m_systems)
    {
        file << "CEREAL_REGISTER_TYPE(" << system.m_ClassName << ")\n";
        file << "template<> inline std::string GetName<" << system.m_ClassName
             << ">()\n";
        file << "{\n";
        file << "\treturn \"" << system.m_ClassName << "\";\n";
        file << "}\n";
        file << "template <class Archive> void serialize(Archive &ar, "
             << system.m_ClassName << " &s)\n";
        file << "{\n";
        file << "\tar(cereal::base_class<SystemProvider>(&s)";
        if (system.m_Variables.size() > 0)
        {
            file << ", ";
            int j = 0;
            for (auto uvar : system.m_Variables)
            {
                file << "s." << uvar.m_Identifier;
                if (j < system.m_Variables.size() - 1)
                    file << ", ";
                j++;
            }
        }
        file << ");\n";
        file << "}\n";
    }

    // i = 0;
    // for (auto uclass : m_Classes)
    // {
    //     int j = 0;
    //     for (auto uvar : uclass.m_Variables)
    //     {
    //         if (i == m_Classes.size() - 1 && j == uclass.m_Variables.size() - 1)
    //             file << "\t\t\t\"" << uclass.m_ClassName.c_str()
    //                  << "::" << uvar.m_Identifier.c_str() << "\"_hs\n";
    //         else
    //             file << "\t\t\t\"" << uclass.m_ClassName.c_str()
    //                  << "::" << uvar.m_Identifier.c_str() << "\"_hs,\n";

    //         j++;
    //     }

    //     i++;
    // }

    // file << "\t\t};\n\n";

    return file.str();
}

void ScriptParser::DebugPrint()
{
    // for (auto structIter = m_Structs.begin(); structIter != m_Structs.end();
    //      structIter++)
    // {
    //     // Log::Info("%s {", structIter->m_StructName.c_str());
    //     for (auto varIter = structIter->m_Variables.begin();
    //          varIter != structIter->m_Variables.end(); varIter++)
    //     {
    //         // Log::Info("Type<%s> %s", varIter->m_Type.c_str(),
    //         //           varIter->m_Identifier.c_str());
    //     }
    //     // Log::Info("}");
    // }

    // for (auto classIter = m_Classes.begin(); classIter != m_Classes.end();
    //      classIter++)
    // {
    //     // Log::Info("%s {", classIter->m_ClassName.c_str());
    //     for (auto varIter = classIter->m_Variables.begin();
    //          varIter != classIter->m_Variables.end(); varIter++)
    //     {
    //         // Log::Info("Type<%s> %s", varIter->m_Type.c_str(),
    //         //           varIter->m_Identifier.c_str());
    //     }
    //     // Log::Info("}");
    // }
}

void ScriptParser::Parse()
{
    m_CurrentToken = 0;
    m_CurrentIter = m_Tokens.begin();
    do
    {
        if (Match(TokenType::COMPONENT_PROP))
        {
            Expect(TokenType::LEFT_PAREN);
            Expect(TokenType::RIGHT_PAREN);
            Match(TokenType::SEMICOLON);
            Expect(TokenType::STRUCT_KW);
            VClass res = ParseClass();
            m_components.push_back(res);
        }
        else if (Match(TokenType::SYSTEM_PROP))
        {
            Expect(TokenType::LEFT_PAREN);
            Expect(TokenType::RIGHT_PAREN);
            Match(TokenType::SEMICOLON);
            Expect(TokenType::CLASS_KW);
            VClass res = ParseClass();
            m_systems.push_back(res);
        }
        else
        {
            m_CurrentToken++;
            m_CurrentIter++;
        }
    } while (m_CurrentToken < m_Tokens.size() &&
             m_CurrentIter->m_Type != TokenType::END_OF_FILE);
}

VClass ScriptParser::ParseClass()
{
    Token classType = Expect(TokenType::IDENTIFIER);

    if (Match(TokenType::COLON))
    {
        while (!Match(TokenType::LEFT_BRACKET))
        {
            m_CurrentToken++;
            m_CurrentIter++;
        }
    }
    else
    {
        Expect(TokenType::LEFT_BRACKET);
    }

    VClass clazz =
        VClass{classType.m_Lexeme, m_FullFilepath.GetPath(), std::list<VVariable>()};

    int level = 1;
    while (m_CurrentIter->m_Type != TokenType::END_OF_FILE)
    {
        if (Match(TokenType::LEFT_BRACKET))
        {
            level++;
        }
        else if (Match(TokenType::RIGHT_BRACKET))
        {
            level--;
            if (level <= 0)
            {
                Expect(TokenType::SEMICOLON);
                break;
            }
        }
        else if (Match(TokenType::PROPERTY))
        {
            Expect(TokenType::LEFT_PAREN);
            Match(TokenType::IDENTIFIER); // Consume any EditAnywhere type thing, it
                                          // doesn't do anything for now...
            Expect(TokenType::RIGHT_PAREN);
            Match(TokenType::SEMICOLON); // Consume a semicolon if it is there, this
                                         // is to help with indentation
            clazz.m_Variables.push_back(ParseVariable());
        }
        else
        {
            m_CurrentToken++;
            m_CurrentIter++;
        }
    }

    // m_Classes.push_back(clazz);
    return clazz;
}

VVariable ScriptParser::ParseVariable()
{
    std::vector<Token> allTokensBeforeSemiColon = std::vector<Token>();
    std::vector<Token>::iterator current;
    int afterIdentifierIndex = -1;

    do
    {
        current = m_Tokens.begin();
        std::advance(current, m_CurrentToken);
        if (m_CurrentIter->m_Type == TokenType::SEMICOLON &&
            afterIdentifierIndex == -1)
        {
            afterIdentifierIndex = allTokensBeforeSemiColon.size();
        }
        else if (m_CurrentIter->m_Type == TokenType::EQUAL)
        {
            afterIdentifierIndex = allTokensBeforeSemiColon.size();
        }
        allTokensBeforeSemiColon.push_back(*current);
        m_CurrentToken++;
        m_CurrentIter++;
    } while (current->m_Type != TokenType::END_OF_FILE &&
             current->m_Type != TokenType::SEMICOLON);

    Token variableIdentifier = GenerateErrorToken();
    if (afterIdentifierIndex != -1)
    {
        if (afterIdentifierIndex <= 1)
        {
            // Log::Error("Weird equal sign placement.");
            // Log::Error("Line: %d, Column: %d", m_CurrentIter->m_Line,
            //            m_CurrentIter->m_Column);
        }
        auto it = allTokensBeforeSemiColon.begin();
        std::advance(it, afterIdentifierIndex - 1);
        variableIdentifier = *it;
    }
    else
    {
        // Log::Error("Cannot find variable identifier.");
        // Log::Error("Line: %d, Column: %d", m_CurrentIter->m_Line,
        //            m_CurrentIter->m_Column);
        return VVariable{"ERROR", variableIdentifier.m_Lexeme};
    }

    Token typeIdentifier = GenerateErrorToken();
    auto endIter = allTokensBeforeSemiColon.begin();
    std::advance(endIter, afterIdentifierIndex - 1);
    for (auto iter = allTokensBeforeSemiColon.begin(); iter != endIter; iter++)
    {
        if (iter->m_Type == TokenType::IDENTIFIER)
        {
            typeIdentifier = *iter;
            break;
        }
    }

    return VVariable{typeIdentifier.m_Lexeme, variableIdentifier.m_Lexeme};
}

const Token &ScriptParser::Expect(TokenType type)
{
    if (m_CurrentIter->m_Type != type)
    {
        // Log::Error("Error: Expected '%d' but instead got '%d'", type,
        //            m_CurrentIter->m_Type);
        // Log::Error("Line: %d, Column: %d", m_CurrentIter->m_Line,
        //            m_CurrentIter->m_Column);
        return GenerateErrorToken();
    }

    auto tokenToReturn = m_CurrentIter;
    m_CurrentToken++;
    m_CurrentIter++;
    return *tokenToReturn;
}

bool ScriptParser::Match(TokenType type)
{
    if (m_CurrentIter->m_Type == type)
    {
        m_CurrentToken++;
        m_CurrentIter++;
        return true;
    }

    return false;
}
} // namespace Vultr
