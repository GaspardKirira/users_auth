#ifndef PHONE_HPP
#define PHONE_HPP

#include <string>
#include <Adastra/validator.hpp>
#include <Adastra/Exception.hpp>

ADASTRA_EXCEPTION(InvalidPhoneException, "Phone number invalid", ErrorCode::InvalidInput)

class PhoneValidator
{
public:
    static void validate(const std::string &phone)
    {
        if (!Adastra::Validator::validatePhoneNumber(phone))
        {
            throw InvalidPhoneException();
        }
    }
};

class Phone
{
public:
    Phone(const std::string &phone_number) : m_phone(phone_number)
    {
        PhoneValidator::validate(phone_number);
    }

    const std::string &getPhone() const { return m_phone; }

private:
    std::string m_phone{};
};

#endif