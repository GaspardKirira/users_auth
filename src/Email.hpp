#ifndef EMAIL_HPP
#define EMAIL_HPP

#include <string>
#include <Adastra/validator.hpp>
#include <Adastra/Exception.hpp>
#include <regex>
#include <iostream>

ADASTRA_EXCEPTION(InvalidEmailException, "Email invalid", ErrorCode::InvalidInput);

class EmailValidator
{
public:
    static void validate(const std::string &email)
    {
        if (email.empty())
        {
            throw InvalidEmailException("L'email ne peut pas être vide.");
        }

        if (!isValidFormat(email))
        {
            throw InvalidEmailException("L'email est invalide. Il doit respecter le format 'utilisateur@domaine.extension'.");
        }

        if (!isValidDomain(email))
        {
            throw InvalidEmailException("Le domaine de l'email n'est pas valide.");
        }
    }

private:
    static bool isValidFormat(const std::string &email)
    {
        // Regex pour valider le format général d'un email
        const std::regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        return std::regex_match(email, pattern);
    }

    static bool isValidDomain(const std::string &email)
    {
        // On vérifie la présence du domaine après le '@'
        size_t atPos = email.find('@');
        if (atPos != std::string::npos)
        {
            std::string domain = email.substr(atPos + 1);

            // Vérification qu'il n'y a pas de double point ".." dans le domaine
            if (domain.find("..") != std::string::npos)
            {
                return false; // Domaine invalide si ".." est trouvé
            }

            // Le domaine doit avoir une extension valide (par exemple ".com", ".org", etc.)
            return domain.find('.') != std::string::npos;
        }
        return false;
    }
};

class Email
{
public:
    Email(const std::string &email) : m_email(email)
    {
        // Valide l'email au moment de la création de l'objet
        EmailValidator::validate(email);
    }

    const std::string &getEmail() const
    {
        return m_email;
    }

private:
    std::string m_email;
};

#endif