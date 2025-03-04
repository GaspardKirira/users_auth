#ifndef USER_HPP
#define USER_HPP

#include "FullName.hpp"
#include "Phone.hpp"
#include "Email.hpp"

#include <memory>
#include <iostream>

class User
{
public:
    User(const std::shared_ptr<FullName> fullname,
         const std::shared_ptr<Email> email,
         const std::shared_ptr<Phone> phone)
        : m_fullname(fullname), m_email(email), m_phone(phone) {}

    friend std::ostream &operator<<(std::ostream &os, const User &user);

    const std::string &getFullName() const { return m_fullname->getFUllName(); }
    const std::string &getEmail() const { return m_email->getEmail(); }
    const std::string &getPhone() const { return m_phone->getPhone(); }

private:
    std::shared_ptr<FullName> m_fullname;
    std::shared_ptr<Email> m_email;
    std::shared_ptr<Phone> m_phone;
};

std::ostream &operator<<(std::ostream &os, const User &user)
{
    return os << "FULLNAME: " << user.m_fullname << "\n"
              << "EMAIL: " << user.m_email << "\n"
              << "PHONE: " << user.m_phone << "\n";
}

#endif