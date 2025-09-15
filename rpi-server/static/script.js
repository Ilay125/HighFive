let up_slider = document.getElementById("up_slider");
let down_slider = document.getElementById("down_slider");

let up_out = document.getElementById("up_out");
let down_out = document.getElementById("down_out");

let sleep_btn = document.getElementById("sleep_btn");

up_out.innerHTML = up_slider.value;
down_out.innerHTML = down_slider.value;

up_slider.oninput = function() {
    up_out.innerHTML = this.value;
}

down_slider.oninput = function() {
    down_out.innerHTML = this.value;
}


sleep_btn.onclick = function() {
    console.log("sleep pressed");

    up_slider.value = 15;
    down_slider.value = 15;

    up_out.innerHTML = up_slider.value;
    down_out.innerHTML = down_slider.value;
    
    document.forms[0].submit()
}
