#ifndef USERREPOSITORY_HPP
#define USERREPOSITORY_HPP

#include <optional>
#include <vector>
#include <Adastra/Database.hpp>
#include "User.hpp"

class IDeleteUser
{
public:
    virtual void deleteUser(const User &user) = 0;
    virtual ~IDeleteUser() = default;
};

class IGetUser
{
public:
    virtual std::optional<User> getUserById(int id) = 0;
    virtual ~IGetUser() = default;
};

class IGetAllUsers
{
public:
    virtual std::vector<User> getUsers() = 0;
    virtual ~IGetAllUsers() = default;
};

class ISave
{
public:
    virtual void save(const User &user) = 0;
    virtual ~ISave() = default;
};

class IUpdateUser
{
public:
    virtual void update(int id, const User &user) = 0;
    virtual ~IUpdateUser() = default;
};

class UserRepository : public ISave, public IDeleteUser, public IGetUser, public IGetAllUsers, public IUpdateUser
{
public:
    UserRepository(std::unique_ptr<Adastra::Database> &db) : m_db(std::move(db)) {}

    void save(const User &user) override
    {
        m_db->create("users", {"fullname", "email", "phone", "password"},
                     user.getFullName(), user.getEmail(), user.getPhone(), user.getPassword());
    }

    void deleteUser(const User &user) override
    {
        m_db->remove("users", "id = ?", user.getId());
    }

    std::optional<User> getUserById(int id) override
    {
        auto result = m_db->executePreparedQuery("SELECT * FROM users WHERE id = ?", id);

        if (result->next())
        {
            auto fullname = std::make_shared<FullName>(result->getString("fullname"));
            auto email = std::make_shared<Email>(result->getString("email"));
            auto phone = std::make_shared<PhoneNumber>(result->getString("phone"));
            auto password = std::make_shared<Password>(result->getString("password"));

            User user(fullname, email, phone, password);

            return user;
        }

        return std::nullopt;
    }

    std::vector<User> getUsers() override
    {
        std::vector<User> users;
        auto result = m_db->executeQuery("SELECT id,fullname,email,phone,password FROM users");

        while (result->next())
        {
            std::cout << result->getString("fullname") << std::endl;
            std::cout << result->getString("email") << std::endl;
            std::cout << result->getString("phone") << std::endl;
            std::cout << result->getString("password") << std::endl;

            auto fullname = std::make_shared<FullName>(result->getString("fullname"));
            auto email = std::make_shared<Email>(result->getString("email"));
            auto phone = std::make_shared<PhoneNumber>("phone");
            auto password = std::make_shared<Password>(result->getString("password"));
            std::cout << "************************" << std::endl;
            std::cout << fullname->getFullName() << std::endl;
            std::cout << email->getEmail() << std::endl;
            std::cout << phone->getPhoneNumber() << std::endl;
            std::cout << password->getPassword() << std::endl;

            User user(fullname, email, phone, password);
            users.push_back(user);
        }

        return users;
    }

    void update(int id, const User &user) override
    {
        // Construction de la requête SQL pour mettre à jour un utilisateur existant
        m_db->update("users", {"fullname", "email", "phone", "password"},
                     {"fullname = ?", "email = ?", "phone = ?", "password = ?"},
                     user.getFullName(), user.getEmail(), user.getPhone(), user.getPassword(), id);
    }

private:
    std::unique_ptr<Adastra::Database> m_db;
};

#endif // USERREPOSITORY_HPP
