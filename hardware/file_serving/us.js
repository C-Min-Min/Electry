function program_table(p_number, start_time, end_time, days, temperature) {
    document.write("<tr>");
    document.write("<td><form id='form");
    document.write(p_number);
    document.write("' action='/save/program_list.hex' method='post'>");
    document.write("<input type='hidden' name='program' value='");
    document.write(p_number);
    document.write("'/><label>");
    document.write(p_number);
    document.write("</label>");
    document.write("</td><td>");
    document.write("<input type='time' id='start_time");
    document.write(p_number);
    document.write("' name='start_time' value='");
    document.write(start_time);
    document.write("'></td>");
    document.write("<td><input type='time' id='end_time");
    document.write(p_number);
    document.write("' name='end_time' value='");
    document.write(end_time);
    document.write("'></td>");
    document.write("<td>");

    document.write('<input type="button"');
    if (Boolean(days & 0x01)) {
        document.write(' style="background:rgb(85, 51, 170); color:#FFFFFF"');
    }
    document.write(' id="btn_monday');
    document.write(p_number);
    document.write('" value="пн" onclick="set_data(\'btn_monday');
    document.write(p_number);
    document.write("', 'monday");
    document.write(p_number);
    document.write("')\"/>");

    document.write('<input type="button"');
    if (Boolean(days & 0x02)) {
        document.write(' style="background:rgb(85, 51, 170); color:#FFFFFF"');
    }
    document.write(' id="btn_tuesday');
    document.write(p_number);
    document.write('" value="вт" onclick="set_data(\'btn_tuesday');
    document.write(p_number);
    document.write("', 'tuesday");
    document.write(p_number);
    document.write("')\"/>");

    document.write('<input type="button"');
    if (Boolean(days & 0x04)) {
        document.write(' style="background:rgb(85, 51, 170); color:#FFFFFF"');
    }
    document.write(' id="btn_wednesday');
    document.write(p_number);
    document.write('" value="ср" onclick="set_data(\'btn_wednesday');
    document.write(p_number);
    document.write("', 'wednesday");
    document.write(p_number);
    document.write("')\"/>");

    document.write('<input type="button"');
    if (Boolean(days & 0x08)) {
        document.write(' style="background:rgb(85, 51, 170); color:#FFFFFF"');
    }
    document.write(' id="btn_thursday');
    document.write(p_number);
    document.write('" value="чт" onclick="set_data(\'btn_thursday');
    document.write(p_number);
    document.write("', 'thursday");
    document.write(p_number);
    document.write("')\"/>");

    document.write('<input type="button"');
    if (Boolean(days & 0x10)) {
        document.write(' style="background:rgb(85, 51, 170); color:#FFFFFF"');
    }
    document.write(' id="btn_friday');
    document.write(p_number);
    document.write('" value="пт" onclick="set_data(\'btn_friday');
    document.write(p_number);
    document.write("', 'friday");
    document.write(p_number);
    document.write("')\"/>");

    document.write('<input type="button"');
    if (Boolean(days & 0x20)) {
        document.write(' style="background:rgb(85, 51, 170); color:#FFFFFF"');
    }
    document.write(' id="btn_saturday');
    document.write(p_number);
    document.write('" value="сб" onclick="set_data(\'btn_saturday');
    document.write(p_number);
    document.write("', 'saturday");
    document.write(p_number);
    document.write("')\"/>");

    document.write('<input type="button"');
    if (Boolean(days & 0x40)) {
        document.write(' style="background:rgb(85, 51, 170); color:#FFFFFF"');
    }
    document.write('id="btn_sunday');
    document.write(p_number);
    document.write('" value="нд" onclick="set_data(\'btn_sunday');
    document.write(p_number);
    document.write("', 'sunday");
    document.write(p_number);
    document.write("')\"/>");

    document.write('<input type="button"');
    if (Boolean(days & 0x80)) {
        document.write(' style="background:rgb(85, 51, 170); color:#FFFFFF"');
    }
    document.write(' id="btn_holiday');
    document.write(p_number);
    document.write('" value="празник" onclick="set_data(\'btn_holiday');
    document.write(p_number);
    document.write("', 'holiday");
    document.write(p_number);
    document.write("')\"/>");

    document.write('<input type="hidden" name="mon" value="');
    if (Boolean(days & 0x01)) {
        document.write("y");
    } else document.write("n");
    document.write('" id="monday');
    document.write(p_number);
    document.write('"/>');

    document.write('<input type="hidden" name="tue" value="');
    if (Boolean(days & 0x02)) {
        document.write("y");
    } else document.write("n");
    document.write('" id="tuesday');
    document.write(p_number);
    document.write('"/>');

    document.write('<input type="hidden" name="wed" value="');
    if (Boolean(days & 0x04)) {
        document.write("y");
    } else document.write("n");
    document.write('" id="wednesday');
    document.write(p_number);
    document.write('"/>');

    document.write('<input type="hidden" name="thu" value="');
    if (Boolean(days & 0x08)) {
        document.write("y");
    } else document.write("n");
    document.write('" id="thursday');
    document.write(p_number);
    document.write('"/>');

    document.write('<input type="hidden" name="fri" value="');
    if (Boolean(days & 0x10)) {
        document.write("y");
    } else document.write("n");
    document.write('" id="friday');
    document.write(p_number);
    document.write('"/>');

    document.write('<input type="hidden" name="sat" value="');
    if (Boolean(days & 0x20)) {
        document.write("y");
    } else document.write("n");
    document.write('" id="saturday');
    document.write(p_number);
    document.write('"/>');

    document.write('<input type="hidden" name="sun" value="');
    if (Boolean(days & 0x40)) {
        document.write("y");
    } else document.write("n");
    document.write('" id="sunday');
    document.write(p_number);
    document.write('"/>');

    document.write('<input type="hidden" name="hol" value="');
    if (Boolean(days & 0x80)) {
        document.write("y");
    } else document.write("n");
    document.write('" id="holiday');
    document.write(p_number);
    document.write('"/>');

    document.write("</td><td>");
    document.write(
        "<input type='number' name='temperature' style='text-align:center;' value='"
    );
    document.write(temperature);
    document.write("' min='7' max='70'><label> C</label>");
    document.write("</td>");
    document.write("<td><input onclick='delete_data(");
    document.write(p_number);
    document.write(")' type='button' value='Изтрий'/></td>");

    document.write("<td><input onclick=\"check_data('form");
    document.write(p_number);
    document.write("', 'start_time");
    document.write(p_number);
    document.write("', 'end_time");
    document.write(p_number);
    document.write("', 'monday");
    document.write(p_number);
    document.write("', 'tuesday");
    document.write(p_number);
    document.write("', 'wednesday");
    document.write(p_number);
    document.write("', 'thursday");
    document.write(p_number);
    document.write("', 'friday");
    document.write(p_number);
    document.write("', 'saturday");
    document.write(p_number);
    document.write("', 'sunday");
    document.write(p_number);
    document.write("', 'holiday");
    document.write(p_number);
    document.write('\')" type="button" value="Запази"/></form></td>');

    document.write("</tr>");
}
