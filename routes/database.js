var sqliteDbContext = require('../data/database.js');

module.exports.getUsersInfo = function(req, res){
	sqliteDbContext.getUsersInfo(function(data)
	{
		res.send(data);		
	});
};

module.exports.getConnectionsByHour = function(req, res){
	sqliteDbContext.getConnectionsByHour(function(data)
	{
		res.send(data);		
	});
};

module.exports.getUsageByHour = function(req, res){
	sqliteDbContext.getUsageByHour(function(data)
	{
		res.send(data);		
	});
};