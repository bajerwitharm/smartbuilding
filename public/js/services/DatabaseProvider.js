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
	getConnectionsByWeekday : function() {
	    return $http.get("/getConnectionsByWeekday", {
		responseType : "json"
	    });
	},
	getConnectionsByMonthday : function() {
	    return $http.get("/getConnectionsByMonthday", {
		responseType : "json"
	    });
	},
	getConnectionsByMonth : function() {
	    return $http.get("/getConnectionsByMonth", {
		responseType : "json"
	    });
	},
	getConnectionsByAP : function() {
	    return $http.get("/getConnectionsByAP", {
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
	getUsageByMonthday : function() {
	    return $http.get("/getUsageByMonthday", {
		responseType : "json"
	    });
	},
	getUsageByMonth : function() {
	    return $http.get("/getUsageByMonth", {
		responseType : "json"
	    });
	},
	getUsageByAP : function() {
	    return $http.get("/getUsageByAP", {
		responseType : "json"
	    });
	},
    };
});