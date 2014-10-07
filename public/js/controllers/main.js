salwatorskaControllers.controller('mainController', [ '$scope', '$rootScope',

function($scope, $rootScope) {

	$scope.$on('ngrepeatfinish', function(ngRepeatFinishedEvent) {
		$('#tiles').imagesLoaded(function() {
			// Prepare layout options.
			var options = {
				autoResize : true, // This will auto-update the layout when the
									// browser window is resized.
				container : $('#main1'), // Optional, used for some extra CSS
											// styling
				offset : 2, // Optional, the distance between grid items
				itemWidth : 200
			// Optional, the width of a grid item
			};

			// Get a reference to your grid items.
			var handler = $('#tiles li');

			// Call the layout function.
			handler.wookmark(options);

			// Init lightbox
			$('a', handler).colorbox({
				rel : 'lightbox'
			});
		})(jQuery);
	});
} ]);
