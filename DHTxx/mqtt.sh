!/bin/ash
mqtt_server='192.168.0.25'
data_url='http://192.168.0.29/cgi-bin/gettemp'
data=$(curl "$data_url")
temp=$(echo "$data" | grep -Eo '(temp: )[^,]*' | tr -d 'temp: ')
humidity=$(echo "$data" | grep -Eo '(humidity: )[^}]*' | tr -d 'humidity: ')
mosquitto_pub -h "$mqtt_server" -t "nullo/temp" -m "$temp"
mosquitto_pub -h "$mqtt_server" -t "nullo/humidity" -m "$humidity"
