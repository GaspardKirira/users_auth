// Récupérer l'ID de l'utilisateur depuis l'URL
const urlParams = new URLSearchParams(window.location.search);
const userId = urlParams.get('id');

// Vérifier si l'ID est présent
if (!userId) {
    alert("ID utilisateur manquant.");
    window.location.href = "users.html";  // Rediriger vers la page des utilisateurs si l'ID manque
} else {
    // Récupérer les informations de l'utilisateur avant de les modifier
    axios.get(`http://localhost:18080/users/${userId}`)
        .then(function(response) {
            const user = response.data;

            // Si l'utilisateur existe, pré-remplir les champs du formulaire
            if (response.status === 200 && user) {
                document.getElementById('fullname').value = user.fullname;
                document.getElementById('email').value = user.email;
                document.getElementById('phone').value = user.phone;
                document.getElementById('password').value = user.password;
            } else {
                // Si l'utilisateur n'existe pas ou erreur 404
                alert(response.data.message || "Utilisateur non trouvé.");
                window.location.href = "users.html";  // Rediriger si l'utilisateur n'est pas trouvé
            }
        })
        .catch(function(error) {
            console.error("Erreur lors de la récupération des détails de l'utilisateur", error);
            alert("Une erreur est survenue lors de la récupération des informations de l'utilisateur.");
        });

    // Ajouter un événement pour envoyer la mise à jour
    document.getElementById('update-user-form').addEventListener('submit', function(event) {
        event.preventDefault();

        // Récupérer les nouvelles valeurs du formulaire
        const updatedUser = {
            fullname: document.getElementById('fullname').value,
            email: document.getElementById('email').value,
            phone: document.getElementById('phone').value,
            password: document.getElementById('password').value
        };

        // Envoyer la requête PUT pour mettre à jour les informations de l'utilisateur
        axios.put(`http://localhost:18080/users/${userId}`, updatedUser)
            .then(function(response) {
                // Afficher un message venant du backend
                if (response.status === 200) {
                    document.getElementById('response-message').innerHTML = `
                        <p>${response.data.message}</p>
                    `;
                    window.location.href = "users.html";
                } else {
                    // Si une erreur se produit au niveau du backend (ex : 404 ou autre)
                    document.getElementById('response-message').innerHTML = `
                        <p>${response.data.message || "Une erreur s'est produite lors de la mise à jour de l'utilisateur."}</p>
                    `;
                }
            })
            .catch(function(error) {
                // Afficher un message d'erreur du backend (si disponible)
                if (error.response && error.response.data && error.response.data.message) {
                    document.getElementById('response-message').innerHTML = `
                        <p>${error.response.data.message}</p>
                    `;
                } else {
                    document.getElementById('response-message').innerHTML = `
                        <p>Erreur inconnue lors de la mise à jour de l'utilisateur. Veuillez réessayer plus tard.</p>
                    `;
                }
            });
    });
}
