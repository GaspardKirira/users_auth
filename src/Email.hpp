#ifndef EMAIL_HPP
#define EMAIL_HPP

#include <string>
#include <Adastra/Exception.hpp>
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

        if (!isValidFormat(email.c_str())) // Changer l'argument pour un const char*
        {
            throw InvalidEmailException("L'email est invalide. Il doit respecter le format 'utilisateur@domaine.extension'.");
        }

        if (!isValidDomain(email.c_str())) // Changer l'argument pour un const char*
        {
            throw InvalidEmailException("Le domaine de l'email n'est pas valide.");
        }
    }

private:
    static bool isValidFormat(const char *email) // Utilisation de const char* au lieu de std::string
    {
        // La fonction de validation du format reste non-constexpr, car elle utilise std::regex
        static const std::regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        return std::regex_match(email, pattern);
    }

    static constexpr bool isValidDomain(const char *email) // Maintenant, la fonction est constexpr
    {
        size_t atPos = findAtPosition(email);
        if (atPos != std::string::npos)
        {
            const char *domain = email + atPos + 1;

            if (containsConsecutiveDots(domain))
            {
                return false;
            }

            return containsDot(domain);
        }
        return false;
    }

    // Fonction helper constexpr pour trouver la position de '@'
    static constexpr size_t findAtPosition(const char *str)
    {
        size_t pos = 0;
        while (str[pos] != '\0')
        {
            if (str[pos] == '@')
            {
                return pos;
            }
            ++pos;
        }
        return -1; // Retourne une valeur invalid si '@' n'est pas trouvé
    }

    // Fonction constexpr pour vérifier la présence de deux points consécutifs
    static constexpr bool containsConsecutiveDots(const char *str)
    {
        size_t i = 0;
        while (str[i] != '\0')
        {
            if (str[i] == '.' && str[i + 1] == '.')
            {
                return true;
            }
            ++i;
        }
        return false;
    }

    // Fonction constexpr pour vérifier la présence d'un point
    static constexpr bool containsDot(const char *str)
    {
        size_t i = 0;
        while (str[i] != '\0')
        {
            if (str[i] == '.')
            {
                return true;
            }
            ++i;
        }
        return false;
    }
};

class Email
{
public:
    Email(const std::string &email) : m_email(email)
    {
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
