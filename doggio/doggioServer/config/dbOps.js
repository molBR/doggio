var User            = require('../app/models/user');

var insertUser = function(user,res,callback){
	User.findOne({ 'local.username' :  user.username }, function(err, gotUser) {
		if (err)
		    return res.json(err);
		if (gotUser) {
			console.log("veio ate aqui1");	
			callback({'error' : 1});
	}else {
		User.findOne({'local.email' : user.email},function(err,gotUser){
			if(err)
				return res.json(err);
			if(gotUser){
				console.log("veio ate aqui2");	
				callback({'error' : 2});
			}else{
				newUser = new User();
				newUser.local.username = user.username;
				newUser.local.email = user.email;
				newUser.local.password = newUser.generateHash(user.password);	
				newUser.save(function(err){
					if(err)
						throw err;
					callback({'error' : 0});
				});
			}
		});
	}
});
}

module.exports = {insertUser : insertUser}