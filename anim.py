import os

def pad(n):
    return str(n).zfill(3)


for i in range(0, 360):
    template = '''{
        "Shadows": false,
        "MaxRecursionDepth": 2,
        "SuperSamplingFactor": 1,
        "Eye": [
            200,
            200,
            1000
        ],
        "Lights": [
            {
                "position": [
                    200,
                    200,
                    1000
                ],
                "color": [
                    1.0,
                    1.0,
                    1.0
                ]
            }
        ],
        "Objects": [
            {
                "type": "sphere",
                "comment": "Transparent sphere",
                "position": [
                    200,
                    200,
                    100
                ],
                "radius": 100,
                "rotation": [
                    0,
                    1,
                    0.7
                ],
                "angle": 90,
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
                    "isTransparent": true,
                    "nt": 1.7
                }
            },
            {
                "type": "sphere",
                "comment": "Camera sphere",
                "position": [
                    200,
                    200,
                    1002
                ],
                "radius": 1,
                "material": {
                    "color": [
                        1.0,
                        1.0,
                        1.0
                    ],
                    "ka": 1.0,
                    "kd": 0.0,
                    "ks": 0.0,
                    "n": 16
                }
            },
            {
                "type": "sphere",
                "comment": "Background sphere",
                "position": [
                    200,
                    200,
                    1000
                ],
                "radius": 10000,
                "rotation": [
                    -1,
                    0,
                    0
                ],
                "angle": ''' + str(i) + ''',
                "material": {
                    "texture": "textures/pond.png",
                    "ka": 0.2,
                    "kd": 0.8,
                    "ks": 0,
                    "n": 1
                }
            }
        ]
    }'''

    with open('scenes/8_animation/1.json', 'w+') as f:
        f.write(template)

    os.system(f"./build/ray scenes/8_animation/1.json scenes/8_animation/frames/{pad(i)}.png")

os.system("ffmpeg -framerate 30 -pattern_type glob -i 'scenes/8_animation/frames/*.png' -c:v libx264 -pix_fmt yuv420p out.mp4")