#ifndef FULLNAME_HPP
#define FULLNAME_HPP

#include <string>
#include <Adastra/validator.hpp>
#include <Adastra/Exception.hpp>
#include <regex>

ADASTRA_EXCEPTION(InvalidFullNameException, "Invalid FullName", ErrorCode::InvalidInput)

#include <string>
#include <regex>
#include <iostream>

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
    static bool isValidFormat(const std::string &fullname)
    {
        size_t firstSpace = fullname.find(' ');
        if (firstSpace == std::string::npos || firstSpace == fullname.size() - 1)
        {
            return false;
        }

        return true;
    }

    static bool isValidCharacters(const std::string &fullname)
    {
        // Autoriser uniquement les lettres, accents, espaces, apostrophes et tirets
        const std::regex pattern("^[a-zA-Zàáâäãåçéèêëíîïóôöõúùûüñ' -]+$");
        return std::regex_match(fullname, pattern);
    }

    static std::string removeExtraSpaces(const std::string &fullname)
    {
        // Remplacer les espaces multiples par un seul espace
        std::string result = std::regex_replace(fullname, std::regex("\\s+"), " ");

        // Enlever les espaces au début et à la fin de la chaîne
        result.erase(0, result.find_first_not_of(' ')); // Enlever les espaces au début
        result.erase(result.find_last_not_of(' ') + 1); // Enlever les espaces à la fin

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
