function setpath() {
    var default_path = document.getElementById("newfile").files[0].name;
    document.getElementById("filepath").value = default_path;
}

function users() {
	document.getElementById("users").innerHTML += " OK";
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (xhttp.readyState == 4) {
			if (xhttp.status == 200) {
				document.open();
				document.write(xhttp.responseText);
				document.close();
			} else if (xhttp.status == 0) {
				alert("Server closed the connection abruptly!");
			} else {
				alert(xhttp.status + " Error!\n" + xhttp.responseText);
			}
		}
	};
    xhttp.open("POST", "/spiffs/testing_request_to_server.txt", true);
    xhttp.send();
}


function upload() {
    var filePath = document.getElementById("filepath").value;
    var upload_path = "/upload/" + filePath;
    var fileInput = document.getElementById("newfile").files;

    /* Max size of an individual file. Make sure this
     * value is same as that set in file_server.c */
    var MAX_FILE_SIZE = 200*1024;
    var MAX_FILE_SIZE_STR = "200KB";

    if (fileInput.length == 0) {
        alert("No file selected!");
    } else if (filePath.length == 0) {
        alert("File path on server is not set!");
    } else if (filePath.indexOf(' ') >= 0) {
        alert("File path on server cannot have spaces!");
    } else if (filePath[filePath.length-1] == '/') {
        alert("File name not specified after path!");
    } else if (fileInput[0].size > 200*1024) {
        alert("File size must be less than 200KB!");
    } else {
        document.getElementById("newfile").disabled = true;
        document.getElementById("filepath").disabled = true;
        document.getElementById("upload").disabled = true;

        var file = fileInput[0];
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if (xhttp.readyState == 4) {
                if (xhttp.status == 200) {
                    document.open();
                    document.write(xhttp.responseText);
                    document.close();
                } else if (xhttp.status == 0) {
                    alert("Server closed the connection abruptly!");
                    location.reload()
                } else {
                    alert(xhttp.status + " Error!\n" + xhttp.responseText);
                    location.reload()
                }
            }
        };
        xhttp.open("POST", upload_path, true);
        xhttp.send(file);
    }
}
function switchled(autocommand) {
    var switchcommand = document.getElementById("switchcommand").value;
    var upload_path = "/upload/" + "led.txt";

	if (autocommand == 0) {
		switchcommand = 'off';
	} else if (autocommand == 1) {
		switchcommand = 'on';
	}
	if (switchcommand.length == 0) {
        alert("Write command for Switch button!");
    } else if (switchcommand.indexOf(' ') >= 0) {
        alert("Command cannot have spaces!");
	} else {
        document.getElementById("switchcommand").disabled = true;
        document.getElementById("switchled").disabled = true;

        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if (xhttp.readyState == 4) {
                if (xhttp.status == 200) {
                    document.open();
                    document.write(xhttp.responseText);
                    document.close();
                } else if (xhttp.status == 0) {
                    alert("Server closed the connection abruptly!");
                    location.reload()
                } else {
                    alert(xhttp.status + " Error!\n" + xhttp.responseText);
                    location.reload()
                }
            }
        };
        xhttp.open("POST", upload_path, true);
        xhttp.send(switchcommand);
    }
}
function set_data(id_btn, id_hidden){
    if (document.getElementById(id_btn).style.backgroundColor == 'rgb(85, 51, 170)'){
		document.getElementById(id_btn).style="background-color:#FFFFFF; color:#000000";
		document.getElementById(id_hidden).value="n";
	}else{
		document.getElementById(id_btn).style="background-color:#5533AA; color:#FFFFFF";
		document.getElementById(id_hidden).value="y";
	}
}
function check_data(form_Id, start_time_Id, end_time_Id, monday_Id, tuesday_Id, wednesday_Id, thursday_Id, friday_Id,
																								saturday_Id, sunday_Id, holiday_Id){
	if(document.getElementById(start_time_Id).value >= document.getElementById(end_time_Id).value){
		alert("Крайният час трябва да е по-голям от началния час.");
	}else if(document.getElementById(monday_Id).value == 'n' && document.getElementById(tuesday_Id).value == 'n' &&
				document.getElementById(wednesday_Id).value == 'n' && document.getElementById(thursday_Id).value == 'n' &&
					document.getElementById(friday_Id).value == 'n' && document.getElementById(saturday_Id).value == 'n' &&
						document.getElementById(sunday_Id).value == 'n' && document.getElementById(holiday_Id).value == 'n'){
		alert("Задайте ден от седмицата.");
	}else document.getElementById(form_Id).submit();
}
function delete_data(delete_index){
}