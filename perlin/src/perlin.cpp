// Perlin Noise Extension for Defold

#include <dmsdk/sdk.h>
#include <cmath>

// Permutation table
const int permutation[256] = {151,160,137,91,90,15, 
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142, 
   8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26, 
   197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149, 
   56,87,174,20,125,136,171,168,68,175,74,165,71,134,139, 
   48,27,166,77,146,158,231,83,111,229,122,60,211,133,230, 
   220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161, 
   1,216,80,73,209,76,132,187,208,89,18,169,200,196,135, 
   130,116,188,159,86,164,100,109,198,173,186,3,64,52, 
   217,226,250,124,123,5,202,38,147,118,126,255,82,85,212, 
   207,206,59,227,47,16,58,17,182,189,28,42,223,183,170, 
   213,119,248,152,2,44,154,163,70,221,153,101,155, 
   167,43,172,9,129,22,39,253,19,98,108,110,79,113,224, 
   232,178,185,112,104,218,246,97,228,251,34,242,193, 
   238,210,144,12,191,179,162,241,81,51,145,235,249, 
   14,239,107,49,192,214,31,181,199,106,157,184, 
   84,204,176,115,121,50,45,127,4,150,254,138, 
   236,205,93,222,114,67,29,24,72,243,141,128,195, 
   78,66,215,61,156,180};

static double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static double lerp(double t, double a, double b) {
    return a + t * (b - a);
}

static double grad(int hash, double x, double y = 0, double z = 0, double w = 0) {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

static double Perlin_Noise1D_Internal(double x) {
    int x0 = floor(x);
    int x1 = x0 + 1;
    double t = x - x0;
    double t_fade = fade(t);

    int hash_x0 = permutation[x0 & 255];
    int hash_x1 = permutation[x1 & 255];

    double grad_x0 = grad(hash_x0, x - x0);
    double grad_x1 = grad(hash_x1, x - x1);

    double noise = lerp(t_fade, grad_x0, grad_x1);
    return noise;
}

static double Perlin_Noise2D_Internal(double x, double y) {
    int x0 = floor(x);
    int y0 = floor(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    double t_x = x - x0;
    double t_y = y - y0;
    double t_fade_x = fade(t_x);
    double t_fade_y = fade(t_y);

    int hash_x0y0 = permutation[(x0 + permutation[y0 & 255]) & 255];
    int hash_x1y0 = permutation[(x1 + permutation[y0 & 255]) & 255];
    int hash_x0y1 = permutation[(x0 + permutation[y1 & 255]) & 255];
    int hash_x1y1 = permutation[(x1 + permutation[y1 & 255]) & 255];

    double grad_x0y0 = grad(hash_x0y0, x - x0, y - y0);
    double grad_x1y0 = grad(hash_x1y0, x - x1, y - y0);
    double grad_x0y1 = grad(hash_x0y1, x - x0, y - y1);
    double grad_x1y1 = grad(hash_x1y1, x - x1, y - y1);

    double noise_x0y0 = lerp(t_fade_x, grad_x0y0, grad_x1y0);
    double noise_x0y1 = lerp(t_fade_x, grad_x0y1, grad_x1y1);
    double noise = lerp(t_fade_y, noise_x0y0, noise_x0y1);
    return noise;
}

static double Perlin_Noise3D_Internal(double x, double y, double z) {
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    int Z = (int)floor(z) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    int A = permutation[X] + Y;
    int AA = permutation[A] + Z;
    int AB = permutation[A + 1] + Z;
    int B = permutation[X + 1] + Y;
    int BA = permutation[B] + Z;
    int BB = permutation[B + 1] + Z;

    double result = lerp(w, lerp(v, lerp(u, grad(permutation[AA], x, y, z),
                                    grad(permutation[BA], x - 1, y, z)),
                            lerp(u, grad(permutation[AB], x, y - 1, z),
                                    grad(permutation[BB], x - 1, y - 1, z))),
                    lerp(v, lerp(u, grad(permutation[AA + 1], x, y, z - 1),
                                    grad(permutation[BA + 1], x - 1, y, z - 1)),
                            lerp(u, grad(permutation[AB + 1], x, y - 1, z - 1),
                                    grad(permutation[BB + 1], x - 1, y - 1, z - 1))));
    
    return (result + 1.0) / 2.0;
}

static double Perlin_Noise4D_Internal(double x, double y, double z, double w) {
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    int Z = (int)floor(z) & 255;
    int W = (int)floor(w) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    w -= floor(w);

    double u = fade(x);
    double v = fade(y);
    double t = fade(z);
    double s = fade(w);

    int A = permutation[X] + Y;
    int AA = permutation[A] + Z;
    int AB = permutation[A + 1] + Z;
    int B = permutation[X + 1] + Y;
    int BA = permutation[B] + Z;
    int BB = permutation[B + 1] + Z;

    double result = lerp(s, lerp(t, lerp(v, lerp(u, grad(permutation[AA], x, y, z, w),
                                    grad(permutation[BA], x - 1, y, z, w)),
                            lerp(u, grad(permutation[AB], x, y - 1, z, w),
                                    grad(permutation[BB], x - 1, y - 1, z, w))),
                    lerp(v, lerp(u, grad(permutation[AA + 1], x, y, z - 1, w),
                                    grad(permutation[BA + 1], x - 1, y, z - 1, w)),
                            lerp(u, grad(permutation[AB + 1], x, y - 1, z - 1, w),
                                    grad(permutation[BB + 1], x - 1, y - 1, z - 1, w)))),
            lerp(t, lerp(v, lerp(u, grad(permutation[AA + W], x, y, z, w - 1),
                                    grad(permutation[BA + W], x - 1, y, z, w - 1)),
                            lerp(u, grad(permutation[AB + W], x, y - 1, z, w - 1),
                                    grad(permutation[BB + W], x - 1, y - 1, z, w - 1))),
                    lerp(v, lerp(u, grad(permutation[AA + 1 + W], x, y, z - 1, w - 1),
                                    grad(permutation[BA + 1 + W], x - 1, y, z - 1, w - 1)),
                            lerp(u, grad(permutation[AB + 1 + W], x, y - 1, z - 1, w - 1),
                                    grad(permutation[BB + 1 + W], x - 1, y - 1, z - 1, w - 1)))));

    return (result + 1.0) / 2.0;
}

// 1D float Perlin noise
static int Perlin_Noise1D(lua_State* L)
{
    double x = luaL_checknumber(L, 1);
    double frequency = luaL_checknumber(L, 2);
    double amplitude = luaL_checknumber(L, 3);
    double octave_count = luaL_checknumber(L, 4);
    double persistence = luaL_checknumber(L, 5);

    double noise = 0.0;
    double total_amplitude = 0.0;

    for (int i = 0; i < octave_count; i++)
    {
        double freq = frequency * pow(2, i);
        double amp = amplitude * pow(persistence, i);

        noise += amp * (1 + Perlin_Noise1D_Internal(x * freq)) / 2;
        total_amplitude += amp;
    }

    noise /= total_amplitude;

    lua_pushnumber(L, noise);
    return 1;
}

// 2D float Perlin noise
static int Perlin_Noise2D(lua_State* L)
{
    double x = luaL_checknumber(L, 1);
    double y = luaL_checknumber(L, 2);
    double frequency = luaL_checknumber(L, 3);
    double amplitude = luaL_checknumber(L, 4);
    double octave_count = luaL_checknumber(L, 5);
    double persistence = luaL_checknumber(L, 6);

    double noise = 0.0;
    double total_amplitude = 0.0;

    for (int i = 0; i < octave_count; i++)
    {
        double freq = frequency * pow(2, i);
        double amp = amplitude * pow(persistence, i);

        noise += amp * (1 + Perlin_Noise2D_Internal(x * freq, y * freq)) / 2;
        total_amplitude += amp;
    }

    noise /= total_amplitude;

    lua_pushnumber(L, noise);
    return 1;
}

// 3D float Perlin noise
static int Perlin_Noise3D(lua_State* L)
{
    double x = luaL_checknumber(L, 1);
    double y = luaL_checknumber(L, 2);
    double z = luaL_checknumber(L, 3);
    double frequency = luaL_checknumber(L, 4);
    double amplitude = luaL_checknumber(L, 5);
    double octave_count = luaL_checknumber(L, 6);
    double persistence = luaL_checknumber(L, 7);

    double noise = 0.0;
    double total_amplitude = 0.0;

    for (int i = 0; i < octave_count; i++)
    {
        double freq = frequency * pow(2, i);
        double amp = amplitude * pow(persistence, i);

        noise += amp * (1 + Perlin_Noise3D_Internal(x * freq, y * freq, z * freq)) / 2;
        total_amplitude += amp;
    }

    noise /= total_amplitude;

    lua_pushnumber(L, noise);
    return 1;
}

// 4D float Perlin noise
static int Perlin_Noise4D(lua_State* L)
{
    double x = luaL_checknumber(L, 1);
    double y = luaL_checknumber(L, 2);
    double z = luaL_checknumber(L, 3);
    double w = luaL_checknumber(L, 4);
    double frequency = luaL_checknumber(L, 5);
    double amplitude = luaL_checknumber(L, 6);
    double octave_count = luaL_checknumber(L, 7);
    double persistence = luaL_checknumber(L, 8);

    double noise = 0.0;
    double total_amplitude = 0.0;

    for (int i = 0; i < octave_count; i++)
    {
        double freq = frequency * pow(2, i);
        double amp = amplitude * pow(persistence, i);

        noise += amp * (1 + Perlin_Noise4D_Internal(x * freq, y * freq, z * freq, w * freq)) / 2;
        total_amplitude += amp;
    }

    noise /= total_amplitude;

    lua_pushnumber(L, noise);
    return 1;
}

// Lua module function mapping
static const luaL_Reg Module_methods[] = {
    {"noise_1d", Perlin_Noise1D},
    {"noise_2d", Perlin_Noise2D},
    {"noise_3d", Perlin_Noise3D},
    {"noise_4d", Perlin_Noise4D},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);
    luaL_register(L, "perlin", Module_methods);
    lua_pop(L, 1);
}

// Defold SDK functions

static dmExtension::Result Initialize(dmExtension::Params* params)
{
    LuaInit(params->m_L);
	dmLogInfo("Registered %s Extension\n", "perlin");
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(perlin, 'perlin', 0, 0, Initialize, 0, 0, 0)
