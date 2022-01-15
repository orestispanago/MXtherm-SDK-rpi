from models import AimPoint
from controllers import rotate_cam, download_photo

aim_points = [
    AimPoint(90, 90),
    AimPoint(120, 90, photo=True)
]
# arduino = Arduino().connect()
for aimpoint in aim_points:
    rotate_cam("x", aimpoint.x)
    rotate_cam("y", aimpoint.y)
    print("Aimed camera at:", aimpoint)
    if aimpoint.photo:
        download_photo("output", aimpoint.x, aimpoint.y)
