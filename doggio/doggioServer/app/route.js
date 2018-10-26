var User            = require('../app/models/user');
var dbOps			= require('../config/dbOps');

module.exports = function(app){

app.get('/',function(req,res){
	res.sendFile('index.html', {root: './views'});
});

app.post('/entrar',function(req,res){
	console.log("entrou!");
});

app.get('/signIn',function(req,res){
	res.sendFile('signIn.html',{root: './views'});
});

app.get('/signIn.js',function(req,res){
	res.sendFile('js/signIn.js',{root: './views'});
});

app.post('/signIn',function(req,res){
	dbOps.insertUser(req.body,res,function(response){
		if(response.error == 0){
			console.log("Usuario cadastrado com sucesso");
			res.json("deu certo!");
		}else if(response.error == 1){
			console.log("username ja esta sendo utilizado");
			res.json("username ja esta sendo utilizado");
		}else if(response.error == 2){
			console.log("email ja esta sendo utilziado");
			res.json("email ja esta sendo utilziado");
		}
	});


});
}