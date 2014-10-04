var sqliteDbContext = require('../data/database.js');

module.exports.getUsersInfo = function(req, res){
	sqliteDbContext.getUsersInfo(function(data)
	{
		res.send(data);		
	});
};