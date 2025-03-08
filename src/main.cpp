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
                        auto fullname = std::make_shared<FullName>(json_data["fullname"].s());
                        auto email = std::make_shared<Email>(json_data["email"].s());
                        auto phone = std::make_shared<PhoneNumber>(json_data["phone"].s());
                        auto password = std::make_shared<Password>(json_data["password"].s());

                        User user(fullname, email, phone, password);
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

        CROW_ROUTE(app, "/users")
            .methods(crow::HTTPMethod::GET)(
                [&userRepository](const crow::request &req)
                {
                    try
                    {
                        auto result = userRepository.getUsers();
                        if (!result.empty())
                        {
                            std::vector<crow::json::wvalue> users;
                            users.reserve(100);
                            for (const auto &user : result)
                            {
                                crow::json::wvalue user_json;

                                int id = user.getId();
                                std::string fullname = user.getFullName();
                                std::string email = user.getEmail();
                                std::string phone = user.getPhone();
                                std::string password = user.getPassword();

                                user_json["id"] = (id == 0) ? "N/A" : std::to_string(id);
                                user_json["fullname"] = fullname.empty() ? "N/A" : fullname;
                                user_json["email"] = email.empty() ? "N/A" : email;
                                user_json["phone"] = phone.empty() ? "N/A" : phone;
                                user_json["password"] = password.empty() ? "N/A" : password;

                                users.emplace_back(std::move(user_json));
                            }
                            crow::json::wvalue res;
                            res["users"] = std::move(users);
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
                            res_json["password"] = user.getPassword();

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

        CROW_ROUTE(app, "/users/<int>")
            .methods(crow::HTTPMethod::PUT)(
                [&userRepository](const crow::request &req, int id)
                {
                    auto json_data = crow::json::load(req.body);

                    if (!json_data)
                    {
                        crow::json::wvalue res;
                        res["message"] = "Invalid JSON format";
                        return crow::response(501, res);
                    }

                    try
                    {
                        auto user = userRepository.getUserById(id);
                        if (!user)
                        {
                            crow::json::wvalue res_json;
                            res_json["message"] = "Utilisateur non trouvé";
                            return crow::response(404, res_json);
                        }

                        std::vector<std::string> columns;
                        std::vector<std::string> values;

                        if (json_data.has("fullname"))
                        {
                            user->setFullName(json_data["fullname"].s());
                            columns.push_back("fullname");
                            values.push_back(json_data["fullname"].s());
                        }
                        if (json_data.has("email"))
                        {
                            user->setEmail(json_data["email"].s());
                            columns.push_back("email");
                            values.push_back(json_data["email"].s());
                        }
                        if (json_data.has("phone"))
                        {
                            user->setPhone(json_data["phone"].s());
                            columns.push_back("phone");
                            values.push_back(json_data["phone"].s());
                        }
                        if (json_data.has("password"))
                        {
                            user->setPassword(json_data["password"].s());
                            columns.push_back("password");
                            values.push_back(json_data["password"].s());
                        }

                        userRepository.update("users", columns, values, "id = ?", id);

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

        CROW_ROUTE(app, "/users/<int>")
            .methods(crow::HTTPMethod::DELETE)(
                [&userRepository](const crow::request &req, int id)
                {
                    try
                    {
                        if (id <= 0)
                        {
                            crow::json::wvalue res;
                            res["message"] = "ID utilisateur invalide";
                            return crow::response(400, res);
                        }

                        auto user = userRepository.getUserById(id);
                        if (user)
                        {
                            userRepository.deleteUser(*user);
                        }
                        else
                        {
                            crow::json::wvalue res;
                            res["message"] = "Utilisateur non trouvé";
                            return crow::response(404, res);
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
