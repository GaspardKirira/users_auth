document.getElementById("user-form").addEventListener("submit", async function (event) {
    event.preventDefault();

    // Récupérer les valeurs du formulaire
    const fullname = document.getElementById("fullname").value;
    const email = document.getElementById("email").value;
    const phone = document.getElementById("phone").value;
    const password = document.getElementById("password").value;

    // Préparer les données à envoyer en JSON
    const userData = {
        fullname: fullname,
        email: email,
        phone: phone,
        password: password
    };

    try {
        // Faire la requête POST avec Fetch API
        const response = await fetch("http://localhost:18080/users", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify(userData)
        });

        const responseData = await response.json();
        const messageDiv = document.getElementById("response-message");

        if (response.ok) {
            // Succès : afficher un message positif
            messageDiv.textContent = responseData.message;
            messageDiv.classList.add("success");
        } else {
            // Échec : afficher un message d'erreur
            messageDiv.textContent = responseData.message || "Une erreur s'est produite.";
            messageDiv.classList.remove("success");
        }

        // Afficher le message
        messageDiv.style.display = "block";
    } catch (error) {
        const messageDiv = document.getElementById("response-message");
        messageDiv.textContent = "Erreur de connexion au serveur.";
        messageDiv.classList.remove("success");
        messageDiv.style.display = "block";
    }
});
