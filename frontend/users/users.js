// Fonction pour charger les utilisateurs au chargement de la page
document.addEventListener('DOMContentLoaded', function() {
    fetchUsers();
});

function fetchUsers() {
    axios.get('http://localhost:18080/users')
        .then(function(response) {
            const users = response.data.users;
            const userListDiv = document.getElementById('userList');
            userListDiv.innerHTML = ''; // Réinitialiser la liste avant d'afficher

            if (users && users.length > 0) {
                users.forEach(function(user) {
                    const userCard = document.createElement('div');
                    userCard.classList.add('userCard');

                    const userId = user.id || "N/A";
                    const userFullname = user.fullname || "N/A";
                    const userEmail = user.email || "N/A";
                    const userPhone = user.phone || "N/A";
                    const userPassword = user.password || "N/A";

                    // Créer le lien vers la page de détails de l'utilisateur
                    const userDetailLink = `<a href="user_details.html?id=${userId}">Voir les détails</a>`;

                    // Ajouter les données de l'utilisateur et le lien à la carte
                    userCard.innerHTML = `
                        <h3>ID: ${userId}</h3>
                        <p><strong>Nom complet:</strong> ${userFullname}</p>
                        <p><strong>Email:</strong> ${userEmail}</p>
                        <p><strong>Téléphone:</strong> ${userPhone}</p>
                        <p><strong>Mot de passe:</strong> ${userPassword}</p>
                        ${userDetailLink}  <!-- Ajouter le lien ici -->
                    `;

                    // Ajouter la carte à la liste des utilisateurs
                    userListDiv.appendChild(userCard);
                });
            } else {
                userListDiv.innerHTML = '<p>Aucun utilisateur trouvé.</p>';
            }
        })
        .catch(function(error) {
            console.error("Erreur lors de la récupération des utilisateurs", error);
        });
}
