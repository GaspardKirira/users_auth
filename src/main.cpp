#include "Email.hpp"
#include "PhoneNumber.hpp"
#include <iostream>

int main()
{
    try
    {
        Email e("gaspard@gmail.com");

        std::string validEmail = e.getEmail();

        std::cout << "L'email valide est : " << validEmail << std::endl;

        PhoneNumber p("+243991324430");

        std::string phone = p.getPhoneNumber();
        std::cout << "Le numero de telephone valide est : " << phone << std::endl;
    }
    catch (const BaseException &e)
    {
        std::cout << "Erreur: " << e.what() << "( Code: " << static_cast<int>(e.code()) << " )\n";
    }

    return 0;
}
