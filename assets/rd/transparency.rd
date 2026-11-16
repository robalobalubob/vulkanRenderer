# Transparency (alpha blending) test scene
#
# Any surface with Opacity < 1 becomes an alpha-blended material. The renderer
# draws every opaque object first, then the transparent ones sorted
# back-to-front by the distance from each object's origin to the camera, with
# depth testing on and depth writes off. Each group below checks one piece of
# that, so a regression changes this scene's render test image.
#
# Only convex, single-sided shapes (spheres, cubes) are transparent here.
# Two-sided open surfaces (Cylinder, Cone, ...) would blend their inner and
# outer faces in index order rather than depth order, which a per-object sort
# cannot fix; that is a known limitation, not something to pin in a golden.
#
# Layout (z-up, camera looking along +y):
#   x = -5  sort order: three overlapping spheres in a line away from the camera
#   x =  0  glass in front of a bright highlight: blending happens in HDR now
#   x = +5  depth test: an opaque cube in front of a transparent one

Display "Transparency" "Screen" "rgbdouble"
Format 800 600

CameraEye 0 -15 6
CameraAt 0 0 1
CameraUp 0 0 1
CameraFOV 45
Clipping 0.5 100
Background 0.05 0.05 0.08

WorldBegin

AmbientLight 0.35 0.35 0.4 1.0
FarLight 0.3 -0.8 0.6  1.0 0.97 0.9  1.4
# Kept outside every primitive's bounding sphere: the loader gives a glow to a
# primitive "hosting" a point light, and the ground disk's sphere is large.
PointLight 3 -4 5  1.0 0.95 0.85  60.0

# ---------------------------------------------------------------------------
# Ground (opaque)
# ---------------------------------------------------------------------------
Surface "matte"
Ka 0.4
Kd 0.8
Opacity 1
Color 0.45 0.45 0.48
Disk 0 11 360

# ---------------------------------------------------------------------------
# x = -5: sort order. Red is nearest the camera, then green, then blue.
# Drawn back-to-front, the overlaps tint toward the nearest sphere's colour;
# a wrong order visibly changes the overlap colours.
# ---------------------------------------------------------------------------
Surface "plastic"
Ka 0.3
Kd 0.7
Ks 0.5
Specular 1 1 1 60
Opacity 0.5

Color 0.2 0.35 1.0
XformPush
Translate -4.3 2.0 1.4
Sphere 1.4 -1.4 1.4 360
XformPop

Color 0.2 0.95 0.3
XformPush
Translate -5.0 0.0 1.4
Sphere 1.4 -1.4 1.4 360
XformPop

Color 1.0 0.25 0.2
XformPush
Translate -5.7 -2.0 1.4
Sphere 1.4 -1.4 1.4 360
XformPop

# ---------------------------------------------------------------------------
# x = 0: a cyan glass sphere half in front of an opaque white sphere lit by
# the point light, half in front of the ground. Blending happens in HDR,
# before tone mapping: over the ground the glass reads clearly cyan, while
# over the white sphere (well above 1.0) it stays nearly white, as a bright
# light seen through tinted glass does.
# ---------------------------------------------------------------------------
Opacity 1
Color 0.95 0.95 0.95
Ks 0.8
Specular 1 1 1 80
XformPush
Translate 0.6 1.5 1.3
Sphere 1.3 -1.3 1.3 360
XformPop

Opacity 0.35
Color 0.2 0.75 1.0
Ks 0.6
Specular 1 1 1 120
XformPush
Translate -0.6 -1.0 1.8
Sphere 1.0 -1.0 1.0 360
XformPop

# ---------------------------------------------------------------------------
# x = +5: depth test. A transparent orange cube sits behind an opaque purple
# cube that covers its lower-left part. Transparent draws still depth-test
# against opaque ones, so the covered part must stay hidden.
# ---------------------------------------------------------------------------
Opacity 0.45
Color 1.0 0.6 0.15
XformPush
Translate 5.3 2.0 1.6
Rotate "Z" 20
Scale 1.6 1.6 1.6
Cube
XformPop

Opacity 1
Color 0.55 0.25 0.8
Ks 0.4
Specular 1 1 1 40
XformPush
Translate 4.3 -1.5 0.9
Rotate "Z" -15
Scale 0.9 0.9 0.9
Cube
XformPop

WorldEnd
