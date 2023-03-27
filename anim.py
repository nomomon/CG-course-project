import os
import json
import math

os.system("bash build.sh")

def pad(n):
    return str(n).zfill(3)

WORLD_CENTER = [0, 0, 0]
WORLD_RADIUS = 4000
FOV = 135


for i in range(0, 360, 2):
    j = - WORLD_RADIUS / 4 * (360 - i) / 360 +  WORLD_RADIUS / 4 * (i / 360)

    EYE = [
        0,
        j, 
        0
    ]

    EYE_ANGLE = [
        0,
        -70,
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
                "position": [0, 0, 0],
                "radius": WORLD_RADIUS,
                "rotation": [
                    -1,
                    0,
                    0
                ],
                "angle": 90 + i,
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
                "type": "sphere",
                "comment": "Transparent sphere",
                "position": [0, 200 + j, 0],
                "radius": 200,
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
        ]
    }

    # save the frame config
    json.dump(template, open("scenes/2_animation/1.json", "w"))

    # render the frame
    os.system(f"./build/ray scenes/2_animation/1.json scenes/2_animation/frames/{pad(i)}.png")

# make the video
os.system("rm out.gif")
os.system("ffmpeg -framerate 30 -pattern_type glob -i 'scenes/2_animation/frames/*.png' out.gif")