var mysql = require('mysql');

var con = mysql.createConnection({
  host: "34.107.85.119",
  user: "root",
  password: "hacktues_c"
});

con.connect(function(err) {
  if (err) throw err;
  console.log("Connected!");
});