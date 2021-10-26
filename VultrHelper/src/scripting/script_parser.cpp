#include <scripting/script_parser.h>
#include <iostream>
#include <sstream>
#include <stack>
#include <types/types.h>

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
        file << "#pragma once\n";

        // Includes
        size_t len;
        file << "#include \"../" << fbasename(&this->m_FullFilepath, &len) << "\"\n\n";

        for (auto component : m_components)
        {
            for (auto name_space : component.m_Namespaces)
            {
                file << "namespace " << name_space << " {\n";
            }

            file << "void to_json(json &j, const " << component.m_ClassName << " &m);\n";
            file << "void from_json(const json &j, " << component.m_ClassName << " &m);\n";

            // Trailing namespace brackets
            for (int i = 0; i < component.m_Namespaces.size(); i++)
            {
                file << "}\n";
            }
        }

        for (auto system : m_systems)
        {
            for (auto name_space : system.m_Namespaces)
            {
                file << "namespace " << name_space << " {\n";
            }

            // Trailing namespace brackets
            for (int i = 0; i < system.m_Namespaces.size(); i++)
            {
                file << "}\n";
            }
        }

        return file.str();
    }

    std::string ScriptParser::GenerateSourceFile()
    {
        if (m_components.size() == 0 && m_systems.size() == 0)
            return "";
        std::ostringstream file;

        // Includes
        file << "#include <vultr.hpp>\n\n";

        size_t len;
        file << "#include \"../" << fbasename(&this->m_FullFilepath, &len) << "\"\n\n";

        std::string generated_header = std::string(fbasename(&m_FullFilepath, &len)).substr(0, strlen(fbasename(&m_FullFilepath, &len)) - 2) + ".generated.h";

        file << "#include \"" << generated_header << "\"\n";

        file << "#include <json/json.hpp>\n";

        file << "using namespace Vultr;\n";

        int i = 0;
        for (auto component : m_components)
        {
            if (component.m_Namespaces.size() > 0)
            {
                file << "using namespace ";
                uint name_space_level = 0;
                for (auto name_space : component.m_Namespaces)
                {
                    file << name_space;
                    name_space_level++;
                    if (name_space_level < component.m_Namespaces.size())
                    {
                        file << "::";
                    }
                }
                file << ";\n";
            }
            file << "template <class Archive> void serialize(Archive &ar, " << component.m_ClassName << " &c)\n";
            file << "{\n";
            file << "\tar(";
            int j = 0;
            for (auto uvar : component.m_Variables)
            {
                file << "c." << uvar.m_Identifier;
                if (j < component.m_Variables.size() - 1)
                    file << ", ";
                j++;
            }
            file << ");\n";
            file << "}\n";
            file << "template <> void Vultr::RenderComponent<" << component.m_ClassName << ">(Entity entity)\n";
            file << "{\n";
            file << "\t" << component.m_ClassName << " *component = entity_get_component_unsafe<" << component.m_ClassName << ">(entity);\n";
            file << "\tif(component == nullptr)\n";
            file << "\t\treturn;\n";
            file << "\tif(ImGui::CollapsingHeader(\"" << component.m_ClassName << "\"))\n";
            file << "\t{\n";
            for (auto uvar : component.m_Variables)
            {
                file << "\t\tImGui::PushID(\"" << component.m_ClassName << uvar.m_Identifier << "\");\n";
                file << "\t\tRenderMember(\"" << uvar.m_Identifier << "\", "
                     << "component->" << uvar.m_Identifier << ");\n";
                file << "\t\tImGui::PopID();\n";
            }
            file << "\t\tif (ImGui::Button(\"Remove\"))\n";
            file << "\t\t{\n";
            file << "\t\t\tentity_remove_component<" << component.m_ClassName << ">(entity);\n";
            file << "\t\t}\n";
            file << "\t}\n";
            file << "}\n";

            j = 0;
            file << "template<> const char* Vultr::get_struct_name<";
            file << component.m_ClassName << ">()\n";
            file << "{\n";
            file << "\treturn \"" << component.m_ClassName << "\";\n";
            file << "}\n";

            i++;

            file << "void to_json(json &j, const " << component.m_ClassName << " &m)\n{\n";
            for (auto member : component.m_Variables)
            {

                file << "\tj[\"" << member.m_Identifier << "\"] = m." << member.m_Identifier << ";\n";
            }
            file << "}\n";

            file << "void from_json(const json &j, " << component.m_ClassName << " &m)\n{\n";
            for (auto member : component.m_Variables)
            {
                file << "\tif (j.contains(\"" << member.m_Identifier << "\"))\n";
                file << "\t{\n";
                file << "\t\tm." << member.m_Identifier << " = j[\"" << member.m_Identifier << "\"];\n";
                file << "\t}\n";
                file << "\telse\n\t{\n";
                // This code makes me want to die but it works
                file << "\t\tstd::cout << \" Value not found in saved file for member " << member.m_Identifier << " in component " << component.m_ClassName << "\" << __FILE__ << \", \" << __LINE__ << std::endl;\n";
                file << "\t}\n";
            }
            file << "}\n";
            file << "template<> void Vultr::ComponentArray<" << component.m_ClassName << ">::to_json(json &j) const \n";
            file << "{\n";
            file << "\tj[\"entity_to_index_map\"] = entity_to_index_map;\n";
            file << "\tj[\"index_to_entity_map\"] = index_to_entity_map;\n";
            file << "\tj[\"size\"] = size;\n";
            file << "\tfor (int i = 0; i < size; i++)\n";
            file << "\t{\n";
            file << "\t\tj[\"component_array\"].push_back(component_array[i]);\n";
            file << "\t}\n";
            file << "}\n";

            file << "template<> void Vultr::ComponentArray<" << component.m_ClassName << ">::from_json(const json &j) \n";
            file << "{\n";
            file << "\tif (j.find(\"component_array\") != j.end())\n";
            file << "\t{\n";
            file << "\t\tfor (auto &component : j[\"component_array\"].items())\n";
            file << "\t\t{\n";
            file << "\t\t\tcomponent_array[atoi(component.key().c_str())] = component.value();\n";
            file << "\t\t}\n";
            file << "\t}\n";
            file << "\tif (j.find(\"index_to_entity_map\") != j.end())\n";
            file << "\t{\n";
            file << "\t\tindex_to_entity_map = j[\"index_to_entity_map\"].get<std::unordered_map<size_t, Entity>>();\n";
            file << "\t}\n";
            file << "\tif (j.find(\"entity_to_index_map\") != j.end())\n";
            file << "\t{\n";
            file << "\t\tentity_to_index_map = j[\"entity_to_index_map\"].get<std::unordered_map<Entity, size_t>>();\n";
            file << "\t}\n";
            file << "\tsize = j[\"size\"].get<size_t>();\n";
            file << "}\n";
        }

        i = 0;
        for (auto system : m_systems)
        {
            file << "template<> const char* Vultr::get_struct_name<";
            for (auto name_space : system.m_Namespaces)
            {
                file << name_space << "::";
            }
            file << system.m_ClassName << ">()\n";
            file << "{\n";
            file << "\treturn \"" << system.m_Namespaces.back() << "\";\n";
            file << "}\n";
            // file << "template <class Archive> void serialize(Archive &ar, " << system.m_ClassName << " &s)\n";
            // file << "{\n";
            // file << "\tar(cereal::base_class<SystemProvider>(&s)";
            // if (system.m_Variables.size() > 0)
            // {
            //     file << ", ";
            //     int j = 0;
            //     for (auto uvar : system.m_Variables)
            //     {
            //         file << "s." << uvar.m_Identifier;
            //         if (j < system.m_Variables.size() - 1)
            //             file << ", ";
            //         j++;
            //     }
            // }
            // file << ");\n";
            // file << "}\n";
        }

        return file.str();
    }

    std::string ScriptParser::InitFile()
    {
        if (m_components.size() == 0 && m_systems.size() == 0)
            return "";
        std::ostringstream file;

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
        std::vector<std::string> namespaces;
        do
        {
            if (Match(TokenType::NAMESPACE_KW))
            {
                namespaces.push_back(Expect(TokenType::IDENTIFIER).m_Lexeme);
                while (Match(TokenType::COLON))
                {
                    Expect(TokenType::COLON);
                    namespaces.push_back(Expect(TokenType::IDENTIFIER).m_Lexeme);
                }
            }
            else if (Match(TokenType::RIGHT_BRACKET))
            {
                namespaces.pop_back();
            }
            else if (Match(TokenType::COMPONENT_PROP))
            {
                Expect(TokenType::LEFT_PAREN);
                Expect(TokenType::RIGHT_PAREN);
                Match(TokenType::SEMICOLON);
                Expect(TokenType::STRUCT_KW);
                VClass res = ParseClass(namespaces);
                m_components.push_back(res);
            }
            else if (Match(TokenType::SYSTEM_PROP))
            {
                Expect(TokenType::LEFT_PAREN);
                Expect(TokenType::RIGHT_PAREN);
                Match(TokenType::SEMICOLON);
                Expect(TokenType::STRUCT_KW);
                VClass res = ParseClass(namespaces);
                m_systems.push_back(res);
            }
            else
            {
                m_CurrentToken++;
                m_CurrentIter++;
            }
        } while (m_CurrentToken < m_Tokens.size() && m_CurrentIter->m_Type != TokenType::END_OF_FILE);
    }

    VClass ScriptParser::ParseClass(std::vector<std::string> p_namespaces)
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

        VClass clazz = VClass{p_namespaces, classType.m_Lexeme, m_FullFilepath, std::list<VVariable>()};

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
            if (m_CurrentIter->m_Type == TokenType::SEMICOLON && afterIdentifierIndex == -1)
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
        } while (current->m_Type != TokenType::END_OF_FILE && current->m_Type != TokenType::SEMICOLON);

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
