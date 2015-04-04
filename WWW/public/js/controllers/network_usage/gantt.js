salwatorskaControllers.controller('networkUsageGanttChartController', function($scope, $rootScope, $timeout, Sample, moment, databaseProvider, GANTT_EVENTS) { 
    $scope.options = {
            mode: 'custom',
            scale: 'hour',
            maxHeight: false,
            width: true,
            autoExpand: 'none',
            taskOutOfRange: 'truncate',
            fromDate: undefined,
            toDate: undefined,
            showLabelsColumn: true,
            currentDate: 'none',
            currentDateValue : new Date(),
            draw: false,
            readOnly: true,
            filterTask: undefined,
            filterRow: undefined,
            timeFrames:
                 {'day': {
                    start: moment('8:00', 'HH:mm'),
                        end: moment('20:00', 'HH:mm'),
                        working: true,
                        default: true
                    },
                 'noon': {
                     start: moment('12:00', 'HH:mm'),
                     end: moment('13:30', 'HH:mm'),
                     working: false,
                     default: true
                 },
                 'weekend': {
                     working: false
                 }
                },
            dateFrames: {
                'weekend': {
                    evaluator: function(date) {
                        return date.isoWeekday() === 6 || date.isoWeekday() === 7;
                    },
                    targets: ['weekend']
                }
            },
            timeFramesNonWorkingMode: 'hidden',
            columnMagnet: '5 minutes'
        };
        
        $scope.connectionsInTimeChart = [];

        $scope.$watch('options.scale', function(newValue, oldValue) {
            if (!angular.equals(newValue, oldValue)) {
                if (newValue === 'quarter') {
                    $scope.options.headersFormats = {'quarter': '[Q]Q YYYY'};
                    $scope.options.headers = ['quarter'];
                } else {
                    $scope.options.headersFormats = undefined;
                    $scope.options.headers = undefined;
                }
            }
        });

        $scope.$on(GANTT_EVENTS.READY, function() {
            $timeout(function() {
                $scope.scrollToDate(new Date());
            }, 0, true);
        });

        
	$scope.changeSelectedView = function(view) {
	    $scope.options.scale = view;
	    $scope.selectedView = view;
	};

        var logTaskEvent = function(event, data) {
            // A task event has occured.
            var output = '';
            for (var property in data) {
                var propertyValue = data[property];
                if (property === 'evt' && propertyValue) {
                    propertyValue = propertyValue.type;
                } else if (property === 'element' && propertyValue.length > 0) {
                    propertyValue = propertyValue[0].localName + (propertyValue[0].className ? '.' + propertyValue[0].className : '');
                } else if (property === 'task') {
                    propertyValue = propertyValue.name;
                } else if (property === 'timespan') {
                    propertyValue = propertyValue.name;
                } else if (property === 'column') {
                    propertyValue = propertyValue.date.format() + ' <---> ' + propertyValue.endDate.format();
                }  else if (property === 'row') {
                    propertyValue = propertyValue.name;
                } else if (property === 'date') {
                    propertyValue = propertyValue.format();
                }
                output += property + ': ' + propertyValue +'; ';
            }
            console.log('$scope.$on: ' + event.name + ': ' + output);
        };
        
        

	    
            var getTimespan = function() {
                return [];
            };
	    
            var getUuid = function() {
                return {
                    s4: function() {
                        return Math.floor((1 + Math.random()) * 0x10000)
                            .toString(16)
                            .substring(1);
                    },
                    randomUuid: function() {
                        return this.s4() + this.s4() + '-' + this.s4() + '-' + this.s4() + '-' +
                            this.s4() + '-' + this.s4() + this.s4() + this.s4();
                    }
                };
            };
            
            var prepareConnectionsInTimeChart = function() {
		    $scope.connectionsInTimeChart=[];
		    if ($scope.connectionsInTime!=null) $scope.connectionsInTime.forEach(function(entry) {
			    var element_index = $rootScope
				    .findItem(
					    $rootScope.filteredUsersInfo,
					    "user_id", entry.user_id);
			    if (element_index >= 0) {
				if ($scope.connectionsInTimeChart[element_index]==null) {
				    $scope.connectionsInTimeChart[element_index] = {'id':getUuid().randomUuid(),'name':$rootScope.filteredUsersInfo[element_index].name,'order': element_index,'tasks':[]}; 
				};
				$scope.connectionsInTimeChart[element_index].tasks.push({'id':getUuid().randomUuid(),'name':'','color': '#ffea00','from':entry.start_timestamp,'to':entry.end_timestamp});
			    };
		    });
		    $scope.clearData();
		    $scope.loadData($scope.connectionsInTimeChart);
		    $scope.loadTimespans(getTimespan());
		    $scope.scrollToDate(new Date());
            };
            
            var getConnectionsInTime = function() {
		databaseProvider.getConnectionsInTime()
		.success(function(data) {
		    $scope.connectionsInTime = data;
		    prepareConnectionsInTimeChart();
		});
            }
                

                  
        $scope.$on(GANTT_EVENTS.TASK_CLICKED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.TASK_DBL_CLICKED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.TASK_CONTEXTMENU, logTaskEvent);
        $scope.$on(GANTT_EVENTS.TASK_ADDED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.TASK_CHANGED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.TASK_REMOVED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.TASK_MOVE_BEGIN, logTaskEvent);
        // $scope.$on(GANTT_EVENTS.TASK_MOVE, logTaskEvent);
        $scope.$on(GANTT_EVENTS.TASK_MOVE_END, logTaskEvent);
        $scope.$on(GANTT_EVENTS.TASK_RESIZE_BEGIN, logTaskEvent);
        // $scope.$on(GANTT_EVENTS.TASK_RESIZE, logTaskEvent);
        $scope.$on(GANTT_EVENTS.TASK_RESIZE_END, logTaskEvent);

        $scope.$on(GANTT_EVENTS.COLUMN_CLICKED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.COLUMN_DBL_CLICKED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.COLUMN_CONTEXTMENU, logTaskEvent);

        $scope.$on(GANTT_EVENTS.ROW_MOUSEDOWN, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_MOUSEUP, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_CLICKED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_DBL_CLICKED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_CONTEXTMENU, logTaskEvent);

        $scope.$on(GANTT_EVENTS.ROW_ORDER_CHANGED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_CHANGED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_ADDED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_REMOVED, logTaskEvent);

        $scope.$on(GANTT_EVENTS.ROW_LABEL_MOUSEDOWN, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_LABEL_MOUSEUP, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_LABEL_CLICKED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_LABEL_DBL_CLICKED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_LABEL_CONTEXTMENU, logTaskEvent);

        $scope.$on(GANTT_EVENTS.ROW_HEADER_MOUSEDOWN, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_HEADER_MOUSEUP, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_HEADER_CLICKED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_HEADER_DBL_CLICKED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.ROW_HEADER_CONTEXTMENU, logTaskEvent);

        $scope.$on(GANTT_EVENTS.ROW_LABELS_RESIZED, logTaskEvent);

        $scope.$on(GANTT_EVENTS.TIMESPAN_ADDED, logTaskEvent);
        $scope.$on(GANTT_EVENTS.TIMESPAN_CHANGED, logTaskEvent);

        $scope.$on(GANTT_EVENTS.READY, logTaskEvent);
        
        $scope.$on(GANTT_EVENTS.ROWS_FILTERED, function(event, data) {
            console.log('$scope.$on: ' + event.name + ': ' + data.filteredRows.length + '/' +  data.rows.length + ' rows displayed.');
        });

        $scope.$on(GANTT_EVENTS.TASKS_FILTERED, function(event, data) {
            console.log('$scope.$on: ' + event.name + ': ' + data.filteredTasks.length + '/' + data.tasks.length + ' tasks displayed.');
        });
        
	$scope.$on('networkUsersGetAllDataAgain',prepareConnectionsInTimeChart);
	getConnectionsInTime();
});