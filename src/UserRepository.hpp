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

class UserRepository : public ISave, public IDeleteUser, public IGetUser, public IGetAllUsers
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
        try
        {
            std::cout << "Tentative de suppression de l'utilisateur avec ID : " << user.getId() << std::endl;

            m_db->beginTransaction();

            m_db->remove("users", "id = ?", user.getId());

            auto result = m_db->executePreparedQuery("SELECT COUNT(*) FROM users WHERE id = ?", user.getId());

            if (result->next())
            {
                int count = result->getInt(1);
                std::cout << "Nombre d'utilisateurs restants avec ID " << user.getId() << ": " << count << std::endl;
                if (count == 0)
                {
                    std::cout << "Utilisateur supprimé avec succès." << std::endl;
                    m_db->commit();
                }
                else
                {
                    std::cerr << "L'utilisateur n'a pas été supprimé." << std::endl;
                    m_db->rollback();
                }
            }
            else
            {
                std::cerr << "Erreur lors de la vérification de l'existence de l'utilisateur." << std::endl;
                m_db->rollback();
            }
        }
        catch (const std::exception &e)
        {
            m_db->rollback();
            std::cerr << "Erreur lors de la suppression de l'utilisateur : " << e.what() << std::endl;
        }
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
            user.setId(result->getInt("id"));

            return user;
        }

        return std::nullopt;
    }

    std::vector<User> getUsers() override
    {
        try
        {
            std::vector<User> users;
            auto result = m_db->executeQuery("SELECT id, fullname, email, phone, password FROM users");

            while (result->next())
            {
                auto fullname = std::make_shared<FullName>(result->getString("fullname"));
                auto email = std::make_shared<Email>(result->getString("email"));
                auto phone = std::make_shared<PhoneNumber>(result->getString("phone"));
                auto password = std::make_shared<Password>(result->getString("password"));

                User user(fullname, email, phone, password);
                user.setId(result->getInt("id"));
                users.push_back(user);
            }

            return users;
        }
        catch (const std::exception &e)
        {
            std::cout << "Erreur: " << e.what() << std::endl;
            return {};
        }
    }

    void update(const std::string &table, const std::vector<std::string> &columns, const std::vector<std::string> &values, const std::string &condition, int id)
    {
        try
        {
            m_db->update(table, columns, values, condition, id);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error updating user: " << e.what() << std::endl;
            throw;
        }
    }

private:
    std::unique_ptr<Adastra::Database> m_db;
};

#endif // USERREPOSITORY_HPP
