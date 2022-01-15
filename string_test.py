camera_ip = ""
camera_user = ""
camera_password = ""
thermal_downloader_path = "./src/build/mxtherm"

msg = (
    f"{thermal_downloader_path} "
    f"--ip {camera_ip} "
    f"--user {camera_user} "
    f"--pass '{camera_password}'"
)

folder = "output"
x = 90
y = 45
print(f"{msg} --output {folder}/{x}_{y}.csv")
