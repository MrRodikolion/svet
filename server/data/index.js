// function checkSvet0() {
//     var towrite = document.getElementById("data");

//     fetch(`/svet0`)
//         .then((response) => {
//             return response.json();
//         })
//         .then((data) => {
//             console.log(data);
//             towrite.innerHTML = data.clr;
//         });
// }

// setInterval(checkSvet0, 100);

function start() {
    interval = setInterval(updateTime, 1);
    miliseconds = tt;

    fetch('/start')
}
function startnotimer() {
    fetch('/start')
}

function stop() {
    clearInterval(interval);

    fetch('/stop')
}

function restart() {
    fetch('/stop')
    fetch('/start')
}

let timer;
const tt = 1000 * 60 * 2;
let miliseconds = tt;
let interval;

function updateTime() {
    timer = document.getElementById('timer');
    miliseconds--;
    if (miliseconds <= 0) {
        stop();
    }
    let milsecs = miliseconds % 1000;
    let secs = ((miliseconds - milsecs) / 1000) % 60;
    let mins = (((miliseconds - milsecs) / 1000) - secs) / 60;
    timer.textContent = `${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}:${milsecs.toString().padStart(4, '0')}`;
}