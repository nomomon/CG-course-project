import os
import json
import math

os.system("bash build.sh")

def pad(n):
    return str(n).zfill(3)

def sphere(x, y, z, r=150):
    return {
                "type": "sphere",
                "comment": "Transparent sphere",
                "position": [x*r, y*r, z*r],
                "radius": r,
                "material": {
                    "color": [
                        1.0,
                        1.0,
                        1.0
                    ],
                    "ka": 0.0,
                    "kd": 0.0,
                    "ks": 0.0,
                    "n": 16,
                    "isTransparent": True,
                    "nt": 1.7
                }
            }

WORLD_CENTER = [0, 0, 0]
WORLD_RADIUS = 4000
EYE_RADIUS = 200
FOV = 135


for i in range(0, 360, 2):
    EYE = [
        0,
        i * (360 - i) / (180 * 180) * EYE_RADIUS, 
        0
    ]

    EYE_ANGLE = [
        i**2 // 360**2 * 50,
        -i,
        0
    ]

    template = {
        "Width": 600,
        "Height": 400,
        "Shadows": False,
        "MaxRecursionDepth": 10,
        "SuperSamplingFactor": 2,
        "Eye": EYE,
        "EyeRotation": EYE_ANGLE,
        "FOV": FOV,
        "Lights": [
            {
                "comment": "Sun",
                "position": [
                    2800,
                    1000,
                    1400
                ],
                "color": [
                    1.0,
                    0.8,
                    0.6
                ]
            }
        ],
        "Objects": [
            {
                "type": "sphere",
                "comment": "Background sphere",
                "position": [0, 2000, 0],
                "radius": WORLD_RADIUS,
                "rotation": [
                    -1,
                    0,
                    0
                ],
                "angle": 90,
                "material": {
                    "texture": "textures/pond_2k.png",
                    # "color": [1, 1, 1],
                    "ka": 0.2,
                    "kd": 0.8,
                    "ks": 0,
                    "n": 1
                }
            },
            {
                "type": "quad",
                "comment": "Floor",
                "v0": [1000, 0, -1000],
                "v1": [-1000, 0, -1000],
                "v2": [1000, 0, -1000],
                "v3": [1000, 0, 1000],
                "material":
                {
                    "texture": "textures/wood.png",
                    "ka": 1.0,
                    "kd": 0.0,
                    "ks": 0.0,
                    "n": 16
                }
            }
        ]
    }

    template["Objects"] += [sphere(math.cos(math.radians(0)), 1, math.sin(math.radians(0)))]
    template["Objects"] += [sphere(math.cos(math.radians(120)), 1, math.sin(math.radians(120)))]
    template["Objects"] += [sphere(math.cos(math.radians(240)), 1, math.sin(math.radians(240)))]

    # save the frame config
    json.dump(template, open("scenes/8_animation/1.json", "w"))

    # render the frame
    os.system(f"./build/ray scenes/8_animation/1.json scenes/8_animation/frames/{pad(i)}.png")

# make the video
os.system("rm out.mp4")
os.system("ffmpeg -framerate 30 -pattern_type glob -i 'scenes/8_animation/frames/*.png' -c:v libx264 -pix_fmt yuv420p out.mp4")