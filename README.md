# CV Frame Effects

Simple project that transforms photo collections into stunning video sequences enhanced with custom OpenCV effects and real-time frame processing.

## Effects: 

### Dotted Contour
- Evenly spaced glowing dots along contours
- Soft glow halos & color variations
- Smooth fade animations

View result: [Dotted Contour Effect](result/hope-dots.mp4)

### Ripple
- Multiple simultaneous spherical waves
- Dynamic ring width & acceleration
- Chromatic aberration edges

View result: [Ripple Effect](result/carl-ripple.mp4)

### Tone
- Preserves original brightness
- Metallic paint simulation
- Animated color palettes

View result: [Tone Effect](result/carl-tone.mp4)

### Chromatic Aberration
- Animated RGB channel shifting
- Adjustable intensity
- Mask-based application

View result: [Chromatic Aberration Effect](result/andy-chromatic.mp4)


## Installation

- Install OpenCV with contrib modules by following the official [instruction](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html).

- Ensure you have both ```opencv``` and ```opencv-contrib``` modules installed.

- Clone this repository:
```bash
git clone https://github.com/Arseniy16/cv-frame-effects/
cd cv-frame-effects
```

## Build 

- Navigate to the project directory:
```bash
cd effect_sandbox
```

<!-- - Create ```3rd/``` directory and add ```opencv``` with ```opencv-contrib``` here:   
```
mkdir 3rd
mv {opencv,opencv-contrib} 3rd/
``` -->

- Create and enter```build``` directory:
```bash
mkdir -p build && cd build
```

- Configure with CMake:
```bash
cmake ..
```

- Build the project:
```bash
make -j8
```

- Run program to test:
```bash
./effect
```

**Note:** Ensure ```opencv``` and ```opencv-contrib``` should be placed in the ```3rd``` directory.
