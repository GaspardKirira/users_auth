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

    friend std::ostream &operator<<(std::ostream &os, const User &user);

    const std::string &getFullName() const { return m_fullname->getFullName(); }
    const std::string &getEmail() const { return m_email->getEmail(); }
    const std::string &getPhone() const { return m_phone->getPhoneNumber(); }
    const std::string &getPassword() const { return m_password->getPassword(); }

private:
    std::shared_ptr<FullName> m_fullname;
    std::shared_ptr<Email> m_email;
    std::shared_ptr<PhoneNumber> m_phone;
    std::shared_ptr<Password> m_password;
};

std::ostream &operator<<(std::ostream &os, const User &user)
{
    return os << "FULLNAME: " << user.m_fullname->getFullName() << "\n"
              << "EMAIL: " << user.m_email->getEmail() << "\n"
              << "PHONE: " << user.m_phone->getPhoneNumber() << "\n";
}

#endif