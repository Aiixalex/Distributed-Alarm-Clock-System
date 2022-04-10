"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
var num_alarms = 0;
var counter_id = 0;
const weekday = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"]
function handleError(error_box, msg){
    console.log('#error-text-'+ error_box)
    $('#error-text-'+ error_box).html(msg);
    $("#error-box-"+ error_box).show();
    var unhidetimer = setTimeout(function(){
        $('#error-box-'+error_box).hide();
    }, 2000);
};

$(document).ready(function() {
    $('#error-box-alarms').css('display', 'block');
    $('#error-box-alarms').hide();
    // Get uptime of BBG
    update_list("2:23:03:0:2:04:3"); // FOR Testing
    socket.emit("message","update");

    const uptimeInterval = window.setInterval(function() {
        update_clock();
	$('#hour-up').click(function(){
        var time = get_time('#alarm-text');
        var hour = parseInt(time[0]);
        var min = time[1];
        var suffix = time[2];
        hour = hour + 1;
        if(hour >= 13){
            hour = 1;
        }
        $('#alarm-text').html(hour.toString().concat(":",min,suffix));
	});
    $('#hour-dn').click(function(){
        var time = get_time('#alarm-text');
        var hour = parseInt(time[0]);
        var min = time[1];
        var suffix = time[2];
        hour = hour - 1;
        if(hour <= 0){
            hour = 12;
        }
        $('#alarm-text').html(hour.toString().concat(":",min,suffix));
	});
    $('#min-up').click(function(){
        var time = get_time('#alarm-text');
        var hour = time[0];
        var min = parseInt(time[1]);
        var suffix = time[2];
        min = min + 1;
        if(min >= 60){
            min = 0;
        }
        var min_str = min.toString();
        if(min < 10){
            min_str = "0" + min_str
        }
        $('#alarm-text').html(hour.concat(":",min_str,suffix));
	});
    $('#min-dn').click(function(){
        var time = get_time('#alarm-text');
        var hour = time[0];
        var min = parseInt(time[1]);
        var suffix = time[2];
        min = min - 1;
        if(min < 0){
            min = 59;
        }
        var min_str = min.toString();
        if(min < 10){
            min_str = "0" + min_str
        }
        $('#alarm-text').html(hour.concat(":",min_str,suffix));
	});
    $('#am-id').click(function(){
        var time = get_time('#alarm-text');
        var hour = time[0];
        var min = time[1];
        $('#alarm-text').html(hour.concat(":",min,"am"));
	});
    $('#pm-id').click(function(){
        var time = get_time('#alarm-text');
        var hour = time[0];
        var min = time[1];
        $('#alarm-text').html(hour.concat(":",min,"pm"));
	});
    //Submit alarm to be added
    $('#submit-id').click(function(){
        var time = get_time('#alarm-text');
        if(add_alarm(time)){
            var hour = (parseInt(time[0]) + 12).toString();
            var min = time[1];
            var day = weekday.indexOf(time[3]);
            var diff = time[4];
            var time_send = "1:".concat(hour,":",min,":",day,":",diff);
            console.log(time_send);
            // Send signal to remove Alarm
            // Format A:HH:MM:D:DF
            // A = amount of dates
            // HH = Hours
            // MM = Mins 
            // D = day [0-6]->[sunday-saturday] 
            // DF = Difficulty [1-4]
            socket.emit("message","submit " + time_send);
        }      
    });
    // Will remove Alarm if there is one
    $('#remove-id').click(function(){
        console.log("amt: ", num_alarms);
        if(num_alarms > 0){
            var select = document.getElementById("alarm-list-id");
            var val = select.options[select.selectedIndex].value;
            select.remove(select.selectedIndex);
            console.log("div_"+val);
            const element = document.getElementById("div_"+val);
            var text = element.textContent;
            console.log(text);
            element.remove();
            num_alarms -= 1;
            // Turn string into proper format
            var date_split = text.split(' ').slice(0);
            var timesplit = date_split[0].split(':').slice(0);
            var hours_str = timesplit[0];
            var suffix = timesplit[1].substring(2,4)

            var hours = parseInt(hours_str);
            if(suffix == "pm"){
                hours += 12;
            }
            hours_str = hours.toString();
            var mins_str = timesplit[1].substring(0,2);
            console.log(hours_str, mins_str);
            var day = weekday.indexOf(date_split[1]);
            // Send signal to remove Alarm
            // Format A:HH:MM:D
            // A = amount of dates
            // HH = Hours
            // MM = Mins 
            // D = day [0-6]->[sunday-saturday] 
            socket.emit("message", "remove ".concat("1:",hours_str,":",mins_str,":",day))
        }else{
            var errorTimer = setTimeout(function() {
                handleError("alarms","No Items to Remove");
            }, 1000);
        }
	});
    // Send a Trigger to start the alarm
    $('#trigger-id').click(function(){
        // Send signal to triiger Alarm 
        socket.emit("message", "trigger");
    });
});
    socket.on('update', function(data) {
        // Update the uptime
        update_list(data);
    });
    function add_alarm(time){
        if(num_alarms < 7){
            var alarm_str = time[0].concat(":",time[1],time[2]," ",time[3]);
            var select = document.getElementById("alarm-list-id");
            var size = select.length;
            for(var i = 0; i < size; i++){
                if(select.options[i].text.localeCompare(alarm_str) == 0){
                    var errorTimer = setTimeout(function() {
                        console.log("here")
                        handleError("alarms","Alarm Already in List");
                    }, 1000);
                    return false;
                }
            }
            var option = document.createElement("option")
            option.text = alarm_str;
            option.value = counter_id;
            num_alarms++;
            select.add(option);
            var $div = $("<div>", {id: "div_"+option.value, text: alarm_str})
            counter_id++;
            $('#alarm-box').append($div);
            return true;
        }else{
            var errorTimer = setTimeout(function() {
                handleError("alarms","Cannot Add More Alarms");
            }, 1000);
        }
        return false;
    }
    function get_time(ele){
        var text = $(ele).text();
        var time_text = text.split(':').slice(0)
        var hour = time_text[0];
        var min = time_text[1].substring(0,2);
        var suffix = time_text[1].substring(2,4);
        var day = weekday[$('#days-id').val()];
        var diff =$('input[name="diff"]:checked').val();
        return [hour,min,suffix,day,diff];
    }
    function update_clock(){
        const curr_time = new Date();
        var hour = curr_time.getHours();
        var min = curr_time.getMinutes();
        var suffix = "am";
        if(hour > 12){
            suffix = "pm";
            hour = hour - 12;
        }
        var min_str = min.toString()
        if( min < 10){
            min_str = "0" + min_str;
        }
        $('#clock-text').html(hour.toString().concat(":",min_str,suffix));
    }
    function update_list(data){
        console.log(data);
        var time_text = data.split(':').slice(0);
        var size = time_text[0];
        var offset = 1;
        for(var i = 0; i < size; i++){
            var hours = parseInt(time_text[0+(i*3)+offset]);
            var mins = parseInt(time_text[1+(i*3)+offset]);
            var day = weekday[parseInt(time_text[2+(i*3)+offset])];
            var suffix = "am";
            if(hours > 11){
                suffix = "pm";
                hours = hours - 12;
            }
            var hour_str = hours.toString();
            var min_str = mins.toString();
            if(mins < 10){
                console.log(min_str)
                min_str = "0" + min_str;
            }
            add_alarm([hour_str,min_str,suffix,day]);
        }
    }