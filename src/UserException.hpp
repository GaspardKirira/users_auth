#ifndef USEREXCEPTIONS_H
#define USEREXCEPTIONS_H

#include <exception>
#include <string>
#include <sstream>

// Enum pour les différents types d'erreurs utilisateur
enum class UserErrorCode
{
    UserNotFound,
    InvalidUserInfo,
    LoginFailed,
    InvalidEmail,
    InvalidPassword,
    InvalidPhoneNumber,
    InvalidFullName
};

// Classe de base pour les exceptions liées au module utilisateur
class UserException : public std::exception
{
public:
    // Constructeur avec message et code d'erreur
    explicit UserException(std::string_view message, UserErrorCode code) noexcept
        : message_("Erreur utilisateur : " + std::string(message)), code_(code) {}

    // Retourner le message d'erreur
    virtual const char *what() const noexcept override
    {
        return message_.c_str();
    }

    // Retourner le code d'erreur
    UserErrorCode code() const noexcept
    {
        return code_;
    }

protected:
    std::string message_;
    UserErrorCode code_;
};

// Exception lorsque l'utilisateur est introuvable
class UserNotFoundException : public UserException
{
public:
    explicit UserNotFoundException(std::string_view user) noexcept
        : UserException("Utilisateur '" + std::string(user) + "' non trouvé.", UserErrorCode::UserNotFound) {}
};

// Exception pour des informations d'utilisateur invalides
class InvalidUserInfoException : public UserException
{
public:
    explicit InvalidUserInfoException(std::string_view userInfo) noexcept
        : UserException("Informations invalides pour l'utilisateur : " + std::string(userInfo), UserErrorCode::InvalidUserInfo) {}
};

// Exception pour une tentative de connexion échouée
class LoginFailedException : public UserException
{
public:
    explicit LoginFailedException(std::string_view username) noexcept
        : UserException("Échec de la connexion pour l'utilisateur : " + std::string(username), UserErrorCode::LoginFailed) {}
};

// Exception pour un email invalide
class InvalidEmailException : public UserException
{
public:
    explicit InvalidEmailException(std::string_view email) noexcept
        : UserException("Email invalide : " + std::string(email), UserErrorCode::InvalidEmail) {}
};

// Exception pour un mot de passe invalide (par exemple, trop court)
class InvalidPasswordException : public UserException
{
public:
    explicit InvalidPasswordException(std::string_view password) noexcept
        : UserException("Mot de passe invalide : " + std::string(password), UserErrorCode::InvalidPassword) {}
};

// Exception pour un numéro de téléphone invalide
class InvalidPhoneNumberException : public UserException
{
public:
    explicit InvalidPhoneNumberException(std::string_view phoneNumber) noexcept
        : UserException("Numéro de téléphone invalide : " + std::string(phoneNumber), UserErrorCode::InvalidPhoneNumber) {}
};

// Exception pour un nom complet invalide (par exemple, trop court ou incomplet)
class InvalidFullNameException : public UserException
{
public:
    explicit InvalidFullNameException(std::string_view fullName) noexcept
        : UserException("Nom complet invalide : " + std::string(fullName), UserErrorCode::InvalidFullName) {}
};

#endif // USEREXCEPTIONS_H
