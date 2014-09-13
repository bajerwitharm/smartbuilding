salwatorskaControllers.controller('vwalkController', [ '$scope', '$rootScope',

function($scope, $rootScope) {
	$('iframe').each( function() {
	    var url = $(this).attr("src")
	    $(this).attr({
	        "src" : url.replace('?rel=0', '')+"?wmode=transparent",
	        "wmode" : "Opaque"
	    })
	});
} ]);
