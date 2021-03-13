let mysql = require('mysql');
const { element } = require('prop-types');


let connection = mysql.createConnection({
    host: '34.107.85.119',
    user: 'root',
    password: 'hacktues_c',
    database: 'electry'
});

connection.connect(function(err) {
    if (err) throw err;

    connection.query("SHOW TABLES FROM `electry` LIKE '%measure%'", function (err, result, fields) {
      if (err) throw err;
      
      for(var i of result){
        for(var j of Object.values(i)){
          connection.query("SELECT * FROM `" + j +"`", function (err, result, fields) {
            if (err) throw err
            console.log(result)
          });
          
          
        }
      }
    });
});
