#include <iostream>
#include <memory>
#include <Adastra/Database.hpp>
#include <Adastra/Exception.hpp>
#include "crow.h"
#include "UserRepository.hpp"
#include "crow/middlewares/cors.h"

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

void addCorsHeaders(crow::response &res)
{
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
    res.set_header("Access-Control-Allow-Credentials", "true");
}

int main()
{
    crow::App<crow::CORSHandler> app;

    auto &cors = app.get_middleware<crow::CORSHandler>();
    cors
        .global()
        .headers("Content-Type", "Authorization")
        .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method, "OPTIONS"_method)
        .max_age(86400);

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
                        crow::json::wvalue res_json;
                        res_json["message"] = "Invalid JSON format";
                        return crow::response(501, res_json);
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
                            crow::json::wvalue res_json;
                            res_json["message"] = "Utilisateurs récupérés avec succès.";
                            res_json["users"] = std::move(users);
                            return crow::response(200, res_json);
                        }
                        else
                        {
                            crow::json::wvalue res_json;
                            res_json["message"] = "Pas d'utilisateurs";
                            return crow::response(200, res_json);
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
                        res_json["message"] = "Erreur interne du serveur: " + std::string(e.what());
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
                        crow::json::wvalue res_json;
                        res_json["message"] = "Invalid JSON format";
                        return crow::response(501, res_json);
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
                            crow::json::wvalue res_json;
                            res_json["message"] = "ID utilisateur invalide";
                            return crow::response(400, res_json);
                        }

                        auto user = userRepository.getUserById(id);
                        if (user)
                        {
                            userRepository.deleteUser(*user);
                        }
                        else
                        {
                            crow::json::wvalue res_json;
                            res_json["message"] = "Utilisateur non trouvé";
                            return crow::response(404, res_json);
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

        CROW_ROUTE(app, "/uploadfile")
            .methods(crow::HTTPMethod::Post)([](const crow::request &req)
                                             {
          crow::multipart::message_view file_message(req);
          for (const auto& part : file_message.part_map)
          {
              const auto& part_name = part.first;
              const auto& part_value = part.second;
              CROW_LOG_DEBUG << "Part: " << part_name;

              if ("InputFile" == part_name)
              {
                  // Extraire le nom du fichier
                  auto headers_it = part_value.headers.find("Content-Disposition");
                  if (headers_it == part_value.headers.end())
                  {
                      CROW_LOG_ERROR << "No Content-Disposition found";
                      return crow::response(400);
                  }
                  auto params_it = headers_it->second.params.find("filename");
                  if (params_it == headers_it->second.params.end())
                  {
                      CROW_LOG_ERROR << "Part with name \"InputFile\" should have a file";
                      return crow::response(400);
                  }
                  const std::string outfile_name{params_it->second};

                  // Écrire le fichier sur le serveur
                  std::ofstream out_file(outfile_name, std::ios::binary);
                  if (!out_file)
                  {
                      CROW_LOG_ERROR << "Write to file failed\n";
                      continue;
                  }
                  out_file << part_value.body;
                  out_file.close();
                  CROW_LOG_INFO << "Contents written to " << outfile_name << '\n';
              }
              else
              {
                  CROW_LOG_DEBUG << "Value: " << part_value.body << '\n';
              }
          }
          return crow::response(200); });

        app.loglevel(crow::LogLevel::Debug);

        app.port(18080).multithreaded().run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors de l'initialisation de la base de donnees: " << e.what() << std::endl;
    }
}
