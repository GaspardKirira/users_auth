#include <iostream>
#include <memory>
#include <Adastra/Database.hpp>
#include <Adastra/Exception.hpp>
#include "User.hpp"

using namespace Adastra;

int main()
{
    try
    {
        // Tentative de création d'un utilisateur avec un mot de passe invalide
        auto invalidPassword = std::make_shared<Password>("Adastra2022@");
        auto validFullName = std::make_shared<FullName>("John Doe");
        auto validEmail = std::make_shared<Email>("john.doe@example.com");
        auto validPhone = std::make_shared<PhoneNumber>("+1234567890");

        // Création d'un utilisateur
        std::shared_ptr<User> user = std::make_shared<User>(validFullName, validEmail, validPhone, invalidPassword);

        auto &db = Database::getInstance("tcp://127.0.0.1:3306", "root", "", "adastra_db");

        // db->create("users", {"fullname", "email", "phone", "password"}, user->getFullName(), user->getEmail(), user->getPhone(), user->getPassword());

        std::unique_ptr<sql::ResultSet> res = db->executeQuery("SELECT id, fullname,email, phone,password FROM users");
        while (res->next())
        {
            std::cout << "ID: " << res->getInt(1) << ", fullname: " << res->getString(2) << ", email: " << res->getString(3) << ", phone: " << res->getString(4) << ", password: " << res->getString(5) << std::endl;
        }

        // db->update("users", {"fullname", "email", "phone", "password"}, {user.getFullName(), user.getEmail(), user.getPhone(), user.getPassword()}, "id = 1");

        // db->remove("users", "id = 1");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }
}