var express = require('express');
var router = express.Router();
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
          connection.query("SELECT * FROM `" + j +"` limit 24", function (err, result, fields) {
            if (err) throw err
            for(var z of result){
                tables.push(z)

            }
            let data = JSON.stringify(tables);
            fs.appendFileSync('tables.json', data);
          });
        }
      }
    });
});
