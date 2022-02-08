class AimPoint:
    def __init__(self, x, y, photo=False):
        self.x = x
        self.y = y
        self.photo = photo

    @classmethod
    def from_file(cls, fname):
        aimpoints = []
        with open(fname, "r") as f:
            for line in f.readlines()[1:]:
                line = line.split(",")
                aimpoint = cls(line[1], line[2], line[3])
                aimpoints.append(aimpoint)
        return aimpoints

    def __repr__(self) -> str:
        return f"AimPoint={{x:{self.x}, y:{self.y}, photo: {self.photo}}}"
