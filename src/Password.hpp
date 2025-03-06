#ifndef PASSWORD_HPP
#define PASSWORD_HPP

#include <string>
#include <Adastra/Exception.hpp>
#include <regex>
#include <iostream>

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

        if (!isValidLength(password))
        {
            throw InvalidPasswordException("Le mot de passe doit contenir entre 8 et 20 caractères.");
        }

        if (!hasUpperAndLowerCase(password))
        {
            throw InvalidPasswordException("Le mot de passe doit contenir à la fois des lettres majuscules et minuscules.");
        }

        if (!hasDigits(password))
        {
            throw InvalidPasswordException("Le mot de passe doit contenir des chiffres.");
        }

        if (!hasSpecialCharacters(password))
        {
            throw InvalidPasswordException("Le mot de passe doit contenir des caractères spéciaux tels que @, #, $, etc.");
        }

        if (containsSpaces(password))
        {
            throw InvalidPasswordException("Le mot de passe ne doit pas contenir d'espaces.");
        }
    }

private:
    static bool isValidLength(const std::string &password)
    {
        return password.length() >= 8 && password.length() <= 20;
    }

    static bool hasUpperAndLowerCase(const std::string &password)
    {
        bool hasUpper = false;
        bool hasLower = false;

        for (char c : password)
        {
            if (std::isupper(c))
                hasUpper = true;
            if (std::islower(c))
                hasLower = true;
        }

        return hasUpper && hasLower;
    }

    static bool hasDigits(const std::string &password)
    {
        for (char c : password)
        {
            if (std::isdigit(c))
                return true;
        }
        return false;
    }

    static bool hasSpecialCharacters(const std::string &password)
    {
        static const std::regex specialChars(R"([!@#$%^&*(),.?":{}|<>])");
        return std::regex_search(password, specialChars);
    }

    static bool containsSpaces(const std::string &password)
    {
        return password.find(' ') != std::string::npos;
    }
};

class Password
{
public:
    Password(const std::string &password) : m_password(password)
    {
        PasswordValidator::validate(password);
    }

    const std::string &getPassword() const
    {
        return m_password;
    }

private:
    std::string m_password;
};

#endif