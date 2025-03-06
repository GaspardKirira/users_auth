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
        : m_fullname(fullname), m_email(email), m_phone(phone), m_password(password) {}

    std::weak_ptr<FullName> m_fullname;
    std::weak_ptr<Email> m_email;
    std::weak_ptr<PhoneNumber> m_phone;
    std::weak_ptr<Password> m_password;

    friend std::ostream &operator<<(std::ostream &os, const User &user);

    const std::string &getFullName() const
    {
        if (auto fullname = m_fullname.lock())
        {
            return fullname->getFullName();
        }
        static std::string empty = "";
        return empty;
    }

    const std::string &getEmail() const
    {
        if (auto email = m_email.lock())
        {
            return email->getEmail();
        }
        static std::string empty = "";
        return empty;
    }

    const std::string &getPhone() const
    {
        if (auto phone = m_phone.lock())
        {
            return phone->getPhoneNumber();
        }
        static std::string empty = "";
        return empty;
    }

    const std::string &getPassword() const
    {
        if (auto password = m_password.lock())
        {
            return password->getPassword();
        }
        static std::string empty = "";
        return empty;
    }
};

std::ostream &operator<<(std::ostream &os, const User &user)
{
    return os << "FULLNAME: " << user.getFullName() << "\n"
              << "EMAIL: " << user.getEmail() << "\n"
              << "PHONE: " << user.getPhone() << "\n";
}

#endif
