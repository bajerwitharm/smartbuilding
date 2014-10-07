salwatorska_app.factory('databaseProvider', function($resource, $http) {
    return {
	getUsersInfo : function() {
	    return $http.get("/getUsersInfo", {
		responseType : "json"
	    });
	},
    };
});