salwatorska_app.factory('databaseProvider', function($resource, $http, $routeParams) {
    return {
	getUsersInfo : function() {
	    return $http.get("/getUsersInfo", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getConnectionsByHour : function() {
	    return $http.get("/getConnectionsByHour", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getConnectionsByWeekday : function() {
	    return $http.get("/getConnectionsByWeekday", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getConnectionsByMonthday : function() {
	    return $http.get("/getConnectionsByMonthday", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getConnectionsByMonth : function() {
	    return $http.get("/getConnectionsByMonth", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getConnectionsByAP : function() {
	    return $http.get("/getConnectionsByAP", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getConnectionsInTime : function() {
	    return $http.get("/getConnectionsInTime", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getConnectionsLast : function() {
	    return $http.get("/getConnectionsLast", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getUrlsLast : function() {
	    return $http.get("/getUrlsLast", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getOtherLogsLast : function() {
	    return $http.get("/getOtherLogsLast", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getUsageByHour : function() {
	    return $http.get("/getUsageByHour", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getUsageByWeekday : function() {
	    return $http.get("/getUsageByWeekday", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getUsageByMonthday : function() {
	    return $http.get("/getUsageByMonthday", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getUsageByMonth : function() {
	    return $http.get("/getUsageByMonth", {
		responseType : "json",
		params : $routeParams
	    });
	},
	getUsageByAP : function() {
	    return $http.get("/getUsageByAP", {
		responseType : "json",
		params : $routeParams
	    });
	},
    };
});