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
    };
});