# defold-perlin-noise

A Defold native extension attempting to implement 1D, 2D, 3D and 4D Perlin Noise.

Not yet tested.

## Usage

```lua
local noise = perlin.noise_1d(0.5, 1.0, 1.0, 4, 0.5)
print(noise) -- 0.685

local noise2 = perlin.noise_2d(0.5, 0.5, 1.0, 1.0, 4, 0.5)
print(noise2) -- 0.807

local noise3 = perlin.noise_3d(0.5, 0.5, 0.5, 1.0, 1.0, 4, 0.5)
print(noise3) -- 0.753

local noise4 = perlin.noise_4d(0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 4, 0.5)
print(noise4) -- 0.532
```

## Background

Created with Chat-GPT. Prompt:

```
create a defold extension that exposes a module to lua, use c++ that does not use any features newer than 2009, and does not use the standard library, and always uses const char* instead of std::string.

the name of the module is perlin.

it must implement "Improved Noise" by Ken Perlin and expose it to lua. use only public domain sources.

implement 1D float Perlin noise, 2D float Perlin noise, 3D float Perlin noise, 4d float Perlin noise.

fill in the permutation table.
```
