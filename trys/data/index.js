function checkSvet0() {
    var towrite = document.getElementById("data");

    fetch(`/svet0`)
        .then((response) => {
            return response.json();
        })
        .then((data) => {
            console.log(data);
            towrite.innerHTML = data;
        });
}

setInterval(checkSvet0, 100);