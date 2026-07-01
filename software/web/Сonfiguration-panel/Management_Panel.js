let number;
let Therm;
let Excerpt;
let Summ;

// аякс по клику на кнопку ".switch-btn"
$(document).ready(function () {
  let X = 0;
  // обработка кликов по свитчерам и кнопкам:
  $(".switch-btn,.gsm_btn").on("click", function (event) {
    let id = event.target.id;    
    if (id.indexOf('Security_toggle_')>-1 || ('Security_state')>-1) {
      $.ajax({
        url: "Security_Settings_Panel.php",
        type: "POST",
        data: { id: id },
        success: function (data) {},
      });
    }
    else {
      $.ajax({
        url: "Management_Panel.php",
        type: "POST",
        data: { id: id },
        success: function (data) {},
      });
    };
  });

  // вывод звукового клика по кнопкам:
  $(".toggle,.hdv,.switch-btn,.gsm_btn").click(function () {
    new Audio("Click.wav").play();
  });

  // обработка таймер-степперов
  $(".button_stepper").on("click", function (event) {
    let button = event.target.id;
    Num = button.replace(/[a-z_]/gi, "");
    let input_stepper = "timer_" + Num;
    let stepper = document.getElementById(input_stepper).value;
    // alert(stepper);
    let time;
    if (Number(Num) != 7) {  
      let stepper_hours = stepper.substring(0, 2);
      var hours_to_sec = (Number(stepper_hours))*3600;
      let stepper_minutes = stepper.substring(3, 5);
      var minutes_to_sec = (Number(stepper_minutes))*60;
      let stepper_seconds = stepper.substring(6);
      var seconds = Number(stepper_seconds);
      var summ = hours_to_sec + minutes_to_sec + seconds;
      let stepper_value = String(summ);
      if ((Number(Num))<10) {Num = "0"+ Num};
      time = "excerpt_" + Num + "_" + stepper_value;
    }
    else {
      if ((Number(Num))<10) {Num = "0"+ Num};
      time = "excerpt_" + Num + "_" + stepper
    };
    $.ajax({
      url: "Management_Panel.php",
      type: "POST",
      data: { id: time },
      success: function (data) {},
    });
  });
  
  // обработка термо-степперов
  $(".thermo_stepper").on("click", function (event) {
    let button = event.target.id;
    Num = button.replace(/[a-z_]/gi, "");
    let input_stepper = "Therm_" + Num;
    let stepper = document.getElementById(input_stepper).value;
    let Val = Number(stepper);
    let res = Val.toFixed(1);
    let Arg = input_stepper + "_" + res;
    $.ajax({
      url: "Management_Panel.php",
      type: "POST",
      data: { id: Arg },
      success: function (data) {},
    });
  });

});


// обновляем открытую веб-страничку:
function fetchdata() {
  //для свитчеров:
  $.ajax({
    url: "Management_Responce.php",
    type: "POST",
    success: function (data) {
      let str = data
        .replace(/"id":/g, "")
        .replace(/,"state"/g, "")
        .replace(/}{/g, ",");
      const obj = JSON.parse(str);
      for (key in obj) {
        let arg = "Manag_toggle_" + key;
        let myElement = document.getElementById(arg);
        if (obj[key] == "on") {
          myElement.classList.add("switch-on");
        } else {
          myElement.classList.remove("switch-on");
        }
      };
    },
  });

  $.ajax({
    url: "Blackout_Responce.php",
    type: "POST",
    success: function (data) {
      let str = data
        .replace(/"id":/g, "")
        .replace(/,"state"/g, "")
        .replace(/}{/g, ",");
      const obj = JSON.parse(str);
      for (key in obj) {
        let arg = "Blackout_toggle_" + key;
        let myElement = document.getElementById(arg);
        if (obj[key] == "on") {
          myElement.classList.add("switch-on");
        } else {
          myElement.classList.remove("switch-on");
        }
      };
    },
  });

  $.ajax({
    url: "Security_Settings_Responce.php",
    type: "POST",
    success: function (data) {
      let str = data
        .replace(/"id":/g, "")
        .replace(/,"state"/g, "")
        .replace(/}{/g, ",");
      const obj = JSON.parse(str);
      // alert(data);
      for (key in obj) {
        if (key != 12) {         
          let arg = "Security_toggle_" + key;
          let myElement = document.getElementById(arg);        
          if (key!=3 && key!=4 && key!=11) {
            if (obj[key] == "on") {
              myElement.classList.add("switch-on");
            } 
            else {
              myElement.classList.remove("switch-on");
            }
          }
          if (key==3 || key==4) {
            if (obj[key] == "on") {
              myElement.textContent = 'locked';
            } 
            else {
              myElement.textContent = 'open';
            }
          }
          if (key==11) {
            if (obj[key] == "on") {
              myElement.textContent = 'open';
            } 
            else {
              myElement.textContent = 'closed';
            }
          }
        };
      }
    },
  });

  //для показаний энерго-аудита:
  $.ajax({
    url: "Power_Responce.php",
    type: "POST",
    success: function (data) {
      let str = data
        .replace(/"id":/g, "")
        .replace(/,"value"/g, "")
        .replace(/}{/g, ",");
      // alert(str);
      const obj = JSON.parse(str);
      for (key in obj) {
        if (key < 10 || key == 18 || key == 20 || key == 21 || key == 24) {
          let myElement = document.getElementById(key);
          myElement.textContent = obj[key];
        }
        if (key > 9 && key < 18) {
          let index = key - 9;
          let arg = "value_" + String(index);
          // let ind = String(arg);
          let myElement = document.getElementById(arg);
          myElement.textContent = obj[key];
        }
        if (key == 22) {
          let myElement = document.getElementById(key);
          // myElement.textContent = obj[key];
          if (obj[key] == "0") {
            myElement.textContent = '❌';
          } 
          else {
            myElement.textContent = '✔';
          }
        }
        if (key == 23) {
          let myElement = document.getElementById(key);
          // myElement.textContent = obj[key];
          if (obj[key] == "0") {
            myElement.textContent = '✔';
          } 
          else {
            myElement.textContent = '❌';
          }
        }
      }  
    },
  });
  const Temp = document.getElementById("6").textContent;
  document.getElementById("TempIN_1").textContent = Temp;
  document.getElementById("TempIN_2").textContent = Temp;

  //для раздела с таймингом:
  $.ajax({
    url: "Timing_Responce.php",
    type: "POST",
    success: function (data) {
      let Str = data
        .replace(/"id":/g, "")
        .replace(/,"value"/g, "")
        .replace(/}{/g, ",");
      // alert(Str);
      const obj = JSON.parse(Str);
      for (key in obj) {
        let arg_msec = "msec_excerpt_" + String(key);
        let arg_sec = "sec_excerpt_" + String(key);
        let arg_min = "min_excerpt_" + String(key);
        let arg_hr = "hr_excerpt_" + String(key);
        let Value_hr;
        let Value_min;
        let Value_sec;
        let Value_msec;
        if (key != 7) {
        if (Number(obj[key])<601) {
          var value_min = Math.floor((Number(obj[key]))/60) 
          var value_sec = Number(obj[key]) - (value_min*60)
          if (value_min<10) {Value_min = "0" + String(value_min)}
          else {Value_min = String(value_min)}
          if (value_sec<10) {Value_sec = "0" + String(value_sec)}
          else {Value_sec = String(value_sec)}
          document.getElementById(arg_sec).textContent = Value_sec
          document.getElementById(arg_min).textContent = Value_min
        };
        if (Number(obj[key])>600) {
          var value_hr = Math.floor((Number(obj[key]))/3600);
          var value_min = Math.floor((Number(obj[key]) - (value_hr*3600))/60);
          if (value_hr<10) {Value_hr = "0" + String(value_hr)}
          else {Value_hr = String(value_hr)}
          if (value_min<10) { Value_min = "0" + String(value_min)}
          else {Value_min = String(value_min)}
          document.getElementById(arg_min).textContent = Value_min;
          document.getElementById(arg_hr).textContent = Value_hr;
        };
        }
        else {document.getElementById(arg_msec).textContent = obj[key]}        
      }
    },
  });

  //обработка USSD-запросов GSM модема:
  $.ajax({
    url: "USSD_Responce.php",
    type: "POST",
    success: function (data) {
      let str = data
        .replace(/"id":/g, "")
        .replace(/,"value"/g, "")
        .replace(/}{/g, ",");
      // alert(str);
      const obj = JSON.parse(str);
      for (key in obj) {
        let arg = "gsm_" + key;
        let myElement = document.getElementById(arg);
        myElement.textContent = obj[key];
      };
    }
  });
}

$(document).ready(function () {
  setInterval(fetchdata, 1000);
});
