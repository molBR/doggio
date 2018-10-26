var express 	= require('express');
var app			= express();
var morgan		= require('morgan');
var bodyParser  = require('body-parser'); 
var mongoose	= require('mongoose');


app.use(morgan('dev')); // log every request to the console

app.use(bodyParser.urlencoded({ extended: true })); //tratamento ajax e forms
app.use(bodyParser.json());

var configDB = require('./config/database.js');//Banco de dados
mongoose.connect(configDB.url); // connect to our database


require('./app/route.js')(app);


var server = app.listen(80,function (){
	var host = server.address().address
	var port = server.address().port
	console.log(server.address());
});