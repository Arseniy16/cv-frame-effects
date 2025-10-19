# CV Frame Effects

Simple project that transforms photo collections into stunning video sequences enhanced with custom OpenCV effects and real-time frame processing.

## Effects 

### Dotted Contour
- Evenly spaced glowing dots along contours
- Soft glow halos & color variations
- Smooth fade animations

<p align="center">
  <img src="gifs/hope-dots.gif" alt="Dotted Contour Effect" width="500">
</p>


### Ripple
- Multiple simultaneous spherical waves
- Dynamic ring width & acceleration
- Chromatic aberration edges

<p align="center">
  <img src="gifs/carl-ripple.gif" alt="Ripple Effect" width="500">
</p>


### Tone
- Preserves original brightness
- Metallic paint simulation
- Animated color palettes

<p align="center">
  <img src="gifs/carl-tone.gif" alt="Tone Effect" width="500">
</p>


### Chromatic Aberration
- Animated RGB channel shifting
- Adjustable intensity
- Mask-based application

<p align="center">
  <img src="gifs/andy-chromatic.gif" alt="Chromatic Aberration Effect" width="700">
</p>

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
