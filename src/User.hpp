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

    int m_id{};
    std::shared_ptr<FullName> m_fullname;
    std::shared_ptr<Email> m_email;
    std::shared_ptr<PhoneNumber> m_phone;
    std::shared_ptr<Password> m_password;

    friend std::ostream &operator<<(std::ostream &os, const User &user);

    const int getId() const { return m_id; }

    const std::string &getFullName() const { return m_fullname->getFullName(); }
    const std::string &getEmail() const { return m_email->getEmail(); }
    const std::string &getPhone() const { return m_phone->getPhoneNumber(); }
    const std::string &getPassword() const { return m_password->getPassword(); }

    void setId(int id) { m_id = id; }
    void setFullName(const std::string &fullname) { m_fullname = std::make_shared<FullName>(fullname); }
    void setEmail(const std::string &email) { m_email = std::make_shared<Email>(email); }
    void setPhone(const std::string &phone) { m_phone = std::make_shared<PhoneNumber>(phone); }
    void setPassword(const std::string &password) { m_password = std::make_shared<Password>(password); }
};

std::ostream &operator<<(std::ostream &os, const User &user)
{
    return os << "FULLNAME: " << user.getFullName() << "\n"
              << "EMAIL: " << user.getEmail() << "\n"
              << "PHONE: " << user.getPhone() << "\n";
}

#endif
