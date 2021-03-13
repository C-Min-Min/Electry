let mysql = require('mysql');

let connection = mysql.createConnection({
    host: '145.14.144.199',
    user: 'id15617968_avatarbg555',
    password: '3s?BC&=FY0Ujj>Bn',
    database: 'measurements_6_0'
});

connection.connect(function(err) {
    if (err) {
      return console.error('error: ' + err.message);
    }
  
    console.log('Connected to the MySQL server.');
})