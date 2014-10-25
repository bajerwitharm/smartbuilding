salwatorska_app.factory('databaseProvider', function($resource, $http) {
    return {
	getUsersInfo : function() {
	    return $http.get("/getUsersInfo", {
		responseType : "json"
	    });
	},
	getConnectionsByHour : function() {
	    return $http.get("/getConnectionsByHour", {
		responseType : "json"
	    });
	},
	getConnectionsByDay : function() {
	    return $http.get("/getConnectionsByWeekday", {
		responseType : "json"
	    });
	},
	getUsageByHour : function() {
	    return $http.get("/getUsageByHour", {
		responseType : "json"
	    });
	},
	getUsageByWeekday : function() {
	    return $http.get("/getUsageByWeekday", {
		responseType : "json"
	    });
	},
    };
});