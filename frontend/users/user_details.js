// Récupérer l'ID de l'utilisateur depuis l'URL
const urlParams = new URLSearchParams(window.location.search);
const userId = urlParams.get('id');

// Vérifier si l'ID est présent
if (userId) {
    // Faire une requête GET pour récupérer les détails de l'utilisateur
    axios.get(`http://localhost:18080/users/${userId}`)
        .then(function(response) {
            const user = response.data;  // La réponse du serveur avec les données ou le message

            const userDetailsDiv = document.getElementById('userDetails');

            if (response.status === 200 && user) {
                // Afficher les informations de l'utilisateur
                userDetailsDiv.innerHTML = `
                    <h3>${user.fullname}</h3>
                    <p><strong>Email:</strong> ${user.email}</p>
                    <p><strong>Téléphone:</strong> ${user.phone}</p>
                    <p><strong>Mot de passe:</strong> ${user.password}</p>
                `;

                // Ajouter les liens "Mettre à jour" et "Supprimer"
                userDetailsDiv.innerHTML += `
                    <div>
                        <a href="update_user.html?id=${userId}">Mettre à jour les informations</a>
                        <br>
                        <button id="deleteUserBtn">Supprimer cet utilisateur</button>
                    </div>
                `;

                // Ajouter l'événement pour la suppression de l'utilisateur
                document.getElementById('deleteUserBtn').addEventListener('click', function() {
                    const confirmDelete = confirm("Êtes-vous sûr de vouloir supprimer cet utilisateur ?");
                    if (confirmDelete) {
                        axios.delete(`http://localhost:18080/users/${userId}`)
                            .then(function(deleteResponse) {
                                alert(deleteResponse.data.message || "Utilisateur supprimé avec succès");
                                window.location.href = "users.html";  
                            })
                            .catch(function(deleteError) {
                                const errorMessage = deleteError.response ? deleteError.response.data.message : "Erreur inconnue.";
                                alert(errorMessage);
                            });
                    }
                });

            } else {
                // Si les données de l'utilisateur sont absentes ou erreur 404
                userDetailsDiv.innerHTML = `<p>${user.message || "Utilisateur non trouvé."}</p>`;
            }
        })
        .catch(function(error) {
            // Gestion des erreurs (problèmes réseau, erreurs 500, etc.)
            console.error("Erreur lors de la récupération des détails de l'utilisateur", error);
            document.getElementById('userDetails').innerHTML = '<p>Une erreur est survenue lors de la récupération des détails de l\'utilisateur. Veuillez réessayer plus tard.</p>';
        });
} else {
    // Si l'ID de l'utilisateur est manquant dans l'URL
    alert("ID utilisateur manquant.");
}
