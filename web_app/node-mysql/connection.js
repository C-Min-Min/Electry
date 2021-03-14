let mysql = require('mysql');

let connection = mysql.createConnection({
    host: '34.107.85.119',
    user: 'root',
    password: 'hacktues_c',
    database: 'electry'
});

let tables = []

connection.connect(function(err) {
    if (err) throw err;
    connection.query("SHOW TABLES FROM `electry` LIKE '%measure%'", function (err, result, fields) {
      if (err) throw err;
      
      for(var i of result){
        for(var j of Object.values(i)){
          connection.query("SELECT * FROM `" + j +"`", function (err, result_2, fields) {
            if (err) throw err
            for(var z of result_2){
              for(var x of Object.values(z)){
                tables.push(x)
              }
            }
          });
        }
      }
    });
    console.log("Nice!")
    console.log(tables)
});



/* let mysql = require('mysql');

let connection = mysql.createConnection({
    host: '34.107.85.119',
    user: 'root',
    password: 'hacktues_c',
    database: 'electry'
});

let tables = []
let isReady = false

const conn = () => {
  return new Promise(resolve => {
    connection.connect(function(err) {
      if (err) throw err;
        connection.query("SHOW TABLES FROM `electry` LIKE '%measure%'", function (err, result, fields) {
          if (err) throw err;
          
          for(var i of result){
            for(var j of Object.values(i)){
              connection.query("SELECT * FROM `" + j +"`", function (err, result_2, fields) {
                if (err) throw err
                for(var z of result_2){
                  for(var x of Object.values(z)){
                    tables.push(x)
                  }
                }
              });
            }
          }
          
        });
  
    });console.log(tables)
  });
}

async function  check(){
  await conn()
  console.log(isReady)
} 

check() */
