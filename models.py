class AimPoint:

    def __init__(self, x, y, photo=False):
        self.x = self.check_min_max(x, 90, 180)
        self.y = self.check_min_max(y, 45, 135)
        self.photo = photo

    @staticmethod
    def check_min_max(val, min, max):
        if not min <= val <= max:
            raise ValueError(f"Expected value from {min} to {max} degrees")
        return val

    def __repr__(self):
        return f"{self.__class__.__name__}(x={self.x}, y={self.y}, photo={self.photo})"
