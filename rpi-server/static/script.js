let up_slider = document.getElementById("up_slider");
let down_slider = document.getElementById("down_slider");

let up_out = document.getElementById("up_out");
let down_out = document.getElementById("down_out");

up_out.innerHTML = up_slider.value;
down_out.innerHTML = down_slider.value;

up_slider.oninput = function() {
    up_out.innerHTML = this.value;
}

down_slider.oninput = function() {
    down_out.innerHTML = this.value;
}