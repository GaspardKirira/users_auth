#ifndef PHONE_HPP
#define PHONE_HPP

#include <string>
#include <Adastra/validator.hpp>
#include <Adastra/Exception.hpp>
#include <regex>
#include <iostream>

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

        // Vérifier que le numéro commence par '+' et un code pays
        if (!isValidFormat(phoneNumber))
        {
            throw InvalidPhoneNumberException("Le numéro de téléphone est invalide. Il doit commencer par '+' suivi du code pays.");
        }

        // Vérifier que le numéro est composé uniquement de chiffres après le code pays (en ignorant les espaces)
        if (!isValidNumber(phoneNumber))
        {
            throw InvalidPhoneNumberException("Le numéro de téléphone ne contient que des chiffres après le code pays.");
        }

        // Vérifier que le numéro a une longueur raisonnable
        if (!isValidLength(phoneNumber))
        {
            throw InvalidPhoneNumberException("Le numéro de téléphone a une longueur invalide.");
        }
    }

private:
    static bool isValidFormat(const std::string &phoneNumber)
    {
        // Vérifie que le numéro commence par '+' et est suivi d'un code pays et du numéro local
        const std::regex pattern(R"(^\+(\d{1,4})\s*(\d{7,15})$)");
        return std::regex_match(phoneNumber, pattern);
    }

    static bool isValidNumber(const std::string &phoneNumber)
    {
        // Le numéro après le code pays doit être composé uniquement de chiffres
        size_t plusPos = phoneNumber.find('+');
        std::string numberPart = phoneNumber.substr(plusPos + 1);

        // Supprimer les espaces avant de vérifier si les caractères sont des chiffres
        numberPart.erase(std::remove_if(numberPart.begin(), numberPart.end(), ::isspace), numberPart.end());

        // Vérifier que tous les caractères après le '+' sont des chiffres
        return std::all_of(numberPart.begin(), numberPart.end(), ::isdigit);
    }

    static bool isValidLength(const std::string &phoneNumber)
    {
        // Vérifie la longueur raisonnable du numéro
        size_t plusPos = phoneNumber.find('+');
        std::string numberPart = phoneNumber.substr(plusPos + 1);

        // Supprimer les espaces avant de mesurer la longueur
        numberPart.erase(std::remove_if(numberPart.begin(), numberPart.end(), ::isspace), numberPart.end());

        return numberPart.length() >= 10 && numberPart.length() <= 15;
    }
};

class PhoneNumber
{
public:
    PhoneNumber(const std::string &phoneNumber) : m_phoneNumber(phoneNumber)
    {
        // Valide le numéro de téléphone au moment de la création de l'objet
        PhoneNumberValidator::validate(phoneNumber);
    }

    const std::string &getPhoneNumber() const
    {
        return m_phoneNumber;
    }

private:
    std::string m_phoneNumber;
};

#endif
