#ifndef USER_HPP
#define USER_HPP

#include "Password.hpp"
#include "FullName.hpp"
#include "PhoneNumber.hpp"
#include "Email.hpp"

#include <memory>
#include <iostream>

class User
{
public:
    User(const std::shared_ptr<FullName> fullname,
         const std::shared_ptr<Email> email,
         const std::shared_ptr<PhoneNumber> phone,
         const std::shared_ptr<Password> password)
        : m_fullname(fullname), m_email(email), m_phone(phone), m_password(password)
    {
    }

    ~User() = default;

    User(const User &other)
        : m_fullname(other.m_fullname), m_email(other.m_email),
          m_phone(other.m_phone), m_password(other.m_password) {}

    User &operator=(const User &other)
    {
        if (this != &other)
        {
            m_fullname = other.m_fullname;
            m_email = other.m_email;
            m_phone = other.m_phone;
            m_password = other.m_password;
        }
        return *this;
    }

    User(User &&other) noexcept
        : m_fullname(std::move(other.m_fullname)),
          m_email(std::move(other.m_email)),
          m_phone(std::move(other.m_phone)),
          m_password(std::move(other.m_password)) {}

    User &operator=(User &&other) noexcept
    {
        if (this != &other)
        {
            m_fullname = std::move(other.m_fullname);
            m_email = std::move(other.m_email);
            m_phone = std::move(other.m_phone);
            m_password = std::move(other.m_password);
        }
        return *this;
    }

    const std::string getFullName() const { return m_fullname ? m_fullname->getFullName() : ""; }
    const std::string getEmail() const { return m_email ? m_email->getEmail() : ""; }
    const std::string getPhone() const { return m_phone ? m_phone->getPhoneNumber() : ""; }
    const std::string getPassword() const { return m_password ? m_password->getPassword() : ""; }

    friend std::ostream &operator<<(std::ostream &os, const User &user);

private:
    std::shared_ptr<FullName> m_fullname;
    std::shared_ptr<Email> m_email;
    std::shared_ptr<PhoneNumber> m_phone;
    std::shared_ptr<Password> m_password;
};

std::ostream &operator<<(std::ostream &os, const User &user)
{
    return os << "FULLNAME: " << (user.m_fullname ? user.m_fullname->getFullName() : "Invalide") << "\n"
              << "EMAIL: " << (user.m_email ? user.m_email->getEmail() : "Invalide") << "\n"
              << "PHONE: " << (user.m_phone ? user.m_phone->getPhoneNumber() : "Invalide") << "\n";
}

#endif
