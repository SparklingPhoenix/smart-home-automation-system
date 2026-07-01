let panel_type = 0;
$(document).ready(function () {
  document.getElementById("defaultOpen").click();
  $(".tablinks, .dropbtn, .btn, .button").click(() =>
    new Audio("Click.wav").play()
  );
  $("body").on("click", function (event) {
    let id = event.target.id;
    var myElement = document.getElementById(id);
    let value_id = id.replace(/[^\d]/g, "");
    let type_btn = id.replace(/[\d_]/g, "");
    /* Dropdown function */
    if (type_btn.includes("dropbt")) {
      let color_off_val = "color_off_" + value_id;
      let color_on_val = "color_on_" + value_id;
      if (myElement.classList.contains(color_off_val)) {
        myElement.classList.replace(color_off_val, color_on_val);
      } else {
        myElement.classList.replace(color_on_val, color_off_val);
      }
      let myDropdown_val = "myDropdown_" + value_id;
      var myDropdownElement = document.getElementById(myDropdown_val);
      myDropdownElement.classList.toggle("show");
    }
    /* Btn function */
    if (type_btn.includes("btn")) {
      $.ajax({
        url: "Switch_Panel.php",
        type: "POST",
        data: { id: id },
        success: function (data) {},
      });
    }
    /* Button function */
    if (type_btn.includes("button")) {
      $.ajax({
        url: "Switch_Panel.php",
        type: "POST",
        data: { id: id },
        success: function (data) {},
      });
    }
  });
});

/* Запит стану освітлення */
function fetchdata_manual() {
  /*Manual_Switch_Panel  */
  if (panel_type == 2) {
    for (let n = 1; n < 8; n++) {
      let myDropdownElement = "myDropdown_" + n;
      var Element = document.getElementById(myDropdownElement);
      if (Element.classList.contains("show")) {
        $.ajax({
          url: "Manual_Switch_Panel_Responce.php",
          type: "POST",
          data: { id: n },
          success: function (data) {
            let str = data
              .replace(/"number":/g, "")
              .replace(/,"state"/g, "")
              .replace(/}{/g, ",");
            const obj_msp = JSON.parse(str);
            for (key in obj_msp) {
              let btn_on = "color_on_" + n;
              let btn_off = "color_off_" + n;
              const myBtnElement = document.getElementById(key);
              if (obj_msp[key] == "on") {
                myBtnElement.classList.replace(btn_off, btn_on);
              }
              if (obj_msp[key] == "off") {
                myBtnElement.classList.replace(btn_on, btn_off);
              }
            }
          },
        });
      }
    }
  }
}

function fetchdata_universal() {
  /*Universal_Switch_Panel*/
  if (panel_type == 1) {
    $.ajax({
      url: "Universal_Switch_Panel_Responce.php",
      type: "POST",
      success: function (data) {
        let str = data
          .replace(/"id":/g, "")
          .replace(/,"state"/g, "")
          .replace(/}{/g, ",");
        const obj = JSON.parse(str);
        for (key in obj) {
          let arg = "button_" + key;
          let state_class_on = "color_" + key + "_on";
          let state_class_off = "color_" + key + "_off";
          const myElement = document.getElementById(arg);
          if (obj[key] == "on") {
            myElement.classList.replace(state_class_off, state_class_on);
            myElement.classList.replace("transition_off", "transition_on");
          }
          if (obj[key] == "off") {
            myElement.classList.replace(state_class_on, state_class_off);
            myElement.classList.replace("transition_on", "transition_off");
          }
        }
      },
    });
  }
}

$(document).ready(function () {
  setInterval(fetchdata_manual, 1000);
  setInterval(fetchdata_universal, 1000);
});

/*Head_switcher*/
function openCity(evt, Panel_Name) {
  var i, tabcontent, tablinks;
  
  tabcontent = document.getElementsByClassName("tabcontent");
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = "none";
  }

  tablinks = document.getElementsByClassName("tablinks");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace("active", "");
  }

  document.getElementById(Panel_Name).style.display = "block";
  evt.currentTarget.className += "active";
  if (Panel_Name == "Universal") {
    panel_type = 1;
  }
  if (Panel_Name == "Manual") {
    panel_type = 2;
  }
}

