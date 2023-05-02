//Selection de la div avec l'id root
const app = document.getElementById('root');
//creation d'une div
const container = document.createElement('div');
//ajout d'une class a la div venant d'etre créé
container.setAttribute('class', 'container');
//ajout dans l'html de la div grace au appendChild
app.appendChild(container);
//debut de la requete
var request = new XMLHttpRequest();
//parametrage de la requete
request.open('GET', 'file.json', true);
//lancement de la fonction au chargement de la page
request.onload = function() {
    //preparation du json
    let data = JSON.parse(this.response);
    //lancement de la requete si la condition est validé
    if (request.status >= 200 && request.status < 400) {
        data.forEach(value => {
            //Creation de la carte qui va avoir les infos
            let card = document.createElement('div');
            card.setAttribute('class', 'card');
            //recuperation du titre
            let title = document.createElement('h2');
            title.setAttribute("id",value.id+"-titre");
            title.textContent = value.title;
            //recuperation de la description
            let description = document.createElement('p');
            description.setAttribute("id",value.id+"-value")
            description.textContent = value.description;
            //ajout dans l'html de la carte et des infos
            container.appendChild(card);
            card.appendChild(title);
            card.appendChild(description);
        })
    }
};
request.send();
//fonction de mise à jour
function refreshData() {
    let request = new XMLHttpRequest();
    request.open("GET", "file.json", false);
    request.onreadystatechange = function() {
        let data = JSON.parse(this.response);
        if (request.status >= 200 && request.status < 400) {
            data.forEach(value => {
                let title = document.getElementById(value.id+"-titre")
                title.textContent = value.title;
                let description = document.getElementById(value.id+"-value")
                description.textContent = value.description;
            })
        }
    };
    request.send();
}
//refresh de la fonction
setInterval(refreshData, 50);
