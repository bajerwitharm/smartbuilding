var dbContext;

module.exports.init = function(context){
	dbContext = context;
};

module.exports.getAvgValues = function(req, res){
	dbContext.getAvgValues(function(data)
	{
		res.send(data);		
	}, req.query);
};

