#include <iostream>
#include <memory>
#include <Adastra/Database.hpp>
#include <Adastra/Exception.hpp>
#include "crow.h"
#include "UserRepository.hpp"

bool validateUserJson(const crow::request &req)
{
    auto json_data = crow::json::load(req.body);

    if (!json_data.has("fullname") || !json_data.has("email") ||
        !json_data.has("phone") || !json_data.has("password"))
    {
        return false;
    }
    return true;
}

int main()
{
    crow::SimpleApp app;

    try
    {
        auto &db = Adastra::Database::getInstance("tcp://127.0.0.1:3306", "root", "", "adastra_db");
        UserRepository userRepository(db);

        // Route pour créer un utilisateur (POST /users)
        CROW_ROUTE(app, "/users")
            .methods(crow::HTTPMethod::POST)(
                [&userRepository](const crow::request &req)
                {
                    auto json_data = crow::json::load(req.body);
                    if (!json_data)
                    {
                        crow::json::wvalue res;
                        res["message"] = "Invalid JSON format";
                        return crow::response(501, res);
                    }

                    if (!validateUserJson(req))
                    {
                        crow::json::wvalue res_json;
                        res_json["message"] = "Données JSON invalides";
                        return crow::response(400, res_json);
                    }

                    try
                    {
                        // Créer l'utilisateur à partir des données JSON
                        auto fullname = std::make_shared<FullName>(json_data["fullname"].s());
                        auto email = std::make_shared<Email>(json_data["email"].s());
                        auto phone = std::make_shared<PhoneNumber>(json_data["phone"].s());
                        auto password = std::make_shared<Password>(json_data["password"].s());

                        User user(fullname, email, phone, password);

                        // Sauvegarder l'utilisateur dans la base de données
                        userRepository.save(user);

                        crow::json::wvalue res_json;
                        res_json["message"] = "Utilisateur créé avec succès";
                        return crow::response(201, res_json);
                    }
                    catch (const std::exception &e)
                    {
                        crow::json::wvalue res_json;
                        res_json["message"] = e.what();
                        return crow::response(400, res_json);
                    }
                });

        // Route pour récupérer tous les utilisateurs (GET /users)
        CROW_ROUTE(app, "/users")
            .methods(crow::HTTPMethod::GET)(
                [&userRepository](const crow::request &req)
                {
                    try
                    {
                        auto users = userRepository.getUsers();
                        if (!users.empty())
                        {
                            std::vector<crow::json::wvalue> res_json;
                            for (const auto &user : users)
                            {
                                crow::json::wvalue user_json;
                                user_json["fullname"] = user.getFullName();
                                user_json["email"] = user.getEmail();
                                user_json["phone"] = user.getPhone();
                                std::cout << "Fullname: " << user.getFullName() << std::endl;
                                std::cout << "Email: " << user.getEmail() << std::endl;
                                std::cout << "Phone: " << user.getPhone() << std::endl;
                                std::cout << "Password: " << user.getPassword() << std::endl;
                                res_json.emplace_back(std::move(user_json));
                            }
                            crow::json::wvalue res;
                            res["users"] = std::move(res_json);
                            return crow::response(200, res);
                        }
                        else
                        {
                            crow::json::wvalue res;
                            res["message"] = "Pas d'utilisateurs";
                            return crow::response(200, res);
                        }
                    }
                    catch (const std::exception &e)
                    {
                        crow::json::wvalue res_json;
                        res_json["message"] = std::string(e.what());
                        return crow::response(400, res_json);
                    }
                });

        // Route pour récupérer un utilisateur par ID (GET /users/{id})
        CROW_ROUTE(app, "/users/<int>")
            .methods(crow::HTTPMethod::GET)(
                [&userRepository](const crow::request &req, int id)
                {
                    try
                    {
                        auto user_opt = userRepository.getUserById(id);

                        if (user_opt)
                        {
                            const auto &user = *user_opt;

                            crow::json::wvalue res_json;
                            res_json["fullname"] = user.getFullName();
                            res_json["email"] = user.getEmail();
                            res_json["phone"] = user.getPhone();

                            return crow::response(200, res_json);
                        }
                        else
                        {
                            crow::json::wvalue res_json;
                            res_json["message"] = "Utilisateur non trouvé";
                            return crow::response(404, res_json);
                        }
                    }
                    catch (const std::exception &e)
                    {
                        crow::json::wvalue res_json;
                        res_json["message"] = std::string(e.what());
                        return crow::response(400, res_json);
                    }
                });

        // Route pour mettre à jour un utilisateur (PUT /users/{id})
        CROW_ROUTE(app, "/users/<int>")
            .methods(crow::HTTPMethod::PUT)(
                [&userRepository](const crow::request &req, int id)
                {
                    if (!validateUserJson(req))
                    {
                        crow::json::wvalue res_json;
                        res_json["message"] = "Données JSON invalides";
                        return crow::response(400, res_json);
                    }

                    auto json_data = crow::json::load(req.body);

                    try
                    {
                        // Mise à jour de l'utilisateur
                        auto fullname = std::make_shared<FullName>(json_data["fullname"].s());
                        auto email = std::make_shared<Email>(json_data["email"].s());
                        auto phone = std::make_shared<PhoneNumber>(json_data["phone"].s());
                        auto password = std::make_shared<Password>(json_data["password"].s());

                        User user(fullname, email, phone, password);

                        userRepository.update(id, user); // Mise à jour de l'utilisateur

                        crow::json::wvalue res_json;
                        res_json["message"] = "Utilisateur mis à jour avec succès";
                        return crow::response(200, res_json);
                    }
                    catch (const std::exception &e)
                    {
                        crow::json::wvalue res_json;
                        res_json["message"] = e.what();
                        return crow::response(400, res_json);
                    }
                });

        // Route pour supprimer un utilisateur (DELETE /users/{id})
        CROW_ROUTE(app, "/users/<int>")
            .methods(crow::HTTPMethod::DELETE)(
                [&userRepository](const crow::request &req, int id)
                {
                    try
                    {
                        auto user = userRepository.getUserById(id);
                        if (user)
                        {
                            userRepository.deleteUser(*user);
                        }
                        else
                        {
                            crow::json::wvalue res;
                            res["message"] = "User not found";
                            return crow::response(400, res);
                        }

                        crow::json::wvalue res_json;
                        res_json["message"] = "Utilisateur supprimé avec succès";
                        return crow::response(200, res_json);
                    }
                    catch (const std::exception &e)
                    {
                        crow::json::wvalue res_json;
                        res_json["message"] = e.what();
                        return crow::response(400, res_json);
                    }
                });
        app.port(18080).multithreaded().run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors de l'initialisation de la base de donnees: " << e.what() << std::endl;
    }
}

// #include <iostream>
// #include <memory>
// #include <Adastra/Database.hpp>
// #include <Adastra/Exception.hpp>
// #include "User.hpp"

// using namespace Adastra;

// int main()
// {
//     try
//     {
//         // Tentative de création d'un utilisateur avec un mot de passe invalide
//         auto invalidPassword = std::make_shared<Password>("Adastra2022@");
//         auto validFullName = std::make_shared<FullName>("John Doe");
//         auto validEmail = std::make_shared<Email>("john.doe@example.com");
//         auto validPhone = std::make_shared<PhoneNumber>("+1234567890");

//         // Création d'un utilisateur
//         std::shared_ptr<User> user = std::make_shared<User>(validFullName, validEmail, validPhone, invalidPassword);

//         auto &db = Database::getInstance("tcp://127.0.0.1:3306", "root", "", "adastra_db");

//         // db->create("users", {"fullname", "email", "phone", "password"}, user->getFullName(), user->getEmail(), user->getPhone(), user->getPassword());

//         std::unique_ptr<sql::ResultSet> res = db->executeQuery("SELECT id, fullname,email, phone,password FROM users");
//         while (res->next())
//         {
//             std::cout << "ID: " << res->getInt(1) << ", fullname: " << res->getString(2) << ", email: " << res->getString(3) << ", phone: " << res->getString(4) << ", password: " << res->getString(5) << std::endl;
//         }

//         // db->update("users", {"fullname", "email", "phone", "password"}, {user.getFullName(), user.getEmail(), user.getPhone(), user.getPassword()}, "id = 1");

//         // db->remove("users", "id = 1");
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Erreur : " << e.what() << std::endl;
//         return 1;
//     }
// }