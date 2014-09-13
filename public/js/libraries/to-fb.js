function publishFacebook(canvas){
  var dataURL = canvas.toDataURL()
  var onlyData = dataURL.substring(dataURL.indexOf(',')+1, dataURL.length);
  var decoded = Base64Binary.decode(onlyData);
  var imageIwillPost = getFormData(decoded);
  console.log(imageIwillPost);
  FB.api('/me/photos', 'POST',
        {'source': '{' + imageIwillPost + '}',
         'message': 'Whatever message we decide'},
         function(resp) {
           console.log('into function');
           if (resp && !resp.error) {
             console.log('uploaded');
             console.log(resp);
           } else {
             console.log('some error');
             console.log(resp.error);}});}



function getFormData(imageData){
  var boundary = '----ThisIsTheBoundary1234567890';
  var formData = '--' + boundary + '\r\n'
  formData += 'Content-Disposition: form-data; name="source"; filename="' + 'image-vaun.png' + '"\r\n';
  formData += 'Content-Type: ' + 'PNG' + '\r\n\r\n';
  for ( var i = 0; i < imageData.length; ++i ){
    formData += String.fromCharCode( imageData[ i ] & 0xff );}
  formData += '\r\n';
  formData += '--' + boundary + '\r\n';

  return formData;
}


var Base64Binary = {
	_keyStr : "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",

	/* will return a  Uint8Array type */
	decodeArrayBuffer: function(input) {
		var bytes = (input.length/4) * 3;
		var ab = new ArrayBuffer(bytes);
		this.decode(input, ab);

		return ab;
	},

	decode: function(input, arrayBuffer) {
		//get last chars to see if are valid
		var lkey1 = this._keyStr.indexOf(input.charAt(input.length-1));
		var lkey2 = this._keyStr.indexOf(input.charAt(input.length-2));

		var bytes = (input.length/4) * 3;
		if (lkey1 == 64) bytes--; //padding chars, so skip
		if (lkey2 == 64) bytes--; //padding chars, so skip

		var uarray;
		var chr1, chr2, chr3;
		var enc1, enc2, enc3, enc4;
		var i = 0;
		var j = 0;

		if (arrayBuffer)
			uarray = new Uint8Array(arrayBuffer);
		else
			uarray = new Uint8Array(bytes);

		input = input.replace(/[^A-Za-z0-9\+\/\=]/g, "");

		for (i=0; i<bytes; i+=3) {
			//get the 3 octects in 4 ascii chars
			enc1 = this._keyStr.indexOf(input.charAt(j++));
			enc2 = this._keyStr.indexOf(input.charAt(j++));
			enc3 = this._keyStr.indexOf(input.charAt(j++));
			enc4 = this._keyStr.indexOf(input.charAt(j++));

			chr1 = (enc1 << 2) | (enc2 >> 4);
			chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
			chr3 = ((enc3 & 3) << 6) | enc4;

			uarray[i] = chr1;
			if (enc3 != 64) uarray[i+1] = chr2;
			if (enc4 != 64) uarray[i+2] = chr3;
		}

		return uarray;
	}
}