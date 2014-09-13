/*

	A video transcoding stream which converts an 
	input video from one format/quality/etc into
	another using FFMPEG.

	Is smart in a few ways:

		*	Does not transcode if there are no 
			output streams being piped to, and
			frees memory by destroying the
			FFMPEG object when not in use.

		*	Automatically pauses and resumes
			the transcoding process as streams
			being piped to are added/removed

*/

var stream 		= require('stream'),
	ffmpeg 		= require('fluent-ffmpeg'),
	util		= require('util');

function FfmpegTranscodeStream(options, config, preset) {

	if (!(this instanceof FfmpegTranscodeStream))
		return new FfmpegTranscodeStream(options);
	stream.Transform.call(this, options);

	var self			= this;
	this.super 			= stream.Transform;
	this.options 		= options;

	// inherit from transform stream class
	util.inherits(this, this.super);

	this.clientCount 	= 0;													// number of output streams being piped to
	this.transcoder 	= null;													// FFMPEG object
	this.preset 		= preset ? preset : null;								// FFMPEG preset object
	this.config 		= config ? config : { };								// FFMPEG settings

	this.isActive = function() {
		return this.clientCount > 0;
	}

	this.start = function() {
  		
		if(this.clientCount == 0) {
			this.resume();
			// create FFMPEG object
			this.transcoder = new ffmpeg(this.config);
			this.transcoder = this.preset(this.transcoder);

			// start the process	
			this.transcoder.writeToStream(this, function(retcode, error){
				if(error) {
					console.log(error);
					// TODO: 	optional silence
					//			throw exception
				}
			});
		}

		this.clientCount++;

	}

	this.stop = function() {

		this.clientCount--;

		if(this.clientCount <= 0) {
			this.clientCount = 0;
			this.transcoder.stop();
			this.pause();
		}

	}

	this._transform = function(chunk, encoding, done) {
		//process.stdout.write('.');	// DEBUG
		this.push(chunk);
		done();
	}

}
util.inherits(FfmpegTranscodeStream, stream.Transform);

module.exports = FfmpegTranscodeStream;