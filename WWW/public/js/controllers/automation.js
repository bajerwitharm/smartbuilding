salwatorskaControllers.controller('automationController', [ '$scope',
	'$rootScope', 'databaseProvider',

	function($scope, $rootScope, databaseProvider) {
	    $scope.state = {
		header : {
		    start : 126,
		    source : 251,
		    destination : 250,
		    fc : 222,
		    size : 18
		},
		inputs : {
			NotUsed : false,

		},
		outputs : {
			NotUsed : false,
			ToiletOut : false,
			ToiletIn : false,
			Corridor : false,
			Room2 : false,
			bulb_enterance : false,
			bulb_stairsleft : false,
			bulb_stairsright : true
		},
		states: {
			CorritorMotion : false,
			CorridorSw : false,
			RoomMotion : false,
			RoomSw2 : false,
			RoomSw1 : false,
			KitchenMotion : false,
			KitchenSw2 : false,
			KitchenSw1 : false  
		},
		crc : 171
	    }
	} ]);
