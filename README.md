#### Mobotix-Downloader-SDK

Downloads raw data from Mobotix IP cam thermal sensor.

Converts to Celsius

Saves to ```.csv``` and exits.

IP, username, password and output file path are passed as arguments.

Example:

``` ./build/thermal-raw --ip 0.0.0.0 --user admin --pass 'pass!@' --output ../output/a.csv```

Based on [Mobotix EventStreamClient](https://developer.mobotix.com/) thermal-raw example.