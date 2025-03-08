#ifndef PHONE_HPP
#define PHONE_HPP

#include <string>
#include <Adastra/Exception.hpp>
#include <regex>
#include <iostream>
#include <algorithm>

ADASTRA_EXCEPTION(InvalidPhoneNumberException, "Invalid Phone Number", ErrorCode::InvalidInput)

class PhoneNumberValidator
{
public:
    static void validate(const std::string &phoneNumber)
    {
        if (phoneNumber.empty())
        {
            throw InvalidPhoneNumberException("Le numéro de téléphone ne peut pas être vide.");
        }

        std::string cleanedPhoneNumber = removeSpaces(phoneNumber);

        if (!isValidFormat(cleanedPhoneNumber))
        {
            throw InvalidPhoneNumberException("Le numéro de téléphone est invalide. Il doit commencer par '+' suivi du code pays.");
        }

        if (!isValidNumber(cleanedPhoneNumber))
        {
            throw InvalidPhoneNumberException("Le numéro de téléphone ne contient que des chiffres après le code pays.");
        }

        if (!isValidLength(cleanedPhoneNumber))
        {
            throw InvalidPhoneNumberException("Le numéro de téléphone a une longueur invalide.");
        }
    }

private:
    static std::string removeSpaces(const std::string &phoneNumber)
    {
        std::string cleanedPhoneNumber = phoneNumber;
        cleanedPhoneNumber.erase(std::remove_if(cleanedPhoneNumber.begin(), cleanedPhoneNumber.end(), ::isspace), cleanedPhoneNumber.end());
        return cleanedPhoneNumber;
    }

    static bool isValidFormat(const std::string &phoneNumber)
    {
        static const std::regex pattern(R"(^\+(\d{1,4})\d{7,15}$)");
        return std::regex_match(phoneNumber, pattern);
    }

    static bool isValidNumber(const std::string &phoneNumber)
    {
        if (phoneNumber[0] == '+')
        {
            return std::all_of(phoneNumber.begin() + 1, phoneNumber.end(), ::isdigit);
        }
        return false;
    }

    static bool isValidLength(const std::string &phoneNumber)
    {
        if (phoneNumber[0] == '+')
        {
            std::string cleanedPhoneNumber = phoneNumber.substr(1);

            if (cleanedPhoneNumber.length() < 8 || cleanedPhoneNumber.length() > 15)
            {
                return false;
            }
        }
        return true;
    }
};

class PhoneNumber
{
public:
    PhoneNumber(const std::string &phoneNumber) : m_phoneNumber(phoneNumber)
    {
        PhoneNumberValidator::validate(phoneNumber);
    }

    const std::string &getPhoneNumber() const
    {
        return m_phoneNumber;
    }

    void setPhoneNumber(const std::string &phoneNumber)
    {
        PhoneNumberValidator::validate(phoneNumber);
        m_phoneNumber = phoneNumber;
    }

private:
    std::string m_phoneNumber;
};

#endif
