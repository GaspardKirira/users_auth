#ifndef FULLNAME_HPP
#define FULLNAME_HPP

#include <string>
#include <Adastra/validator.hpp>
#include <Adastra/Exception.hpp>
#include <regex>

// Déclaration de l'exception pour un nom complet invalide
ADASTRA_EXCEPTION(InvalidFullNameException, "Invalid FullName", ErrorCode::InvalidInput)

#include <string>
#include <regex>
#include <iostream>
#include <algorithm> // Pour std::adjacent_find

// Validator pour valider un nom complet
// Dans FullNameValidator.hpp

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

        // Nettoyer les espaces inutiles
        std::string cleanedFullName = removeExtraSpaces(fullname);
        std::cout << "Chaine nettoyée: " << cleanedFullName << std::endl;

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
    }

private:
    static bool isValidFormat(const std::string &fullname)
    {
        // Vérifie qu'il y a au moins deux mots séparés par un espace
        size_t firstSpace = fullname.find(' ');
        if (firstSpace == std::string::npos || firstSpace == fullname.size() - 1)
        {
            return false; // Pas de deuxième mot
        }

        return true;
    }

    static bool isValidCharacters(const std::string &fullname)
    {
        // Autorise uniquement les lettres, accents, espaces, apostrophes et tirets
        const std::regex pattern("^[a-zA-Zàáâäãåçéèêëíîïóôöõúùûüñ' -]+$");
        return std::regex_match(fullname, pattern);
    }

    static std::string removeExtraSpaces(const std::string &fullname)
    {
        // Remplace plusieurs espaces consécutifs par un seul espace
        std::regex spacePattern("\\s+");
        std::string result = std::regex_replace(fullname, spacePattern, " ");

        // Trim (supprimer les espaces au début et à la fin)
        size_t start = result.find_first_not_of(' ');
        size_t end = result.find_last_not_of(' ');

        if (start == std::string::npos || end == std::string::npos)
        {
            return ""; // Retourne une chaîne vide si tout est composé d'espaces
        }

        return result.substr(start, end - start + 1);
    }
};

// Classe représentant le nom complet
class FullName
{
public:
    FullName(const std::string &fullname) : m_fullname(fullname)
    {
        // Valide le nom complet au moment de la création de l'objet
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
