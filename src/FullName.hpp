#ifndef FULLNAME_HPP
#define FULLNAME_HPP

#include <string>
#include <Adastra/validator.hpp>
#include <Adastra/Exception.hpp>
#include <regex>

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
    static const std::regex validCharactersRegex;

    static bool isValidCharacters(const std::string &fullname)
    {
        return std::regex_match(fullname, validCharactersRegex);
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
        std::string result = std::regex_replace(fullname, std::regex("\\s+"), " ");
        result.erase(0, result.find_first_not_of(' '));
        result.erase(result.find_last_not_of(' ') + 1);
        return result;
    }
};

const std::regex FullNameValidator::validCharactersRegex("^[a-zA-Zàáâäãåçéèêëíîïóôöõúùûüñ' -]+$");

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

    void setFullName(const std::string &fullname)
    {
        FullNameValidator::validate(fullname);
        m_fullname = fullname;
    }

private:
    std::string m_fullname;
};

#endif