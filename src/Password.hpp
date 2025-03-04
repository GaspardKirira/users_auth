#ifndef PASSWORD_HPP
#define PASSWORD_HPP

#include <string>
#include <Adastra/validator.hpp>
#include <Adastra/Exception.hpp>

class PasswordValidator
{
public:
    static void validate(const std::string &password)
    {
    }
};

class Password
{
public:
    Password(const std::string &password) : m_password(password) {}

private:
    std::string m_password{};
};

#endif