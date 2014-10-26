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

module.exports.getConnectionsByWeekday = function(req, res){
	sqliteDbContext.getConnectionsByWeekday(function(data)
	{
		res.send(data);		
	});
};

module.exports.getConnectionsByMonthday = function(req, res){
	sqliteDbContext.getConnectionsByMonthday(function(data)
	{
		res.send(data);		
	});
};

module.exports.getConnectionsByMonth = function(req, res){
	sqliteDbContext.getConnectionsByMonth(function(data)
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

module.exports.getUsageByWeekday = function(req, res){
	sqliteDbContext.getUsageByWeekday(function(data)
	{
		res.send(data);		
	});
};

module.exports.getUsageByMonthday = function(req, res){
	sqliteDbContext.getUsageByMonthday(function(data)
	{
		res.send(data);		
	});
};

module.exports.getUsageByMonth = function(req, res){
	sqliteDbContext.getUsageByMonth(function(data)
	{
		res.send(data);		
	});
};