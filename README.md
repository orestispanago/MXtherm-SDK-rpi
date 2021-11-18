#### Mobotix-Downloader-SDK

Downloads raw data from Mobotix IP cam thermal sensor.

Converts to Celsius

Saves to ```x<pos_x>_y<pos_y>_<date_time>.csv``` and exits.

IP, username, password, x and y positions are passed as arguments.

Example command for x=90, y=45:

``` ./build/thermal-raw 150.140.194.28 admin 'pass!@' 90 45 ```

Based on [Mobotix EventStreamClient](https://developer.mobotix.com/) thermal-raw example.