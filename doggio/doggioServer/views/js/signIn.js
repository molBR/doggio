$(document).ready(function(){
    $("#register").click(function(){
        var email 		= $("#email").val();
        var username 	= $("#username").val();
        var senha		= $("#senha").val();
        var confSenha	= $("#confSenha").val();

        if(senha == confSenha){
        	data = {
        		"email" 	: email,
        		"username" 	: username,
        		"senha"		: senha,
        		"confSenha" : confSenha
        	};
        	data = JSON.stringify(data);
        	console.log(data);
        	$.ajax({
				  type: 'POST',
				  url: '/signIn',
				  contentType: 'application/json',
				  data: data,
			}).done(function(data){
                res.json(data);
            });
        }else{
        	console.log("diferente");
        }

    });
});