import os
import logging

import requests
from requests.auth import HTTPDigestAuth

logger = logging.getLogger(__name__)


class CamClient:
    def __init__(self, ip, user, passwd, thermal_downloader_path):
        self.ip = ip
        self.user = user
        self.passwd = passwd
        self.thermal_downloader_path = thermal_downloader_path

    def get_digest_auth(self, url):
        return requests.get(url, auth=HTTPDigestAuth(self.user, self.passwd))

    def enable_thermal_raw_data(self):
        url = f"http://{self.ip}/control/control/?set&section=thermal&uhu_raw_data=enable"
        self.get_digest_auth(url)

    def download_img(self, fname):
        logger.debug(f"Downloading image: {fname}...")
        url = f"http://{self.ip}/cgi-bin/image.jpg?display_mode=simple"
        resp = self.get_digest_auth(url)
        if resp.status_code == 200:
            with open(fname, "wb") as f:
                f.write(resp.content)
            logger.debug(f"Downloaded image: {fname}")
        else:
            logger.error(resp.status_code)

    def download_thermal(self, fname):
        logger.debug(f"Downloading thermal data: {fname}...")
        cmd = (
            f"{self.thermal_downloader_path} "
            f"--ip {self.ip} "
            f"--user {self.user} "
            f"--pass {self.passwd} "
            f"--output {fname}"
        )
        os.system(cmd)
        logger.debug(f"Downloaded thermal data: {fname}")
