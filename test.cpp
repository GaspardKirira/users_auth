#include <iostream>
#include <regex>
#include <algorithm>
#include <stdexcept>

class InvalidPhoneNumberException : public std::exception
{
public:
    explicit InvalidPhoneNumberException(const std::string &msg) : message(msg) {}
    const char *what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};

class PhoneNumberValidator
{
public:
    static void validate(const std::string &phoneNumber)
    {
        if (phoneNumber.empty())
        {
            throw InvalidPhoneNumberException("Le numéro de téléphone ne peut pas être vide.");
        }

        // Nettoyage des espaces une seule fois
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
    // Méthode pour supprimer les espaces d'un numéro
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
            std::string cleanedPhoneNumber = phoneNumber.substr(1); // Supprimer le "+"

            // Vérifier la longueur totale du numéro (excluant le "+")
            return cleanedPhoneNumber.length() >= 8 && cleanedPhoneNumber.length() <= 15;
        }
        return false;
    }
};

int main()
{
    try
    {
        PhoneNumberValidator::validate("+1 1234567890");
        std::cout << "Numéro valide!" << std::endl;
    }
    catch (const InvalidPhoneNumberException &e)
    {
        std::cout << "Erreur : " << e.what() << std::endl;
    }

    try
    {
        PhoneNumberValidator::validate("+33 612345678");
        std::cout << "Numéro valide!" << std::endl;
    }
    catch (const InvalidPhoneNumberException &e)
    {
        std::cout << "Erreur : " << e.what() << std::endl;
    }

    try
    {
        PhoneNumberValidator::validate("+44 7912345678");
        std::cout << "Numéro valide!" << std::endl;
    }
    catch (const InvalidPhoneNumberException &e)
    {
        std::cout << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}
