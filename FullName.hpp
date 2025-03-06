#ifndef FULLNAME_HPP
#define FULLNAME_HPP

#include <string>
#include <Adastra/validator.hpp>
#include <Adastra/Exception.hpp>
#include <iostream>
#include <cctype> // pour isalpha, etc.
#include <locale> // pour utiliser la locale et les caractères Unicode

ADASTRA_EXCEPTION(InvalidFullNameException, "Invalid FullName", ErrorCode::InvalidInput)

class FullNameValidator
{
public:
    static void validate(const std::string &fullname)
    {
        if (fullname.empty())
        {
            throw InvalidFullNameException("Le nom complet ne peut pas être vide.");
        }

        if (fullname.find_first_not_of(' ') == std::string::npos)
        {
            throw InvalidFullNameException("Le nom complet ne peut pas être constitué uniquement d'espaces.");
        }

        std::string cleanedFullName = removeExtraSpaces(fullname);

        if (cleanedFullName.empty())
        {
            throw InvalidFullNameException("Le nom complet ne peut pas être constitué uniquement d'espaces.");
        }

        if (!isValidCharacters(cleanedFullName))
        {
            throw InvalidFullNameException("Le nom complet contient des caractères invalides.");
        }

        if (!isValidFormat(cleanedFullName))
        {
            throw InvalidFullNameException("Le nom complet est invalide. Seules les lettres, les espaces, les apostrophes et les tirets sont autorisés.");
        }

        if (cleanedFullName.find(' ') == std::string::npos)
        {
            throw InvalidFullNameException("Le nom complet doit comporter un prénom et un nom.");
        }
    }

private:
    static bool isValidCharacters(const std::string &fullname)
    {
        // Vérifier chaque caractère
        for (char c : fullname)
        {
            // Si le caractère n'est ni une lettre, ni un espace, ni un tiret, ni une apostrophe
            // Ou s'il n'est pas un caractère Unicode valide, on retourne false
            if (!(std::isalpha(c, std::locale()) || c == ' ' || c == '\'' || c == '-'))
            {
                return false;
            }
        }
        return true;
    }

    static bool isValidFormat(const std::string &fullname)
    {
        size_t firstSpace = fullname.find(' ');
        if (firstSpace == std::string::npos || firstSpace == fullname.size() - 1)
        {
            return false;
        }

        return true;
    }

    static std::string removeExtraSpaces(const std::string &fullname)
    {
        std::string result;
        bool inSpaces = false;

        for (char c : fullname)
        {
            if (c != ' ' || !inSpaces)
            {
                result += c;
            }
            inSpaces = (c == ' ');
        }

        // Supprimer les espaces au début et à la fin
        if (!result.empty() && result[0] == ' ')
        {
            result.erase(0, 1);
        }
        if (!result.empty() && result[result.size() - 1] == ' ')
        {
            result.erase(result.size() - 1, 1);
        }

        return result;
    }
};

class FullName
{
public:
    FullName(const std::string &fullname) : m_fullname(fullname)
    {
        FullNameValidator::validate(fullname);
    }

    const std::string &getFullName() const
    {
        return m_fullname;
    }

private:
    std::string m_fullname;
};

#endif
