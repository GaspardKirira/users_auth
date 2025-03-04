#ifndef EMAIL_HPP
#define EMAIL_HPP

#include <string>
#include <Adastra/validator.hpp>
#include <Adastra/Exception.hpp>

ADASTRA_EXCEPTION(InvalidEmailException, "Email invalid", ErrorCode::InvalidInput);

class EmailValidator
{
public:
    static void validate(const std::string &email)
    {
        if (!Adastra::Validator::validateEmail(email))
        {
            throw InvalidEmailException();
        }
    }
};

class Email
{
public:
    Email(const std::string &email) : m_email(email)
    {
        EmailValidator::validate(email);
    }

    const std::string &getEmail() const { return m_email; }

private:
    std::string m_email{};
};

#endif