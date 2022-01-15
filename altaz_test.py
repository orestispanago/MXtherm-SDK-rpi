from dataclasses import dataclass, field

cameraXPositions = ["x90", "x180", "x90"]
cameraYPositions = ["y90", "y45", "y90", "y135", "y90"]


def move_cam(position):
    print("Moved cam at:", position)


def download_photo(folder, x, y):
    print(f"Downloaded photo:{folder}/{x}_{y}.png")


# def move_cam_and_download(folder):
#     for countx, x in enumerate(cameraXPositions):
#         move_cam(x)
#         if countx != 2:
#             for county, y in enumerate(cameraYPositions):
#                 move_cam(y)
#                 if (county != 0) and (county != 4):
#                     download_photo(folder, x, y)


# move_cam_and_download("folder")
def rotate_cam(axis, degrees):
    print(f"Rotated cam at: {axis}, {degrees} degrees")


def aim(aimpoint):
    rotate_cam("x", aimpoint.altitude)
    rotate_cam("y", aimpoint.azimuth)
    print("Aimed camera at:", aimpoint)


@dataclass()
class AimPoint:
    altitude: int
    cardinal: str
    azimuth: int = field(init=False)
    take_photo: bool = False
    cardinal_degrees = {
        "N": 0,
        "E": 90,
        "S": 180,
        "W": 270
    }

    def check_altitude(self):
        if not 0 <= self.altitude <= 90:
            raise ValueError("Altitude must be from 0 to 90 degrees")

    def check_cardinal(self):
        nesw = list(self.cardinal_degrees.keys())
        if self.cardinal not in nesw:
            raise ValueError(f"Cardinal direction must be one of: {nesw}")

    def cardinal_to_degrees(self):
        return self.cardinal_degrees.get(self.cardinal)

    def __post_init__(self):
        self.check_altitude()
        self.check_cardinal()
        self.azimuth = self.cardinal_to_degrees()


aimpoints = [AimPoint(90, "S"),
             AimPoint(45, "S", take_photo=True),
             AimPoint(90, "S")]

# print(*aimpoints, sep="\n")

for aimpoint in aimpoints:
    aim(aimpoint)
    if aimpoint.take_photo:
        download_photo("output", aimpoint.altitude, aimpoint.cardinal)
