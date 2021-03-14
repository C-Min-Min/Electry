

let mysql  = require('mysql');

let connection = mysql.createConnection({
    host: '34.107.85.119',
    user: 'root',
    password: 'hacktues_c',
    database: 'electry'
});

console.log(tables);