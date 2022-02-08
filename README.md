#### Mobotix-Downloader-SDK

Rotates camera around 2 axes, downloads images and temperatures .csv file.

Uploads folder to FTP server, creating the remote path if it does not exist.

Cronjob:

```
*/5 * * * * cd ~/Mobotix-Downloader-SDK; python3 control_and_download.py > crontab.log 2> crontab.err
```
Example:

``` ./build/thermal-raw --ip 0.0.0.0 --user admin --pass 'pass!@' --output ../output/a.csv```

Based on [Mobotix EventStreamClient](https://developer.mobotix.com/) thermal-raw example.