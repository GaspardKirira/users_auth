#ifndef PASSWORD_HPP
#define PASSWORD_HPP

#include <string>
#include <regex>
#include <Adastra/validator.hpp>
#include <Adastra/Exception.hpp>

ADASTRA_EXCEPTION(InvalidPasswordException, "Invalid Password", ErrorCode::InvalidInput)

class PasswordValidator
{
public:
    static void validate(const std::string &password)
    {
        if (password.empty())
        {
            throw InvalidPasswordException("Le mot de passe ne peut pas être vide.");
        }

        if (password.length() < 8)
        {
            throw InvalidPasswordException("Le mot de passe doit contenir au moins 8 caractères.");
        }

        std::regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&+])[A-Za-z\\d@$!%*?&+]{8,}$");
        if (!std::regex_match(password, pattern))
        {
            throw InvalidPasswordException("Le mot de passe doit contenir des majuscules, minuscules, chiffres et caractères spéciaux.");
        }
    }
};

class Password
{
public:
    Password(const std::string &password)
    {
        PasswordValidator::validate(password);
        m_password = password;
    }

private:
    std::string m_password{};
};

#endif
