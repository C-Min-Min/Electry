const fs = require('fs');

let mysql = require('mysql');
const { element } = require('prop-types');


let connection = mysql.createConnection({
    host: '34.107.85.119',
    user: 'root',
    password: 'hacktues_c',
    database: 'electry'
});

let tables = [];

connection.connect(function(err) {
    if (err) throw err;

    connection.query("SELECT table_name FROM information_schema.TABLES WHERE table_name LIKE '%measure%'order by CREATE_TIME desc limit 1", function (err, result, fields) {
      if (err) throw err;
      
      for(var i of result){
        for(var j of Object.values(i)){
          connection.query("SELECT `power` FROM `" + j +"`", function (err, result, fields) {
            if (err) throw err
            for(var z of result){

                tables.push(z)
                console.log(z)
            }
            let data = JSON.stringify(tables);
            fs.writeFileSync('./tables.json', data, 'utf8', err => {
              if (err) throw err;
            console.log('File has been saved!');
            });
          });
        }
      }
    });
});
