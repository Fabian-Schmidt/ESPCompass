setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var val = parseInt(this.responseText);
            if (!isNaN(val)) {
                bearingNoSpin(val);
                document.getElementById("compass_val").innerText = val;
            }
        }
    };
    xhttp.open("GET", "/compass", true);
    xhttp.send();
}, 250);

var fieldH = "data-heading";
var fieldB = "data-bearing";
function bearingNoSpin(newB) {
    var e = document.getElementById("compass_spin");
    var oldH = parseInt(e.attributes[fieldH]);
    var oldB = parseInt(e.attributes[fieldB]);
    if (isNaN(oldH)) oldH = 0;
    if (isNaN(oldB)) oldB = 0;
    var delta = oldB - newB;
    if (delta > 180) delta = delta - 360;
    if (delta < -180) delta = 360 + delta;

    var newH = oldH + delta;

    e.attributes[fieldH] = newH;
    e.attributes[fieldB] = newB;
    e.style.removeProperty("visibility");
    e.style.transform = "rotate(" + newH + "deg)";
}