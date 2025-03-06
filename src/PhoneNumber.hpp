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

        if (!isValidFormat(phoneNumber))
        {
            throw InvalidPhoneNumberException("Le numéro de téléphone est invalide. Il doit commencer par '+' suivi du code pays.");
        }

        if (!isValidNumber(phoneNumber))
        {
            throw InvalidPhoneNumberException("Le numéro de téléphone ne contient que des chiffres après le code pays.");
        }

        if (!isValidLength(phoneNumber))
        {
            throw InvalidPhoneNumberException("Le numéro de téléphone a une longueur invalide.");
        }
    }

private:
    static bool isValidFormat(const std::string &phoneNumber)
    {
        static const std::regex pattern(R"(^\+(\d{1,4})\s*(\d{7,15})$)");
        return std::regex_match(phoneNumber, pattern);
    }

    static bool isValidNumber(const std::string &phoneNumber)
    {
        size_t plusPos = phoneNumber.find('+');
        std::string numberPart = phoneNumber.substr(plusPos + 1);

        numberPart.erase(std::remove_if(numberPart.begin(), numberPart.end(), ::isspace), numberPart.end());

        return std::all_of(numberPart.begin(), numberPart.end(), ::isdigit);
    }

    static bool isValidLength(const std::string &phoneNumber)
    {
        size_t plusPos = phoneNumber.find('+');
        std::string numberPart = phoneNumber.substr(plusPos + 1);

        numberPart.erase(std::remove_if(numberPart.begin(), numberPart.end(), ::isspace), numberPart.end());

        return numberPart.length() >= 10 && numberPart.length() <= 15;
    }
};

class PhoneNumber
{
public:
    PhoneNumber(const std::string &phoneNumber)
    {
        try
        {
            PhoneNumberValidator::validate(phoneNumber);
            m_phoneNumber = phoneNumber;
        }
        catch (const InvalidPhoneNumberException &e)
        {
            std::cerr << "Erreur de validation : " << e.what() << std::endl;
            m_phoneNumber = "";
        }
    }

    const std::string &getPhoneNumber() const
    {
        return m_phoneNumber;
    }

private:
    std::string m_phoneNumber;
};

#endif
