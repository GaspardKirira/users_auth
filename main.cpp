#include <iostream>
#include <type_traits>

// -----------------------------------------------------------
// 1 - Fonction template constexpr
// -----------------------------------------------------------
// Fonction générique pour calculer la puissance à la compilation.
template <typename T>
constexpr T puissance(T base, int exposant)
{
    T resultat = 1;
    for (int i = 0; i < exposant; ++i)
    {
        resultat *= base; // Multiplication répétée (exponentiation naïve)
    }
    return resultat; // Renvoie le résultat (évalué à la compilation si possible)
}

// -----------------------------------------------------------
// 2 - Static assert pour vérifier des contraintes à la compilation
// -----------------------------------------------------------
// Vérification qu'une fonction ne peut être appelée qu'avec un type entier
template <typename T>
constexpr void verifierType()
{
    static_assert(std::is_integral<T>::value, "Cette fonction ne supporte que les entiers !");
}

// -----------------------------------------------------------
// 3 - Exemple d'une constante compile-time avec constexpr
// -----------------------------------------------------------
constexpr int TAILLE_TABLEAU = 5;

// -----------------------------------------------------------
// 4 - Fonction qui utilise le constexpr et static_assert ensemble
// -----------------------------------------------------------
template <typename T>
constexpr T sommeTableau(const T (&tableau)[TAILLE_TABLEAU])
{
    verifierType<T>(); // Vérifie qu'on est bien sur un type entier à la compilation

    T somme = 0;
    for (int i = 0; i < TAILLE_TABLEAU; ++i)
    {
        somme += tableau[i]; // Somme des éléments (peut être évaluée à la compilation si les données sont connues)
    }
    return somme;
}

// -----------------------------------------------------------
// 5 - Exemple d'utilisation complète
// -----------------------------------------------------------
int main()
{
    constexpr int resultat = puissance(2, 8); // Calculé à la compilation
    static_assert(resultat == 256, "Le calcul de la puissance est faux !");

    constexpr int tableau[TAILLE_TABLEAU] = {1, 2, 3, 4, 5}; // Tableau connu à la compilation
    constexpr int somme = sommeTableau(tableau);             // Évalué à la compilation

    std::cout << "2^8 = " << resultat << "\n";
    std::cout << "Somme du tableau = " << somme << "\n";

    return 0;
}
