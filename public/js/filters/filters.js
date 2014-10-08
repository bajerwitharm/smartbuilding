(function() {
    "use strict";

    angular.module('filters', []).filter(
	    'bytes',
	    [ function() {
		return function(bytes, precision) {
		    if (bytes === 0) {
			return '0 B';
		    }

		    if (isNaN(parseFloat(bytes)) || !isFinite(bytes)) {
			return '-';
		    }

		    var isNegative = bytes < 0;
		    if (isNegative) {
			bytes = -bytes;
		    }

		    if (typeof precision === 'undefined') {
			precision = 1;
		    }

		    var units = [ 'kB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB',
			    'YB' ];
		    var exponent = Math.min(Math.floor(Math.log(bytes)
			    / Math.log(1024)), units.length - 1);
		    var number = (bytes / Math.pow(1024, Math.floor(exponent)))
			    .toFixed(precision);

		    return (isNegative ? '-' : '') + number + ' '
			    + units[exponent];
		}
	    } ]).filter("asDate", function() {
	return function(input) {
	    return new Date(input);
	}
    }).filter('listfilter', [ function() {
	return function(items, searchText) {
	    var filtered = [];

	    angular.forEach(items, function(item) {
		if (item.label.indexOf(searchText) >= 0)
		    filtered.push(item);
	    });

	    return filtered;
	}
    } ]);

})();